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
#include "da64.h"
#include "tetris_text.h"

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
  KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4, KC_F5,    KC_F6, KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,    KC_DEL, \
  KC_EQL,  KC_Q,    KC_W,    KC_E,    KC_R,  F(0),     KC_Y,  KC_U,    KC_UP,   KC_O,    KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS, \
  KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,  KC_G,     KC_H,  KC_LEFT, KC_DOWN, KC_RGHT, KC_SCLN, KC_QUOT,            KC_ENT, \
  KC_LSFT, MO(2),   KC_X,    KC_C,    KC_V,  KC_B,     KC_N,  KC_M,    KC_COMM, KC_DOT,  KC_SLSH,          KC_PGUP,   KC_RSFT, \
  KC_LCTL, KC_TRNS, KC_LGUI, KC_LALT,        KC_SPACE,                 KC_RALT, KC_APP,           KC_HOME, KC_PGDOWN, KC_END \
),
[2] = KEYMAP(
  KC_GRV,  KC_F1,   KC_F2,   KC_F3,   KC_F4, KC_F5,    KC_F6, KC_F7,   KC_F8,     KC_F9,  KC_F10,  KC_F11,  KC_F12,    KC_DEL, \
  KC_EQL,  KC_Q,    KC_W,    KC_E,    KC_R,  KC_T,     KC_Y,  KC_U,    KC_PGUP,   KC_O,   KC_P,    KC_LBRC, KC_RBRC,   KC_BSLS, \
  KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,  KC_G,     KC_H,  KC_HOME, KC_PGDOWN, KC_END, KC_SCLN, KC_QUOT,            KC_ENT, \
  KC_LSFT, KC_TRNS, KC_X,    KC_C,    KC_V,  KC_B,     KC_N,  KC_M,    KC_COMM,   KC_DOT, KC_SLSH,          KC_PGUP,   KC_RSFT, \
  KC_LCTL, KC_TRNS, KC_LGUI, KC_LALT,        KC_SPACE,                 KC_RALT,   KC_APP,          KC_HOME, KC_PGDOWN, KC_END \
)
};

const uint16_t PROGMEM fn_actions[] = {
  [0] = ACTION_FUNCTION(0),  // Calls action_function()
};

static uint8_t key_presses = 0;
static uint16_t timer = 0;

static uint8_t tetris_running = 0;
static int tetris_keypress = 0;

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt) {
  if (id == 0 && record->event.pressed) {
    tetris_running = 1;
    tetris_keypress = 0;
    timer = 0;
    tetris_start(key_presses);
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  key_presses++;

  if (tetris_running) {
    if (keycode == KC_ESC && record->event.pressed) {
      tetris_running = 0;
      return false;
    }

    if (record->event.pressed) {
      tetris_keypress = 0;
      switch (keycode) {
      case KC_UP: tetris_keypress = 1; break;
      case KC_LEFT: tetris_keypress = 2; break;
      case KC_DOWN: tetris_keypress = 3; break;
      case KC_RIGHT: tetris_keypress = 4; break;
      }
      if (tetris_keypress != 0) {
        return false;
      }
    }
  }

  return true;
}

// Runs just one time when the keyboard initializes.
void matrix_init_user(void) {
}


// Runs constantly in the background, in a loop.
void matrix_scan_user(void) {
  if (tetris_running) {
    timer++;
    if (timer > 100) {
      if (!tetris_tick(50)) {
        // game over
        tetris_running = 0;
      }
      timer = 0;
    }
  }      
}

void send_keycode(uint16_t keycode) {
  register_code(keycode);
  unregister_code(keycode);
}

void send_keycode_shift(uint16_t keycode) {
  register_code(KC_LSFT);
  register_code(keycode);
  unregister_code(keycode);
  unregister_code(KC_LSFT);
}

void tetris_send_up(void) {
  send_keycode(KC_UP);
}
void tetris_send_left(void) {
  send_keycode(KC_LEFT);
}
void tetris_send_down(void) {
  send_keycode(KC_DOWN);
}
void tetris_send_right(void) {
  send_keycode(KC_RGHT);
}

void tetris_send_home(void) {
  send_keycode(KC_HOME);
}
void tetris_send_end(void) {
  send_keycode(KC_END);
}

void tetris_send_backspace(void) {
  send_keycode(KC_BSPC);
}
void tetris_send_delete(void) {
  send_keycode(KC_DEL);
}

void tetris_send_string(const char *s) {
  for (int i = 0; s[i] != 0; i++) {
    if (s[i] >= 'a' && s[i] <= 'z') {
      send_keycode(KC_A + (s[i] - 'a'));
    } else if (s[i] >= 'A' && s[i] <= 'Z') {
      send_keycode_shift(KC_A + (s[i] - 'A'));
    } else if (s[i] >= '1' && s[i] <= '9') {
      send_keycode(KC_1 + (s[i] - '1'));
    } else {
      switch (s[i]) {
      case ' ': send_keycode(KC_SPACE); break;
      case '.': send_keycode(KC_DOT); break;
      case '0': send_keycode(KC_0); break;
      }
    }
  }
}

void tetris_send_newline(void) {
  send_keycode(KC_ENT);
}

int tetris_get_keypress(void) {
  int out = tetris_keypress;
  tetris_keypress = 0;
  return out;
}
