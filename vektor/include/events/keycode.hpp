#pragma once

// Printable keys
#define VEKTOR_KEY_SPACE 32
#define VEKTOR_KEY_APOSTROPHE 39 /* ' */
#define VEKTOR_KEY_COMMA 44      /* , */
#define VEKTOR_KEY_MINUS 45      /* - */
#define VEKTOR_KEY_PERIOD 46     /* . */
#define VEKTOR_KEY_SLASH 47      /* / */

#define VEKTOR_KEY_0 48
#define VEKTOR_KEY_1 49
#define VEKTOR_KEY_2 50
#define VEKTOR_KEY_3 51
#define VEKTOR_KEY_4 52
#define VEKTOR_KEY_5 53
#define VEKTOR_KEY_6 54
#define VEKTOR_KEY_7 55
#define VEKTOR_KEY_8 56
#define VEKTOR_KEY_9 57

#define VEKTOR_KEY_SEMICOLON 59 /* ; */
#define VEKTOR_KEY_EQUAL 61     /* = */

#define VEKTOR_KEY_A 65
#define VEKTOR_KEY_B 66
#define VEKTOR_KEY_C 67
#define VEKTOR_KEY_D 68
#define VEKTOR_KEY_E 69
#define VEKTOR_KEY_F 70
#define VEKTOR_KEY_G 71
#define VEKTOR_KEY_H 72
#define VEKTOR_KEY_I 73
#define VEKTOR_KEY_J 74
#define VEKTOR_KEY_K 75
#define VEKTOR_KEY_L 76
#define VEKTOR_KEY_M 77
#define VEKTOR_KEY_N 78
#define VEKTOR_KEY_O 79
#define VEKTOR_KEY_P 80
#define VEKTOR_KEY_Q 81
#define VEKTOR_KEY_R 82
#define VEKTOR_KEY_S 83
#define VEKTOR_KEY_T 84
#define VEKTOR_KEY_U 85
#define VEKTOR_KEY_V 86
#define VEKTOR_KEY_W 87
#define VEKTOR_KEY_X 88
#define VEKTOR_KEY_Y 89
#define VEKTOR_KEY_Z 90

#define VEKTOR_KEY_LEFT_BRACKET 91  /* [ */
#define VEKTOR_KEY_BACKSLASH 92     /* \ */
#define VEKTOR_KEY_RIGHT_BRACKET 93 /* ] */
#define VEKTOR_KEY_GRAVE_ACCENT 96  /* ` */

// Function keys
#define VEKTOR_KEY_ESCAPE 256
#define VEKTOR_KEY_ENTER 257
#define VEKTOR_KEY_TAB 258
#define VEKTOR_KEY_BACKSPACE 259
#define VEKTOR_KEY_INSERT 260
#define VEKTOR_KEY_DELETE 261
#define VEKTOR_KEY_RIGHT 262
#define VEKTOR_KEY_LEFT 263
#define VEKTOR_KEY_DOWN 264
#define VEKTOR_KEY_UP 265
#define VEKTOR_KEY_PAGE_UP 266
#define VEKTOR_KEY_PAGE_DOWN 267
#define VEKTOR_KEY_HOME 268
#define VEKTOR_KEY_END 269
#define VEKTOR_KEY_CAPS_LOCK 280
#define VEKTOR_KEY_SCROLL_LOCK 281
#define VEKTOR_KEY_NUM_LOCK 282
#define VEKTOR_KEY_PRINT_SCREEN 283
#define VEKTOR_KEY_PAUSE 284

#define VEKTOR_KEY_F1 290
#define VEKTOR_KEY_F2 291
#define VEKTOR_KEY_F3 292
#define VEKTOR_KEY_F4 293
#define VEKTOR_KEY_F5 294
#define VEKTOR_KEY_F6 295
#define VEKTOR_KEY_F7 296
#define VEKTOR_KEY_F8 297
#define VEKTOR_KEY_F9 298
#define VEKTOR_KEY_F10 299
#define VEKTOR_KEY_F11 300
#define VEKTOR_KEY_F12 301
#define VEKTOR_KEY_F13 302
#define VEKTOR_KEY_F14 303
#define VEKTOR_KEY_F15 304
#define VEKTOR_KEY_F16 305
#define VEKTOR_KEY_F17 306
#define VEKTOR_KEY_F18 307
#define VEKTOR_KEY_F19 308
#define VEKTOR_KEY_F20 309
#define VEKTOR_KEY_F21 310
#define VEKTOR_KEY_F22 311
#define VEKTOR_KEY_F23 312
#define VEKTOR_KEY_F24 313
#define VEKTOR_KEY_F25 314

// Modifiers
#define VEKTOR_KEY_LEFT_SHIFT 340
#define VEKTOR_KEY_LEFT_CONTROL 341
#define VEKTOR_KEY_LEFT_ALT 342
#define VEKTOR_KEY_LEFT_SUPER 343
#define VEKTOR_KEY_RIGHT_SHIFT 344
#define VEKTOR_KEY_RIGHT_CONTROL 345
#define VEKTOR_KEY_RIGHT_ALT 346
#define VEKTOR_KEY_RIGHT_SUPER 347
#define VEKTOR_KEY_MENU 348

// Limits
#define VEKTOR_KEY_LAST VEKTOR_KEY_MENU




#pragma once
#include <cstdint>
#include <iostream>

namespace vektor
{

    enum class MouseCode : uint16_t
    {
        Button0 = 0,
        Button1 = 1,
        Button2 = 2,
        Button3 = 3,
        Button4 = 4,
        Button5 = 5,
        Button6 = 6,
        Button7 = 7,

        Left = Button0,
        Right = Button1,
        Middle = Button2
    };

    enum class KeyCode : uint16_t
    {
        Space = 32,
        Apostrophe = 39,
        Comma = 44,
        Minus = 45,
        Period = 46,
        Slash = 47,
        D0 = 48,
        D1 = 49,
        D2 = 50,
        D3 = 51,
        D4 = 52,
        D5 = 53,
        D6 = 54,
        D7 = 55,
        D8 = 56,
        D9 = 57,
        Semicolon = 59,
        Equal = 61,
        A = 65,
        B = 66,
        C = 67,
        D = 68,
        E = 69,
        F = 70,
        G = 71,
        H = 72,
        I = 73,
        J = 74,
        K = 75,
        L = 76,
        M = 77,
        N = 78,
        O = 79,
        P = 80,
        Q = 81,
        R = 82,
        S = 83,
        T = 84,
        U = 85,
        V = 86,
        W = 87,
        X = 88,
        Y = 89,
        Z = 90,
        LeftBracket = 91,
        Backslash = 92,
        RightBracket = 93,
        GraveAccent = 96,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,
        CapsLock = 280,
        ScrollLock = 281,
        NumLock = 282,
        PrintScreen = 283,
        Pause = 284,
        F1 = 290,
        F2 = 291,
        F3 = 292,
        F4 = 293,
        F5 = 294,
        F6 = 295,
        F7 = 296,
        F8 = 297,
        F9 = 298,
        F10 = 299,
        F11 = 300,
        F12 = 301,
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        LeftSuper = 343,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,
        RightSuper = 347,
        Menu = 348
    };

    inline std::ostream &operator<<(std::ostream &os, KeyCode key)
    {
        return os << static_cast<uint16_t>(key);
    }
}