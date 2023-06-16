#include "termui.h"
#include "nec.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

// TODO: fix expanded last line empty (possible hint: uneven or even size)

struct termios termiosOriginal;
void termui_init(void)
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    termiosOriginal = raw;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf(TERMUI_RESET);
}

int termui_deinit(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginal);
    printf(TERMUI_RESET TERMUI_CLEAR TERMUI_MC, 1, 1);
    return 0;
}

char termui_read(char* out)
{
    char c;
    read(STDIN_FILENO, &c, 1);
    if(out) *out = c;
    return c;
}

void termui_fullscreen(termui* obj)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    obj->width = obj->plotWidth = w.ws_col;
    obj->height = obj->plotHeight = w.ws_row;
}

termui* termui_box(char flags, int width, int height, ...)
{
    termui object = { 0, 0, 0, 0, width, height, 0, 0, 0, 0, 0, 1, flags };
    termui* object_pointer = 0;
    nec_push(object_pointer, object);

    va_list args;
    va_start(args, height);

    termui** children = 0;
    while(1)
    {
        termui* child = va_arg(args, termui*);
        if(!child) break;
        child->parent = object_pointer;
        nec_push(children, child);
    }
    va_end(args);

    object_pointer->children = children;
    return object_pointer;
}

termui* termui_title(const char* title, termui* obj)
{
    obj->title = title;
    return obj;
}

termui* termui_text(const char* text, termui* obj)
{
    obj->text = text;
    return obj;
}

void termui_focus(termui* obj)
{
    const int width = obj->plotWidth - 2 * (obj->flags & TERMUI_BORDER);
    const int textSize = obj->text ? strlen(obj->text) : 0;
    const int lines = textSize / width;
    const int chars = textSize % width;
    printf(TERMUI_MC, obj->y + lines + 1, obj->x + chars + 1);
    fflush(stdout);
}

void print_if_inside(termui* obj, int x, int y, char c)
{
    x += obj->x;
    y += obj->y;
    if(!obj->parent)
    {
        printf(TERMUI_MC "%c", y + 1, x + 1, c);
        return;
    }
    const int hasBorder = obj->parent->flags & TERMUI_BORDER;
    const int l = obj->parent->x + hasBorder;
    const int r = obj->parent->x + obj->parent->plotWidth - 1 - hasBorder;
    const int t = obj->parent->y + hasBorder;
    const int b = obj->parent->y + obj->parent->plotHeight - 1 - hasBorder;
    if(x < l || x > r || y < t || y > b) return;
    printf(TERMUI_MC "%c", y + 1, x + 1, c);
}

void clear_view(termui* obj)
{
    for(int y = 0; y < obj->height; y++)
    {
        for(int x = 0; x < obj->width; x++)
        {
            print_if_inside(obj, x, y, ' ');
        }
    }
}

void draw_border(termui* obj)
{
    print_if_inside(obj, 0, 0, '/');
    for(int x = 1; x < obj->width - 1; x++)
    {
        print_if_inside(obj, x, 0, '-');
    }
    print_if_inside(obj, obj->width - 1, 0, '\\');
    for(int y = 1; y < obj->height - 1; y++)
    {
        print_if_inside(obj, 0, y, '|');
        print_if_inside(obj, obj->width - 1, y, '|');
    }
    print_if_inside(obj, 0, obj->height - 1, '\\');
    for(int x = 1; x < obj->width - 1; x++)
    {
        print_if_inside(obj, x, obj->height - 1, '-');
    }
    print_if_inside(obj, obj->width - 1, obj->height - 1, '/');
}

void draw_text(termui* obj)
{
    if(obj->text[0] == 0) return;
    const int hasBorder = obj->flags & TERMUI_BORDER;
    const int width = obj->width - 2 * hasBorder;
    const int height = obj->height - 2 * hasBorder;
    const int area = width * height;
    const int textSize = strlen(obj->text);

    int i = 0;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            char c = obj->text[i];
            if(textSize > area && area - i < 4) c = '.';
            print_if_inside(obj, x, y, c);
            if(++i >= textSize) return;
        }
    }
}

void draw_title(termui* obj)
{
    termui real = *obj;
    real.x += 2;
    real.width -= 4;
    real.text = obj->title;
    draw_text(&real);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

#define MAIN_AXIS(o) (isRow ? o->width : o->height)
#define CROSS_AXIS(o) (isRow ? o->height : o->width)

void termui_plot(termui* obj)
{
    const int hasBorder = obj->flags & TERMUI_BORDER;
    const int isRow = (obj->flags & TERMUI_ROW) >> 1;
    const int isExpanded = (obj->flags & TERMUI_EXPAND) >> 2;
    clear_view(obj);
    if(hasBorder)
    {
        draw_border(obj);
        if(obj->title) draw_title(obj);
    }

    int expandedChildCount = 0;
    int freeSpace = MAIN_AXIS(obj) - 2 * hasBorder;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui* child = obj->children[i];
        if(!child->isEnabled) continue;
        if(child->flags & TERMUI_EXPAND && MAIN_AXIS(child) == 0)
        {
            expandedChildCount++;
            continue;
        }
        freeSpace -= MAIN_AXIS(child);
    }
    const int expandedSize = max(0, freeSpace / max(1, expandedChildCount));

    int axisOffset = obj->scroll, crossAxisSize = 0;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui* child = obj->children[i];
        if(!child->isEnabled) continue;

        child->x = obj->x + hasBorder + axisOffset * isRow;
        child->y = obj->y + hasBorder + axisOffset * !isRow;

        const int oldSize[2] = { child->width, child->height };
        int* mainAxis = isRow ? &child->width : &child->height;
        int* crossAxis = isRow ? &child->height : &child->width;
        if(child->flags & TERMUI_EXPAND)
        {
            if(*mainAxis == 0) *mainAxis = expandedSize;
            if(*crossAxis == 0) *crossAxis = CROSS_AXIS(obj) - 2 * hasBorder;
        }

        child->plotWidth = child->width;
        child->plotHeight = child->height;
        termui_plot(child);

        axisOffset += *mainAxis;
        crossAxisSize = max(crossAxisSize, *crossAxis);

        child->width = oldSize[0];
        child->height = oldSize[1];
    }

    if(obj->text) draw_text(obj);

    fflush(stdout);
}

void termui_replot(termui* obj)
{
    const int oldSize[2] = { obj->width, obj->height };
    obj->width = obj->plotWidth;
    obj->height = obj->plotHeight;
    termui_plot(obj);
    obj->width = oldSize[0];
    obj->height = oldSize[1];
}

void termui_free(termui* obj)
{
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui_free(obj->children[i]);
    }
    nec_free(obj->children);
    nec_free(obj);
}

