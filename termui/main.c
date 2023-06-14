#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "termui.h"

volatile char c = 0;

void* always()
{
    while(1) termui_read((char*)&c);
    return 0;
}

termui* build_message(const char* sender, const char* text)
{
    return termui_init(TERMUI_ROW | TERMUI_EXPAND, 0, 1,
        termui_init(0, 1, 0, 0),
        termui_text(sender, termui_init(0, 5, 1, 0)),
        termui_init(0, 2, 0, 0),
        termui_text(text, termui_init(TERMUI_EXPAND, 0, 1, 0)),
    0);
}

int main()
{
    termui_raw_mode(1);
    printf(TERMUI_NOBLINK);

    termui* root = termui_init(TERMUI_ROW | TERMUI_EXPAND, 0, 0,
        termui_init(TERMUI_EXPAND, 0, 0,
            termui_title("Messages", termui_init(TERMUI_BORDER | TERMUI_EXPAND, 0, 0,
                termui_init(TERMUI_EXPAND, 0, 0, 0),
                build_message("Seha:", "Kako je brate?"),
                termui_init(0, 0, 1, 0),
                build_message("Vapi:", "Dobro je Elham!"),
                termui_init(0, 0, 1, 0),
            0)),
            termui_text("Input text...", termui_init(TERMUI_BORDER | TERMUI_EXPAND, 0, 5, 0)),
        0),
        termui_title("People", termui_init(TERMUI_BORDER | TERMUI_EXPAND, 20, 0,
            termui_text(" - Seha", termui_init(TERMUI_EXPAND, 0, 1, 0)),
            termui_text(" - Vapi", termui_init(TERMUI_EXPAND, 0, 1, 0)),
            termui_text(" - xX_GameSlayer_Xx", termui_init(TERMUI_EXPAND, 0, 1, 0)),
        0)),
    0);

    pthread_t tid = 0;
    pthread_create(&tid, 0, always, 0);

    while(c != 'q')
    {
        termui_terminal_size(&root->width, &root->height);
        termui_plot(root, 0);
        usleep(10000);
    }
    return 0;
}

