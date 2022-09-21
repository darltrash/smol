#ifdef SOKOL_GLCORE33
#define SMOL_BACKEND "GL 3.3"
#elif SOKOL_GLES3
#define SMOL_BACKEND "GLES 3"
#elif SOKOL_GLES2
#define SMOL_BACKEND "GLES 2"
#elif SOKOL_D3D11
#define SMOL_BACKEND "DirectX11"
#elif SOKOL_METAL
#define SMOL_BACKEND "Metal"
#else
#define SMOL_BACKEND "Unknown"
#endif

#ifndef SMOL_OS
#define SMOL_OS "Unknown"
#endif

#define SMOL_QUAD_AMOUNT 2048
#define SMOL_METATEXTURE_WIDTH 1024
#define SMOL_METATEXTURE_HEIGHT 1024

#define SMOL_TEXTURE_WIDTH  1024
#define SMOL_TEXTURE_HEIGHT 1024


#include "vec.h"

typedef struct
{
    size_t x, y, w, h;
} sm_piece;

typedef struct
{
    unsigned int x, y;
    bool buttons[3];
    float scroll[2];
} sm_mouse_state;

typedef struct
{
    size_t w, h;
    uint32_t *data;
} sm_raw_texture;

const char *sm_keymap[SAPP_KEYCODE_MENU + 1] = {
    [SAPP_KEYCODE_INVALID]       = "invalid",
    [SAPP_KEYCODE_SPACE]         = "space",
    [SAPP_KEYCODE_APOSTROPHE]    = "apostrophe",
    [SAPP_KEYCODE_COMMA]         = "comma",
    [SAPP_KEYCODE_MINUS]         = "minus",
    [SAPP_KEYCODE_PERIOD]        = "period",
    [SAPP_KEYCODE_SLASH]         = "slash",
    [SAPP_KEYCODE_0]             = "0",
    [SAPP_KEYCODE_1]             = "1",
    [SAPP_KEYCODE_2]             = "2",
    [SAPP_KEYCODE_3]             = "3",
    [SAPP_KEYCODE_4]             = "4",
    [SAPP_KEYCODE_5]             = "5",
    [SAPP_KEYCODE_6]             = "6",
    [SAPP_KEYCODE_7]             = "7",
    [SAPP_KEYCODE_8]             = "8",
    [SAPP_KEYCODE_9]             = "9",
    [SAPP_KEYCODE_SEMICOLON]     = "semicolon",
    [SAPP_KEYCODE_EQUAL]         = "equal",
    [SAPP_KEYCODE_A]             = "a",
    [SAPP_KEYCODE_B]             = "b",
    [SAPP_KEYCODE_C]             = "c",
    [SAPP_KEYCODE_D]             = "d",
    [SAPP_KEYCODE_E]             = "e",
    [SAPP_KEYCODE_F]             = "f",
    [SAPP_KEYCODE_G]             = "g",
    [SAPP_KEYCODE_H]             = "h",
    [SAPP_KEYCODE_I]             = "i",
    [SAPP_KEYCODE_J]             = "j",
    [SAPP_KEYCODE_K]             = "k",
    [SAPP_KEYCODE_L]             = "l",
    [SAPP_KEYCODE_M]             = "m",
    [SAPP_KEYCODE_N]             = "n",
    [SAPP_KEYCODE_O]             = "o",
    [SAPP_KEYCODE_P]             = "p",
    [SAPP_KEYCODE_Q]             = "q",
    [SAPP_KEYCODE_R]             = "r",
    [SAPP_KEYCODE_S]             = "s",
    [SAPP_KEYCODE_T]             = "t",
    [SAPP_KEYCODE_U]             = "u",
    [SAPP_KEYCODE_V]             = "v",
    [SAPP_KEYCODE_W]             = "w",
    [SAPP_KEYCODE_X]             = "x",
    [SAPP_KEYCODE_Y]             = "y",
    [SAPP_KEYCODE_Z]             = "z",
    [SAPP_KEYCODE_BACKSLASH]     = "backslash",
    [SAPP_KEYCODE_GRAVE_ACCENT]  = "grave_accent",
    [SAPP_KEYCODE_WORLD_1]       = "world_1",
    [SAPP_KEYCODE_WORLD_2]       = "world_2",
    [SAPP_KEYCODE_ESCAPE]        = "escape",
    [SAPP_KEYCODE_ENTER]         = "enter",
    [SAPP_KEYCODE_TAB]           = "tab",
    [SAPP_KEYCODE_BACKSPACE]     = "backspace",
    [SAPP_KEYCODE_INSERT]        = "insert",
    [SAPP_KEYCODE_DELETE]        = "delete",
    [SAPP_KEYCODE_RIGHT]         = "right",
    [SAPP_KEYCODE_LEFT]          = "left",
    [SAPP_KEYCODE_DOWN]          = "down",
    [SAPP_KEYCODE_UP]            = "up",
    [SAPP_KEYCODE_PAGE_UP]       = "page_up",
    [SAPP_KEYCODE_PAGE_DOWN]     = "page_down",
    [SAPP_KEYCODE_HOME]          = "home",
    [SAPP_KEYCODE_END]           = "end",
    [SAPP_KEYCODE_CAPS_LOCK]     = "caps_lock",
    [SAPP_KEYCODE_SCROLL_LOCK]   = "scroll_lock",
    [SAPP_KEYCODE_NUM_LOCK]      = "num_lock",
    [SAPP_KEYCODE_PRINT_SCREEN]  = "print_screen",
    [SAPP_KEYCODE_PAUSE]         = "pause",
    [SAPP_KEYCODE_F1]            = "f1",
    [SAPP_KEYCODE_F2]            = "f2",
    [SAPP_KEYCODE_F3]            = "f3",
    [SAPP_KEYCODE_F4]            = "f4",
    [SAPP_KEYCODE_F5]            = "f5",
    [SAPP_KEYCODE_F6]            = "f6",
    [SAPP_KEYCODE_F7]            = "f7",
    [SAPP_KEYCODE_F8]            = "f8",
    [SAPP_KEYCODE_F9]            = "f9",
    [SAPP_KEYCODE_F10]           = "f10",
    [SAPP_KEYCODE_F11]           = "f11",
    [SAPP_KEYCODE_F12]           = "f12",
    [SAPP_KEYCODE_F13]           = "f13",
    [SAPP_KEYCODE_F14]           = "f14",
    [SAPP_KEYCODE_F15]           = "f15",
    [SAPP_KEYCODE_F16]           = "f16",
    [SAPP_KEYCODE_F17]           = "f17",
    [SAPP_KEYCODE_F18]           = "f18",
    [SAPP_KEYCODE_F19]           = "f19",
    [SAPP_KEYCODE_F20]           = "f20",
    [SAPP_KEYCODE_F21]           = "f21",
    [SAPP_KEYCODE_F22]           = "f22",
    [SAPP_KEYCODE_F23]           = "f23",
    [SAPP_KEYCODE_F24]           = "f24",
    [SAPP_KEYCODE_F25]           = "f25",
    [SAPP_KEYCODE_KP_0]          = "kp_0",
    [SAPP_KEYCODE_KP_1]          = "kp_1",
    [SAPP_KEYCODE_KP_2]          = "kp_2",
    [SAPP_KEYCODE_KP_3]          = "kp_3",
    [SAPP_KEYCODE_KP_4]          = "kp_4",
    [SAPP_KEYCODE_KP_5]          = "kp_5",
    [SAPP_KEYCODE_KP_6]          = "kp_6",
    [SAPP_KEYCODE_KP_7]          = "kp_7",
    [SAPP_KEYCODE_KP_8]          = "kp_8",
    [SAPP_KEYCODE_KP_9]          = "kp_9",
    [SAPP_KEYCODE_KP_DECIMAL]    = "kp_decimal",
    [SAPP_KEYCODE_KP_DIVIDE]     = "kp_divide",
    [SAPP_KEYCODE_KP_MULTIPLY]   = "kp_multiply",
    [SAPP_KEYCODE_KP_SUBTRACT]   = "kp_subtract",
    [SAPP_KEYCODE_KP_ADD]        = "kp_add",
    [SAPP_KEYCODE_KP_ENTER]      = "kp_enter",
    [SAPP_KEYCODE_KP_EQUAL]      = "kp_equal",
    [SAPP_KEYCODE_LEFT_BRACKET]  = "left_bracket",
    [SAPP_KEYCODE_RIGHT_BRACKET] = "right_bracket",
    [SAPP_KEYCODE_LEFT_SHIFT]    = "left_shift",
    [SAPP_KEYCODE_RIGHT_SHIFT]   = "right_shift",
    [SAPP_KEYCODE_LEFT_CONTROL]  = "left_control",
    [SAPP_KEYCODE_RIGHT_CONTROL] = "right_control",
    [SAPP_KEYCODE_LEFT_ALT]      = "left_alt",
    [SAPP_KEYCODE_RIGHT_ALT]     = "right_alt",
    [SAPP_KEYCODE_LEFT_SUPER]    = "left_super",
    [SAPP_KEYCODE_RIGHT_SUPER]   = "right_super",
    [SAPP_KEYCODE_MENU]          = "menu"
};