#include "termui.h"
#include "nec.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

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

void termui_deinit(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOriginal);
    printf(TERMUI_RESET TERMUI_CLEAR TERMUI_MC, 1, 1);
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

    obj->width = w.ws_col;
    obj->height = w.ws_row;
    const int hasBorder = obj->flags & TERMUI_BORDER;
    obj->left = obj->top = obj->drawLeft = obj->drawTop = hasBorder;
    obj->right = obj->drawRight = obj->width - 1 - hasBorder;
    obj->bottom = obj->drawBottom = obj->height - 1 - hasBorder;
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
    va_end(args);

    termui object = {
        children, 0, 0, width, height, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, flags
    };
    termui* object_pointer = 0;
    nec_push(object_pointer, object);
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

void termui_focus(const termui* obj)
{
    const int width = obj->right - obj->left + 1; // REF: if focus point outside, don't focus
    const int textSize = obj->text ? strlen(obj->text) : 0;
    const int lines = textSize / width;
    const int chars = textSize % width;
    printf(TERMUI_MC, obj->top + lines + 1, obj->left + chars + 1);
    fflush(stdout);
}

void print_if_inside(const termui* obj, int x, int y, char c)
{
    const int l = obj->drawLeft; // TODO: don't use variables
    const int r = obj->drawRight;
    const int t = obj->drawTop;
    const int b = obj->drawBottom;
    if(x < l || x > r || y < t || y > b)
    {
        //printf("ERROR\n");
        //exit(1);
        return;
    }
    printf(TERMUI_MC "%c", y + 1, x + 1, c);
}

void clear_view(const termui* obj)
{
    for(int y = obj->drawTop; y <= obj->drawBottom; y++)
    {
        for(int x = obj->drawLeft; x <= obj->drawRight; x++)
        {
            print_if_inside(obj, x, y, ' ');
        }
    }
}

void draw_text(const termui* obj)
{
    if(obj->text[0] == 0) return;
    const int width = obj->right - obj->left + 1;
    const int height = obj->bottom - obj->top + 1;
    const int area = width * height;
    const int textSize = strlen(obj->text);

    int i = 0;
    for(int y = obj->top; y <= obj->bottom; y++)
    {
        for(int x = obj->left; x <= obj->right; x++)
        {
            char c = obj->text[i];
            if(textSize > area && area - i < 4) c = '.';
            print_if_inside(obj, x, y, c);
            if(++i >= textSize) return;
        }
    }
}

void draw_border(const termui* obj)
{
    termui real = *obj;
    if(real.left-- == real.drawLeft) real.drawLeft--;
    if(real.right++ == real.drawRight) real.drawRight++;
    if(real.top-- == real.drawTop) real.drawTop--;
    if(real.bottom++ == real.drawBottom) real.drawBottom++;
    for(int y = real.top; y <= real.bottom; y++)
    {
        for(int x = real.left; x <= real.right; x++)
        {
            if(y != real.top && y != real.bottom && x != real.left && x != real.right) continue;
            char c = y != real.top && y != real.bottom ? '|' : '-';
            if(x == real.left && y == real.top) c = '/';
            if(x == real.right && y == real.bottom) c = '/';
            if(x == real.right && y == real.top) c = '\\';
            if(x == real.left && y == real.bottom) c = '\\';
            print_if_inside(&real, x, y, c);
        }
    }
    if(!real.title) return;
    real.left += 2;
    real.right -= 2;
    real.bottom = real.top;
    real.text = real.title;
    draw_text(&real);
}

int max(int a, int b)
{
    return a > b ? a : b;
}

int min(int a, int b)
{
    return a > b ? b : a;
}

void termui_plot(const termui* obj)
{
    const int isRow = (obj->flags & TERMUI_ROW) >> 1;
    const int isReverse = (obj->flags & TERMUI_REVERSE) >> 2;
    const int mainAxis = (isRow ? obj->right - obj->left : obj->bottom - obj->top) + 1;
    const int crossAxis = (isRow ? obj->bottom - obj->top : obj->right - obj->left) + 1;

    int expandedChildCount = 0;
    int freeSpace = mainAxis;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui* child = obj->children[i];
        if(!child->isEnabled) continue;
        const int mainAxis = isRow ? child->width : child->height;
        if(mainAxis == 0)
        {
            expandedChildCount++;
            continue;
        }
        freeSpace -= mainAxis;
    }
    const int expandedSize = max(0, freeSpace / max(1, expandedChildCount));

    clear_view(obj);

    int axisOffset = obj->scroll + freeSpace * isReverse;
    for(int i = 0; i < nec_size(obj->children); i++)
    {
        termui* child = obj->children[i];
        if(!child->isEnabled) continue;

        int width = child->width;
        int height = child->height;
        int* childMainAxis = isRow ? &width : &height;
        int* childCrossAxis = isRow ? &height : &width;
        if(*childMainAxis == 0) *childMainAxis = expandedSize;
        if(*childCrossAxis == 0) *childCrossAxis = crossAxis;

        const int hasBorder = child->flags & TERMUI_BORDER;
        child->left = obj->left + axisOffset * isRow + hasBorder;
        child->top = obj->top + axisOffset * !isRow + hasBorder;
        child->right = child->left + width - 1 - 2 * hasBorder;
        child->bottom = child->top + height - 1 - 2 * hasBorder;

        child->drawLeft = max(child->left, obj->drawLeft);
        child->drawTop = max(child->top, obj->drawTop);
        child->drawRight = min(child->right, obj->drawRight);
        child->drawBottom = min(child->bottom, obj->drawBottom);

        termui_plot(child);

        // if(child->drawRight < child->drawLeft || child->drawBottom < child->drawTop) break;
        axisOffset += *childMainAxis;
    }

    if(obj->flags & TERMUI_BORDER) draw_border(obj);
    if(obj->text) draw_text(obj);

    fflush(stdout);
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

