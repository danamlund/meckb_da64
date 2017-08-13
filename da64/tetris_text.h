#ifndef TETRIS_TEXT_H
#define TETRIS_TEXT_H

//// to implement
void tetris_send_up(void);
void tetris_send_left(void);
void tetris_send_down(void);
void tetris_send_right(void);

void tetris_send_home(void);
void tetris_send_end(void);

void tetris_send_backspace(void);
void tetris_send_delete(void);

void tetris_send_string(const char *s);

void tetris_send_newline(void);

// return = meaning
// 0 = no keys pressed
// 1 = up
// 2 = left
// 3 = down
// 4 = right
int tetris_get_keypress(void);

//// to call
void tetris_start(uint8_t seed);
// returns 0 when game is over
int tetris_tick(int ms_since_previous_tick);

#endif
