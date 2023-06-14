#ifndef SEHA_TERMUI
#define SEHA_TERMUI

// COOL COLORS
#define TERMUI_RESET "\033[0m"
#define TERMUI_OK "\033[30m"
#define TERMUI_BAD "\033[31m"

// CONTROL
#define TERMUI_NOBLINK "\033[?25l"
#define TERMUI_MC "\033[%d;%dH"
#define TERMUI_CLEAR "\033[2J"

// FLAGS
#define TERMUI_BORDER 0x01
#define TERMUI_ROW    0x02
#define TERMUI_EXPAND 0x04
#define TERMUI_DISABLED 0x08

void termui_init(void);
void termui_terminal_size(int*, int*);
char termui_read(char*);

struct termui
{
    struct termui **children;
    int width, height, x, y;
    const char *title, *text;
    char flags;
};
typedef struct termui termui;

termui* termui_box(char, int, int, ...);
termui* termui_title(const char*, termui*);
termui* termui_text(const char*, termui*);
termui* termui_enabled(int, termui*);
int termui_plot(termui*, int);

#endif /* SEHA_TERMUI */

