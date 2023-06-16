#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "termui.h"
#include "nec.h"

#define str_push(__str_a, __str_c) (__str_a[nec_size(__str_a) - 1] = __str_c, nec_push(__str_a, 0))
#define str_pop(__str_a) (nec_size(__str_a) > 1 ? (nec_pop(__str_a), __str_a[nec_size(__str_a) - 1] = 0) : 0)

char* str_cpy(const char* src)
{
    char* string = 0;
    nec_push(string, 0);
    if(!src) return string;
    while(*src != 0) str_push(string, *src++);
    return string;
}

char* str_trim(const char* src)
{
    char* string = 0, c;
    while(isspace(*src)) src++;
    while(*src) nec_push(string, *src++);
    while(string && isspace(string[nec_size(string) - 1])) nec_pop(string);
    nec_push(string, 0);
    return string;
}

termui* build_message(const char* sender, const char* text)
{
    return termui_box(TERMUI_EXPAND, 0, 2,
        termui_box(TERMUI_ROW | TERMUI_EXPAND, 0, 1,
            termui_box(0, 1, 0, 0),
            termui_text(sender, termui_box(0, 5, 1, 0)),
            termui_box(0, 1, 0, 0),
            termui_text(text, termui_box(TERMUI_EXPAND, 0, 1, 0)),
        0),
        termui_box(0, 0, 1, 0),
    0);
}

int main()
{
    termui_init();

    termui *inputKey, *peopleKey, *messagesKey;
    termui* root = termui_box(TERMUI_ROW | TERMUI_EXPAND, 0, 0,
        termui_box(TERMUI_EXPAND, 0, 0,
            messagesKey = termui_title("Messages", termui_box(TERMUI_BORDER | TERMUI_EXPAND, 0, 0,
                termui_box(TERMUI_EXPAND, 0, 0, 0),
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

    char* input = str_cpy(0), keyCode;
    termui_fullscreen(root);
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
            if(strcmp(input, "/exit") == 0) break;
            if(strcmp(input, "/people") == 0)
            {
                peopleKey->isEnabled = !peopleKey->isEnabled;
            }
            else
            {
                char* msg = str_trim(input);
                if(strlen(msg) == 0) nec_free(msg);
                else nec_push(messagesKey->children,
                    build_message("Seha:", msg)
                );
            }
next:
            nec_free(input);
            input = str_cpy(0);
            inputKey->text = input;
            termui_fullscreen(root);
            termui_plot(root);
            continue;
        }
        if(keyCode < 32 || keyCode > 126) continue;
        str_push(input, keyCode);
    }

    nec_free(input);
    for(int i = 1; i < nec_size(messagesKey->children); i++)
    {
        nec_free(messagesKey->children[i]->children[0]->children[3]->text);
    }
    termui_free(root);
    return termui_deinit();
}

/*
int main()
{
    termui_init();

    const int towerWidth = 60;
    const int towerHeight = 30;

    termui *root, *scrollKey;
    root = termui_box(TERMUI_EXPAND | TERMUI_ROW, 0, 0,
        termui_box(TERMUI_EXPAND | TERMUI_BORDER, towerWidth, 0, 0),
        termui_box(TERMUI_EXPAND, 0, 0,
            termui_box(TERMUI_EXPAND | TERMUI_BORDER, 0, towerHeight, 0),
            scrollKey = termui_box(TERMUI_EXPAND | TERMUI_BORDER | TERMUI_ROW, 0, 0,
                //termui_text("123456789", termui_box(0, 3, 3, 0)),
                termui_box(TERMUI_BORDER, 3, 3, 0),
            0),
            termui_box(TERMUI_EXPAND | TERMUI_BORDER, 0, towerHeight, 0),
        0),
        termui_box(TERMUI_EXPAND | TERMUI_BORDER, towerWidth, 0, 0),
    0);

    printf(TERMUI_NOCURSOR);
    while(1)
    {
        termui_terminal_size(&root->width, &root->height);
        termui_plot(root);
        char c = termui_read(0);
        if(c == 'q') break;
        if(c != 'h' && c != 'l') continue;
        if(c == 'h') scrollKey->scroll--;
        if(c == 'l') scrollKey->scroll++;
    }

    termui_free(root);
    termui_deinit();
    return 0;
}
*/

