package com.vantacom.aarm.wine.controls.keyboard;

import android.view.KeyEvent;

public class KeyCodesWine {
    private KeyCodes[] keyboardKeyCodes;

    public KeyCodesWine() {
        keyboardKeyCodes = new KeyCodes[KeyEvent.getMaxKeyCode() + 1];
        generateKeyboardKeyCodes();
    }

    private void generateKeyboardKeyCodes()
    {
        this.keyboardKeyCodes[4] = KeyCodes.KEY_ESC;

        this.keyboardKeyCodes[7] = KeyCodes.KEY_0;
        this.keyboardKeyCodes[8] = KeyCodes.KEY_1;
        this.keyboardKeyCodes[9] = KeyCodes.KEY_2;
        this.keyboardKeyCodes[10] = KeyCodes.KEY_3;
        this.keyboardKeyCodes[11] = KeyCodes.KEY_4;
        this.keyboardKeyCodes[12] = KeyCodes.KEY_5;
        this.keyboardKeyCodes[13] = KeyCodes.KEY_6;
        this.keyboardKeyCodes[14] = KeyCodes.KEY_7;
        this.keyboardKeyCodes[15] = KeyCodes.KEY_8;
        this.keyboardKeyCodes[16] = KeyCodes.KEY_9;

        this.keyboardKeyCodes[19] = KeyCodes.KEY_UP;
        this.keyboardKeyCodes[20] = KeyCodes.KEY_DOWN;
        this.keyboardKeyCodes[21] = KeyCodes.KEY_LEFT;
        this.keyboardKeyCodes[22] = KeyCodes.KEY_RIGHT;

        this.keyboardKeyCodes[29] = KeyCodes.KEY_A;
        this.keyboardKeyCodes[30] = KeyCodes.KEY_B;
        this.keyboardKeyCodes[31] = KeyCodes.KEY_C;
        this.keyboardKeyCodes[32] = KeyCodes.KEY_D;
        this.keyboardKeyCodes[33] = KeyCodes.KEY_E;
        this.keyboardKeyCodes[34] = KeyCodes.KEY_F;
        this.keyboardKeyCodes[35] = KeyCodes.KEY_G;
        this.keyboardKeyCodes[36] = KeyCodes.KEY_H;
        this.keyboardKeyCodes[37] = KeyCodes.KEY_I;
        this.keyboardKeyCodes[38] = KeyCodes.KEY_J;
        this.keyboardKeyCodes[39] = KeyCodes.KEY_K;
        this.keyboardKeyCodes[40] = KeyCodes.KEY_L;
        this.keyboardKeyCodes[41] = KeyCodes.KEY_M;
        this.keyboardKeyCodes[42] = KeyCodes.KEY_N;
        this.keyboardKeyCodes[43] = KeyCodes.KEY_O;
        this.keyboardKeyCodes[44] = KeyCodes.KEY_P;
        this.keyboardKeyCodes[45] = KeyCodes.KEY_Q;
        this.keyboardKeyCodes[46] = KeyCodes.KEY_R;
        this.keyboardKeyCodes[47] = KeyCodes.KEY_S;
        this.keyboardKeyCodes[48] = KeyCodes.KEY_T;
        this.keyboardKeyCodes[49] = KeyCodes.KEY_U;
        this.keyboardKeyCodes[50] = KeyCodes.KEY_V;
        this.keyboardKeyCodes[51] = KeyCodes.KEY_W;
        this.keyboardKeyCodes[52] = KeyCodes.KEY_X;
        this.keyboardKeyCodes[53] = KeyCodes.KEY_Y;
        this.keyboardKeyCodes[54] = KeyCodes.KEY_Z;

        this.keyboardKeyCodes[55] = KeyCodes.KEY_COMMA;
        this.keyboardKeyCodes[56] = KeyCodes.KEY_PERIOD;
        this.keyboardKeyCodes[57] = KeyCodes.KEY_ALT_LEFT;
        this.keyboardKeyCodes[58] = KeyCodes.KEY_ALT_RIGHT;
        this.keyboardKeyCodes[59] = KeyCodes.KEY_SHIFT_LEFT;
        this.keyboardKeyCodes[60] = KeyCodes.KEY_SHIFT_RIGHT;
        this.keyboardKeyCodes[61] = KeyCodes.KEY_TAB;
        this.keyboardKeyCodes[62] = KeyCodes.KEY_SPACE;

        this.keyboardKeyCodes[66] = KeyCodes.KEY_RETURN;
        this.keyboardKeyCodes[67] = KeyCodes.KEY_BACKSPACE;
        this.keyboardKeyCodes[68] = KeyCodes.KEY_GRAVE;
        this.keyboardKeyCodes[69] = KeyCodes.KEY_MINUS;
        this.keyboardKeyCodes[70] = KeyCodes.KEY_EQUAL;
        this.keyboardKeyCodes[71] = KeyCodes.KEY_BRACKET_LEFT;
        this.keyboardKeyCodes[72] = KeyCodes.KEY_BRACKET_RIGHT;
        this.keyboardKeyCodes[73] = KeyCodes.KEY_BACKSLASH;
        this.keyboardKeyCodes[74] = KeyCodes.KEY_SEMICOLON;
        this.keyboardKeyCodes[75] = KeyCodes.KEY_APOSTROPHE;
        this.keyboardKeyCodes[76] = KeyCodes.KEY_SLASH;

        this.keyboardKeyCodes[92] = KeyCodes.KEY_PRIOR;
        this.keyboardKeyCodes[93] = KeyCodes.KEY_NEXT;

        this.keyboardKeyCodes[112] = KeyCodes.KEY_DELETE;
        this.keyboardKeyCodes[113] = KeyCodes.KEY_CONTROL_LEFT;
        this.keyboardKeyCodes[114] = KeyCodes.KEY_CONTROL_RIGHT;
        this.keyboardKeyCodes[115] = KeyCodes.KEY_CAPS_LOCK;

        this.keyboardKeyCodes[122] = KeyCodes.KEY_HOME;
        this.keyboardKeyCodes[123] = KeyCodes.KEY_END;
        this.keyboardKeyCodes[124] = KeyCodes.KEY_INSERT;

        this.keyboardKeyCodes[131] = KeyCodes.KEY_F1;
        this.keyboardKeyCodes[132] = KeyCodes.KEY_F2;
        this.keyboardKeyCodes[133] = KeyCodes.KEY_F3;
        this.keyboardKeyCodes[134] = KeyCodes.KEY_F4;
        this.keyboardKeyCodes[135] = KeyCodes.KEY_F5;
        this.keyboardKeyCodes[136] = KeyCodes.KEY_F6;
        this.keyboardKeyCodes[137] = KeyCodes.KEY_F7;
        this.keyboardKeyCodes[138] = KeyCodes.KEY_F8;
        this.keyboardKeyCodes[139] = KeyCodes.KEY_F9;
        this.keyboardKeyCodes[140] = KeyCodes.KEY_F10;
        this.keyboardKeyCodes[141] = KeyCodes.KEY_F11;
        this.keyboardKeyCodes[142] = KeyCodes.KEY_F12;
        this.keyboardKeyCodes[143] = KeyCodes.KEY_NUM_LOCK;
        this.keyboardKeyCodes[144] = KeyCodes.KEY_KP_0;
        this.keyboardKeyCodes[145] = KeyCodes.KEY_KP_1;
        this.keyboardKeyCodes[146] = KeyCodes.KEY_KP_2;
        this.keyboardKeyCodes[147] = KeyCodes.KEY_KP_3;
        this.keyboardKeyCodes[148] = KeyCodes.KEY_KP_4;
        this.keyboardKeyCodes[149] = KeyCodes.KEY_KP_5;
        this.keyboardKeyCodes[150] = KeyCodes.KEY_KP_6;
        this.keyboardKeyCodes[151] = KeyCodes.KEY_KP_7;
        this.keyboardKeyCodes[152] = KeyCodes.KEY_KP_8;
        this.keyboardKeyCodes[153] = KeyCodes.KEY_KP_9;
        this.keyboardKeyCodes[154] = KeyCodes.KEY_KP_DIV;
        this.keyboardKeyCodes[155] = KeyCodes.KEY_KP_MULTIPLY;
        this.keyboardKeyCodes[156] = KeyCodes.KEY_KP_SUB;
        this.keyboardKeyCodes[157] = KeyCodes.KEY_KP_ADD;
        this.keyboardKeyCodes[158] = KeyCodes.KEY_KP_DEL;
    }
}
