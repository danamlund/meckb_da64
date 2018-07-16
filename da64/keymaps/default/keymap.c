/* Copyright 2017 dan amlund
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#include "tinybasic_qmk.h"
#include "tetris_qmk.h"
#include "minesweeper_qmk.h"

#define PREVENT_STUCK_MODIFIERS

enum my_keycodes {
  HELP = SAFE_RANGE,
  TETRIS,
  BASIC,
  MINES,
  QUICKCALC,
  DYNAMIC_MACRO_RANGE,
};

#define DYNAMIC_MACRO_SIZE 48
#include "dynamic_macro.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Qwerty gui/alt/space/alt/gui
 * ,-----------------------------------------------------------------------------------------.
 * | Esc |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  0  |  -  |  =  |   Bksp    |
 * |-----------------------------------------------------------------------------------------+
 * | Tab    |  Q  |  W  |  E  |  R  |  T  |  Y  |  U  |  I  |  O  |  P  |  [  |  ]  |   \    |
 * |-----------------------------------------------------------------------------------------+
 * | Caps    |  A  |  S  |  D  |  F  |  G  |  H  |  J  |  K  |  L  |  ;  |  '  |    Enter    |
 * |-----------------------------------------------------------------------------------------+
 * | Shift     |  Z  |  X  |  C  |  V  |  B  |  N  |  M  |  ,  |  .  |  /  |        Up |Shift|
 * |-----------------------------------------------------------------------------------------+
 * | Ctrl | Fn | Win | LAlt |             Space                | RAlt | Menu |Left|Down|Right|
 * `-----------------------------------------------------------------------------------------'
 */

[0] = KEYMAP( /* Base */
  KC_ESC,  KC_1,  KC_2,    KC_3,    KC_4, KC_5,     KC_6, KC_7, KC_8,    KC_9,   KC_0,    KC_MINS, KC_EQL,  KC_BSPC, \
  KC_TAB,  KC_Q,  KC_W,    KC_E,    KC_R, KC_T,     KC_Y, KC_U, KC_I,    KC_O,   KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, \
  KC_CAPS, KC_A,  KC_S,    KC_D,    KC_F, KC_G,     KC_H, KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,          KC_ENT, \
  KC_LSFT, KC_Z,  KC_X,    KC_C,    KC_V, KC_B,     KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH,          KC_UP,   KC_RSFT, \
  KC_LCTL, MO(1), KC_LGUI, KC_LALT,       KC_SPACE,             KC_RALT, KC_APP,          KC_LEFT, KC_DOWN, KC_RGHT \
),
[1] = KEYMAP(
  KC_GRV,  KC_F1,     KC_F2,          KC_F3,        KC_F4,           KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL, \
  XXXXXXX, QUICKCALC, DYN_REC_START1, DYN_REC_STOP, DYN_MACRO_PLAY1, TETRIS,  XXXXXXX, HELP   , KC_UP,   KC_INS,  KC_PSCR, KC_LSCR, KC_PAUS, XXXXXXX, \
  XXXXXXX, XXXXXXX,   XXXXXXX,        XXXXXXX,      XXXXXXX,         XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX,          XXXXXXX, \
  KC_LSFT, MO(2),     XXXXXXX,        XXXXXXX,      XXXXXXX,         BASIC,   XXXXXXX, MINES,   XXXXXXX, XXXXXXX, XXXXXXX,          KC_PGUP, KC_RSFT, \
  KC_LCTL, KC_TRNS,   KC_LGUI,        KC_LALT,              KC_SPACE,                           KC_RALT, KC_APP,           KC_HOME, KC_PGDN, KC_END \
),
[2] = KEYMAP(
  KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TETRIS,  XXXXXXX, XXXXXXX, KC_PGUP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME, KC_PGDN, KC_END,  XXXXXXX, XXXXXXX,          XXXXXXX, \
  KC_LSFT, KC_TRNS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,          KC_PGUP, KC_RSFT, \
  KC_LCTL, KC_TRNS, KC_LGUI, KC_LALT,        KC_SPACE,                    KC_RALT, KC_APP,           KC_HOME, KC_PGDN, KC_END \
)
};

const uint16_t PROGMEM fn_actions[] = {
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_dynamic_macro(keycode, record)) {
        return false;
    }

    if (record->event.pressed) {
        if (keycode == TETRIS && !basic_is_running()) {
            tetris_qmk_start();
            layer_clear();
        }
        if (keycode == HELP) {
            SEND_STRING("http://github.com/danamlund/meckb_da64");
        }
        if (keycode == BASIC && !basic_is_running()) {
            basic_start();
        }
        if (keycode == MINES && !basic_is_running()) {
            minesweeper_start();
            layer_clear();
        }
        if (keycode == QUICKCALC && !basic_is_running()) {
            basic_quickcalc();
        }
    }
    if (!tetris_process_record_user(keycode, record)) {
        return false;
    }
    if (!minesweeper_process_record_user(keycode, record)) {
        return false;
    }
    return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
}

/* Called when sending a keycode press. Do not send code if false. */
bool register_code_user(uint8_t code) {
    if (!basic_register_code_user(code)) {
        return false;
    }
    return true;
}

/* Called when sending a keycode release. Do not send code if false. */
bool unregister_code_user(uint8_t code) {
    return true;
}

// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
    da_util_matrix_scan_user();
    tetris_matrix_scan_user();
    basic_matrix_scan_user();
}

bool is_layer_on(uint32_t state, uint8_t layer) {
    return state & (1UL << layer);
}
uint32_t without_layer(uint32_t state, uint8_t layer) {
    return state & ~(1UL << layer);
}

uint32_t layer_state_set_user(uint32_t state) {
    // dont get stuck on layers depending on two held keys
    if (!is_layer_on(state, 1)) {
        state = without_layer(state, 2);
    }
    return state;
}
