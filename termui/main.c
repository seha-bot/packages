#include <stdio.h>
#include <string.h>
#include "termui.h"
#include "nec.h"

char* str_init(void)
{
    char* string = 0;
    nec_push(string, 0);
    return string;
}

#define str_push(__str_a, __str_c) (__str_a[nec_size(__str_a) - 1] = __str_c, nec_push(__str_a, 0))
#define str_pop(__str_a) (nec_size(__str_a) > 1 ? (nec_pop(__str_a), __str_a[nec_size(__str_a) - 1] = 0) : 0)

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

    termui *inputKey, *peopleKey;
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
                inputKey = termui_box(TERMUI_EXPAND, 0, 0, 0),
                termui_box(0, 1, 0, 0),
            0)),
        0),
        peopleKey = termui_title("People", termui_box(TERMUI_BORDER | TERMUI_EXPAND, 20, 0,
            termui_text(" - Seha", termui_box(TERMUI_EXPAND, 0, 1, 0)),
            termui_text(" - Vapi", termui_box(TERMUI_EXPAND, 0, 1, 0)),
            termui_text(" - xX_GameSlayer_Xx", termui_box(TERMUI_EXPAND, 0, 1, 0)),
        0)),
    0);

    char* input = str_init(), keyCode;
    termui_terminal_size(&root->width, &root->height);
    termui_plot(root);
    while(1)
    {
        inputKey->text = input;
        termui_replot(inputKey);
        termui_focus(inputKey);
        termui_read((char*)&keyCode);
        if(keyCode == 127 && nec_size(input) > 1)
        {
            str_pop(input);
            continue;
        }
        if(keyCode == 13)
        {
            if(strcmp(input, "/exit") == 0) return termui_deinit();
            if(strcmp(input, "/people") == 0)
            {
                peopleKey->isEnabled = !peopleKey->isEnabled;
            }
            nec_free(input);
            input = str_init();
            termui_terminal_size(&root->width, &root->height);
            termui_plot(root);
            continue;
        }
        if(keyCode < 32 || keyCode > 126) continue;
        str_push(input, keyCode);
    }
    return termui_deinit();
}

