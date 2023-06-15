#include <stdio.h>
#include <string.h>
#include "termui.h"
#include "nec.h"

termui* build_message(const char* sender, const char* text)
{
    return termui_box(TERMUI_ROW | TERMUI_EXPAND, 0, 1,
        termui_box(0, 1, 0, 0),
        termui_text(sender, termui_box(0, 5, 1, 0)),
        termui_box(0, 2, 0, 0),
        termui_text(text, termui_box(TERMUI_EXPAND, 0, 1, 0)),
    0);
}

int main()
{
    termui_init();

    termui* key;
    termui* root = termui_box(TERMUI_ROW | TERMUI_EXPAND, 0, 0,
        termui_box(TERMUI_EXPAND, 0, 0,
            termui_title("Messages", termui_box(TERMUI_BORDER | TERMUI_EXPAND, 0, 0,
                termui_box(TERMUI_EXPAND, 0, 0, 0),
                build_message("Seha:", "Kako je brate?"),
                termui_box(0, 0, 1, 0),
                build_message("Vapi:", "Dobro je Elham!"),
                termui_box(0, 0, 1, 0),
            0)),
            termui_title("Input text...", termui_box(TERMUI_BORDER | TERMUI_ROW | TERMUI_EXPAND, 0, 5,
                termui_box(0, 1, 0, 0),
                key = termui_box(TERMUI_EXPAND, 0, 0, 0),
                termui_box(0, 1, 0, 0),
            0)),
        0),
        termui_title("People", termui_box(TERMUI_BORDER | TERMUI_EXPAND, 20, 0,
            termui_text(" - Seha", termui_box(TERMUI_EXPAND, 0, 1, 0)),
            termui_text(" - Vapi", termui_box(TERMUI_EXPAND, 0, 1, 0)),
            termui_text(" - xX_GameSlayer_Xx", termui_box(TERMUI_EXPAND, 0, 1, 0)),
        0)),
    0);

    char* input = 0, c;
    nec_push(input, 0);
    termui_terminal_size(&root->width, &root->height);
    termui_plot(root);
    while(1)
    {
        key->text = input;
        termui_replot(key);
        termui_focus(key);
        termui_read((char*)&c);
        if(c == 127 && nec_size(input) > 1)
        {
            nec_pop(input);
            input[nec_size(input) - 1] = 0;
            continue;
        }
        if(c == 13)
        {
            if(strcmp(input, "exit") == 0) return 0;
            nec_free(input);
            nec_push(input, 0);
            continue;
        }
        if(c < 32 || c > 126) continue;
        input[nec_size(input) - 1] = c;
        nec_push(input, 0);
    }
    return 0;
}

