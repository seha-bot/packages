#include "termui.h"
#include "nec.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

// TODO: fix expanded last line empty (possible hint: uneven or even size)

void termui_init(void)
{
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    printf(TERMUI_RESET);
}

void termui_terminal_size(int* width, int* height)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    *width = w.ws_col;
    *height = w.ws_row;
}

char termui_read(char* out)
{
    char c;
    read(STDIN_FILENO, &c, 1);
    if(out) *out = c;
    return c;
}

termui* termui_box(char flags, int width, int height, ...)
{
    va_list args;
    va_start(args, height);

    termui** children = 0;

    while(1)
    {
        termui* child = va_arg(args, termui*);
        if(!child) break;
        nec_push(children, child);
    }

    termui object = { children, width, height, 0, 0, 0, 0, flags };
    termui* object_pointer = 0;
    nec_push(object_pointer, object);
    va_end(args);
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

termui* termui_enabled(int isEnabled, termui* obj)
{
    if(isEnabled) obj->flags &= ~TERMUI_DISABLED;
    else obj->flags |= TERMUI_DISABLED;
    return obj;
}

void draw_border(termui* obj)
{
    for(int y = 0; y < obj->height; y++)
    {
        printf(TERMUI_MC, obj->y + 1 + y, obj->x + 1);
        for(int x = 0; x < obj->width; x++)
        {
            if(!y && !x || y == obj->height - 1 && x == obj->width - 1) printf("/");
            else if(!y && x == obj->width - 1 || !x && y == obj->height - 1) printf("\\");
            else if(!y || y == obj->height - 1) printf("-");
            else if(!x || x == obj->width - 1) printf("|");
            else printf(" ");
        }
    }
}

void draw_text(const char* text, int width, int height, int x1, int y1)
{
    const int area = width * height;
    int textSize = strlen(text);
    const int hasOverflow = textSize > area;

    int i = 0;
    for(int y = 0; y < height; y++)
    {
        printf(TERMUI_MC, y + y1 + 1, x1 + 1);
        for(int x = 0; x < width; x++)
        {
            if(hasOverflow && area - i < 4)
            {
                printf("%c", '.');
                continue;
            }
            printf("%c", text[i]);
            if(++i >= textSize) return;
        }
    }
}

void draw_title(termui* obj)
{
    int x = obj->x + 2;
    int y = obj->y;
    int width = obj->width - 4;
    draw_text(obj->title, width, 1, x, y);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

#define MAIN_AXIS(o) (isRow ? o->width : o->height)
#define CROSS_AXIS(o) (isRow ? o->height : o->width)

int error(const char* error)
{
    printf(TERMUI_CLEAR);
    printf(TERMUI_MC, 1, 1);
    printf("%s\n", error);
    return 1;
}

int termui_plot(termui* obj, int singleOrParentExpanded)
{
    const int hasBorder = obj->flags & TERMUI_BORDER;
    const int isRow = (obj->flags & TERMUI_ROW) >> 1;
    const int isExpanded = (obj->flags & TERMUI_EXPAND) >> 2;
    if(obj->title && !hasBorder) return error("Objects with title must have a border!");
    if(hasBorder) draw_border(obj);
    if(obj->title) draw_title(obj);

    int expandedChildCount = 0;
    int freeSpace = MAIN_AXIS(obj) - 2 * hasBorder;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui* child = obj->children[i];
        if(child->flags & TERMUI_EXPAND && MAIN_AXIS(child) == 0)
        {
            expandedChildCount++;
            continue;
        }
        freeSpace -= MAIN_AXIS(child);
    }
    const int expandedSize = freeSpace / max(expandedChildCount, 1);

    int axisOffset = 0, crossAxisSize = 0;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui* child = obj->children[i];
        if(child->flags & TERMUI_DISABLED) continue;

        child->x = obj->x + hasBorder + axisOffset * isRow;
        child->y = obj->y + hasBorder + axisOffset * !isRow;

        const int oldSize[2] = { child->width, child->height };
        int* mainAxis = isRow ? &child->width : &child->height;
        int* crossAxis = isRow ? &child->height : &child->width;
        if(child->flags & TERMUI_EXPAND)
        {
            if(*mainAxis == 0) *mainAxis = expandedSize;
            if(singleOrParentExpanded == 0 && *crossAxis == 0) *crossAxis = CROSS_AXIS(obj) - 2 * hasBorder;
        }

        if(termui_plot(child, nec_size(obj->children) * !isExpanded)) return 1;

        axisOffset += *mainAxis;
        crossAxisSize = max(crossAxisSize, *crossAxis);

        child->width = oldSize[0];
        child->height = oldSize[1];
    }

    if(obj->text) draw_text(obj->text,
        obj->width - 2 * hasBorder, obj->height - 2 * hasBorder,
        obj->x + hasBorder, obj->y + hasBorder
    );

    fflush(stdout);
    return 0;
}

