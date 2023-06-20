#ifndef SEHA_TERMUI
#define SEHA_TERMUI

// COOL COLORS
#define TERMUI_RESET "\033[0m"
#define TERMUI_BLACK "\033[30m"
#define TERMUI_RED "\033[31m"
#define TERMUI_GREEN "\033[32m"
#define TERMUI_YELLOW "\033[33m"
#define TERMUI_PURPLE "\033[34m"
#define TERMUI_PINK "\033[35m"
#define TERMUI_BLUE "\033[36m"
#define TERMUI_WHITE "\033[37m"
#define TERMUI_BLACK_B "\033[40m"
#define TERMUI_RED_B "\033[41m"
#define TERMUI_GREEN_B "\033[42m"
#define TERMUI_YELLOW_B "\033[43m"
#define TERMUI_PURPLE_B "\033[44m"
#define TERMUI_PINK_B "\033[45m"
#define TERMUI_BLUE_B "\033[46m"
#define TERMUI_WHITE_B "\033[47m"

// CONTROL
#define TERMUI_CURSOR "\033[?25h"
#define TERMUI_NOCURSOR "\033[?25l"
#define TERMUI_MC "\033[%d;%dH"
#define TERMUI_CLEAR "\033[2J"

// FLAGS
#define TERMUI_BORDER 0x01
#define TERMUI_ROW    0x02
#define TERMUI_REVERSE 0x04

void termui_init(void);
void termui_deinit(void);
char termui_read(char*);

struct termui
{
    struct termui **children;
    const char *title, *text;
    int width, height; // The initial size
    int left, right, top, bottom; // Runtime calculated box area
    int drawLeft, drawRight, drawTop, drawBottom; // Runtime calculated drawing area
    int scroll;
    char isEnabled, flags; // move isEnabled inside flags?
};
typedef struct termui termui;

void termui_fullscreen(termui*);
termui* termui_box(char, int, int, ...);
termui* termui_title(const char*, termui*);
termui* termui_text(const char*, termui*);
void termui_focus(const termui*); // TODO: add hasFocus field and you know what to do on the color update
void termui_plot(const termui*);
void termui_free(termui*);

#endif /* SEHA_TERMUI */

