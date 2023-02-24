#include <linux/uinput.h>
#include <string.h>
#include <stdio.h>

#include "keys.h"
#include "utility.h"

struct key_lookup keys[100];

int find_key (char* name)
{
    for (int i = 0; i < KEYLIST_SIZE; i++)
    {
        if (strcmp(keys[i].name, name) == 0)
            return keys[i].code;
    }

    return -1;
}

struct key_lookup create_key (char* name, int code)
{
    struct key_lookup k;
    strcpy(k.name, name);
    k.code = code;

   return k;
}

void fill_keylist()
{
    int i=0;

    keys[i++] = create_key("KEY_0", KEY_0);
    keys[i++] = create_key("KEY_1", KEY_1);
    keys[i++] = create_key("KEY_2", KEY_2);
    keys[i++] = create_key("KEY_3", KEY_3);
    keys[i++] = create_key("KEY_4", KEY_4);
    keys[i++] = create_key("KEY_5", KEY_5);
    keys[i++] = create_key("KEY_6", KEY_6);
    keys[i++] = create_key("KEY_7", KEY_7);
    keys[i++] = create_key("KEY_8", KEY_8);
    keys[i++] = create_key("KEY_9", KEY_9);

    keys[i++] = create_key("KEY_D", KEY_D);
    keys[i++] = create_key("KEY_I", KEY_I);
    keys[i++] = create_key("KEY_S", KEY_S);
    keys[i++] = create_key("KEY_Q", KEY_Q);

    keys[i++] = create_key("KEY_SPACE", KEY_SPACE);

    keys[i++] = create_key("KEY_F1", KEY_F1);
    keys[i++] = create_key("KEY_F2", KEY_F2);
    keys[i++] = create_key("KEY_F3", KEY_F3);
    keys[i++] = create_key("KEY_F4", KEY_F4);
    keys[i++] = create_key("KEY_F5", KEY_F5);
    keys[i++] = create_key("KEY_F6", KEY_F6);
    keys[i++] = create_key("KEY_F7", KEY_F7);
    keys[i++] = create_key("KEY_F8", KEY_F8);
    keys[i++] = create_key("KEY_F9", KEY_F9);
    keys[i++] = create_key("KEY_F10", KEY_F10);
    keys[i++] = create_key("KEY_F11", KEY_F11);
    keys[i++] = create_key("KEY_F12", KEY_F12);

    keys[i++] = create_key("KEY_UP", KEY_UP);
    keys[i++] = create_key("KEY_DOWN", KEY_DOWN);
    keys[i++] = create_key("KEY_LEFT", KEY_LEFT);
    keys[i++] = create_key("KEY_RIGHT", KEY_RIGHT);

    keys[i++] = create_key("KEY_PAGEDOWN", KEY_PAGEDOWN);
    keys[i++] = create_key("KEY_UP", KEY_PAGEUP);

    keys[i++] = create_key("KEY_ENTER", KEY_ENTER);
    keys[i++] = create_key("KEY_RETURN", KEY_ENTER);

    keys[i++] = create_key("KEY_ESC", KEY_ESC);

    keys[i++] = create_key("WINKEY", KEY_LEFTMETA);
    keys[i++] = create_key("GUI", KEY_LEFTMETA);

    keys[i++] = create_key("CTRL", KEY_LEFTCTRL);
    keys[i++] = create_key("ALT", KEY_LEFTALT);
    keys[i++] = create_key("RALT", KEY_RIGHTALT);
    keys[i++] = create_key("SHIFT", KEY_LEFTSHIFT);

    keys[i++] = create_key("KEY_TAB", KEY_TAB);
    keys[i++] = create_key("KEY_BACKSLASH", KEY_BACKSLASH);
    keys[i++] = create_key("KEY_BACKSPACE", KEY_BACKSPACE);
    keys[i++] = create_key("KEY_DELETE", KEY_DELETE);
    keys[i++] = create_key("KEY_INSERT", KEY_INSERT);
    keys[i++] = create_key("KEY_TILDE", KEY_GRAVE);

    keys[i++] = create_key("KEY_PLUS", KEY_KPPLUS);
    keys[i++] = create_key("KEY_MINUS", KEY_KPMINUS);

    if (i > KEYLIST_SIZE) {
        fprintf(stderr, "Key list size is too large.\r\n");
        exit_with_error(1);
    }
}
