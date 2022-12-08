#include QMK_KEYBOARD_H

enum tap_dance_codes {
  DOT_EXC,
  COMM_MIN,
  SLS_UNDS,
  DOL_SCSH,
  ESC_QUIT,
  TAB_SEL,
  GRV_SEL,
  GRV_TIL,
  LFT_ALT,
  RGT_ALT,
  BKSL_HME,
  PIPE_END,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(
  //------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
      KC_Q,   KC_W,     KC_E,                 KC_R,                  KC_T,    		       KC_Y,   KC_U,                KC_I,               KC_O,          KC_P,
  //------------------------------------------------------------------------------       ----------------------------------------------------------------------------
      KC_A,   KC_S,     MT(MOD_LALT,KC_D),    MT(MOD_LGUI,KC_F),     KC_G,  	           KC_H,   MT(MOD_RGUI,KC_J),   MT(MOD_RALT,KC_K),  KC_L,          KC_SCLN,
  //------------------------------------------------------------------------------       ----------------------------------------------------------------------------
      KC_Z,   KC_X,     KC_C,                 KC_V,                  KC_B,     	           KC_N,   KC_M,                TD(COMM_MIN),       TD(DOT_EXC),   TD(SLS_UNDS),
  //------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
		    					              OSM(MOD_RCTL),  OSM(MOD_LSFT),        MT(MOD_MEH,KC_SPACE), TO(1)
  ),

  [1] = LAYOUT(
//------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
KC_TAB,        KC_AT,    KC_HASH,         TD(DOL_SCSH),          KC_PERC,              KC_CIRC,       KC_AMPR,        KC_ASTR,        KC_COLON,      KC_BSPC,
//------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
KC_ESCAPE,     KC_EQL,   KC_UNDS,         KC_MINS,               KC_QUOT,              TD(BKSL_HME),  LSFT(KC_LBRC),  LSFT(KC_RBRC),  TD(PIPE_END),  KC_ENTER,
//------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
TD(GRV_TIL),   KC_PLUS,  KC_LT,           KC_GT,                 KC_DQUO,              KC_LBRC,       KC_LPRN,        KC_RPRN,        KC_RBRC,       TO(3)   ,
//------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
                                                        TO(0),     KC_LGUI,          KC_RALT      ,TO(2)
),

[2] = LAYOUT(
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
KC_TAB,     KC_MEDIA_PLAY_PAUSE,  KC_MEDIA_PREV_TRACK,  KC_MEDIA_NEXT_TRACK, LSFT(KC_MINS),        KC_MINS,   KC_7,  KC_8, KC_9,   KC_BSPC,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
KC_ESCAPE,  KC_MUTE,              KC_AUDIO_VOL_DOWN,    KC_AUDIO_VOL_UP,     LSFT(KC_EQL),         KC_EQL,    KC_4,  KC_5, KC_6,   KC_ENTER,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
TD(GRV_TIL),KC_KP_ASTERISK,       KC_BRIGHTNESS_DOWN,   KC_BRIGHTNESS_UP,    KC_KP_DOT,            KC_0,      KC_1,  KC_2, KC_3,   KC_COMM,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
                                                                          TO(0), KC_LGUI,     KC_RALT ,TO(2)
),

[3] = LAYOUT(
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
  TD(TAB_SEL),  KC_MS_WH_LEFT,  KC_MS_UP,       KC_MS_WH_RIGHT, KC_Q,                         LGUI(KC_LBRC),  LCTL(LSFT(KC_TAB)),   RCTL(KC_TAB),       LGUI(KC_RBRC), KC_BSPC,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
  TD(ESC_QUIT),   KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    MT(MOD_LGUI,KC_DEL),          TD(LFT_ALT),    MT(MOD_RGUI,KC_DOWN), MT(MOD_RALT,KC_UP), TD(RGT_ALT),   KC_ENTER,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
  TD(GRV_SEL),   KC_MS_BTN2,     KC_MS_WH_UP,    KC_MS_WH_DOWN,  KC_TRNS,                      KC_MS_BTN1,     KC_MS_ACCEL0,         KC_MS_ACCEL1,       KC_MS_ACCEL2,  TO(4),
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
                                                                          TO(0),KC_LGUI,       KC_RALT, TO(1)
),

[4] = LAYOUT(
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
  RGB_TOG,      RGB_MOD,    RGB_HUI,    RGB_SAI,    RGB_VAI,                RGB_SPI,    RGB_M_P,    RGB_M_X,    RGB_M_K,    RGB_M_B,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
  KC_TRNS,      RGB_RMOD,   RGB_HUD,    RGB_SAD,    RGB_VAD,                RGB_SPD,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
  KC_TRNS,      KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,                KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
//---------------------------------------------------------------------------------------       ----------------------------------------------------------------------------------
                                                TO(0),KC_TRNS,                         KC_TRNS, TO(1)
)
};





extern bool g_suspend_state;
#define GET_TAP_KC(dual_role_key) dual_role_key & 0xFF
uint16_t last_keycode = KC_NO;
uint8_t last_modifier = 0;

typedef struct {
bool is_press_action;
uint8_t step;
} tap;

enum {
SINGLE_TAP = 1,
SINGLE_HOLD,
DOUBLE_TAP,
DOUBLE_HOLD,
DOUBLE_SINGLE_TAP,
MORE_TAPS
};

static tap dance_state = {
.is_press_action = true,
.step = 0
};

uint8_t dance_step(qk_tap_dance_state_t *state);
uint8_t dance_step(qk_tap_dance_state_t *state) {
if (state->count == 1) {
    if (state->interrupted || !state->pressed) return SINGLE_TAP;
    else return SINGLE_HOLD;
} else if (state->count == 2) {
    if (state->interrupted) return DOUBLE_SINGLE_TAP;
    else if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
}
return MORE_TAPS;
}

void on_dance_2(qk_tap_dance_state_t *state, void *user_data);
void dance_2_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_2_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_2(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_COMM);
    tap_code16(KC_COMM);
    tap_code16(KC_COMM);
}
if(state->count > 3) {
    tap_code16(KC_COMM);
}
}

void dance_2_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_COMM); break;
    case SINGLE_HOLD: register_code16(KC_MINS); break;
    case DOUBLE_TAP: register_code16(KC_COMM); register_code16(KC_COMM); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_COMM); register_code16(KC_COMM);
}
}

void dance_2_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_COMM); break;
    case SINGLE_HOLD: unregister_code16(KC_MINS); break;
    case DOUBLE_TAP: unregister_code16(KC_COMM); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_MINS); break;
}
dance_state.step = 0;
}
void on_dance_3(qk_tap_dance_state_t *state, void *user_data);
void dance_3_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_3_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_3(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_SLASH);
    tap_code16(KC_SLASH);
    tap_code16(KC_SLASH);
}
if(state->count > 3) {
    tap_code16(KC_SLASH);
}
}

void dance_3_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_SLASH); break;
    case SINGLE_HOLD: register_code16(KC_UNDS); break;
    case DOUBLE_TAP: register_code16(KC_SLASH); register_code16(KC_SLASH); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_SLASH); register_code16(KC_SLASH); break;
}
}

void dance_3_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_SLASH); break;
    case SINGLE_HOLD: unregister_code16(KC_UNDS); break;
    case DOUBLE_TAP: unregister_code16(KC_SLASH); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_SLASH); break;
}
dance_state.step = 0;
}

void on_dance_1(qk_tap_dance_state_t *state, void *user_data);
void dance_1_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_1_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_1(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_DOT);
    tap_code16(KC_DOT);
    tap_code16(KC_DOT);
}
if(state->count > 3) {
    tap_code16(KC_DOT);
}
}

void dance_1_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_DOT); break;
    case SINGLE_HOLD: register_code16(KC_EXLM); break;
    case DOUBLE_TAP: register_code16(KC_DOT); register_code16(KC_DOT); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_DOT); register_code16(KC_DOT);
}
}

void dance_1_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_DOT); break;
    case SINGLE_HOLD: unregister_code16(KC_EXLM); break;
    case DOUBLE_TAP: unregister_code16(KC_DOT); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_DOT); break;
}
dance_state.step = 0;
}
void on_dance_31(qk_tap_dance_state_t *state, void *user_data);
void dance_31_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_31_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_31(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(LSFT(KC_4));
    tap_code16(LSFT(KC_4));
    tap_code16(LSFT(KC_4));
}
if(state->count > 3) {
    tap_code16(LSFT(KC_4));
}
}

void dance_31_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(LSFT(KC_4)); break;
    case SINGLE_HOLD: register_code16(LCTL(LGUI(LSFT(KC_4)))); break;
    case DOUBLE_TAP: register_code16(LSFT(KC_4)); register_code16(LSFT(KC_4)); break;
    case DOUBLE_SINGLE_TAP: tap_code16(LSFT(KC_4)); register_code16(LSFT(KC_4));
}
}

void dance_31_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(LSFT(KC_4)); break;
    case SINGLE_HOLD: unregister_code16(LCTL(LGUI(LSFT(KC_4)))); break;
    case DOUBLE_TAP: unregister_code16(LSFT(KC_4)); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(LSFT(KC_4)); break;
}
dance_state.step = 0;
}

void on_dance_54(qk_tap_dance_state_t *state, void *user_data);
void dance_54_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_54_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_54(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_ESCAPE);
    tap_code16(KC_ESCAPE);
    tap_code16(KC_ESCAPE);
}
if(state->count > 3) {
    tap_code16(KC_ESCAPE);
}
}

void dance_54_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_ESCAPE); break;
    case SINGLE_HOLD: register_code16(LALT(LGUI(KC_ESCAPE))); break;
    case DOUBLE_TAP: register_code16(KC_ESCAPE); register_code16(KC_ESCAPE); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_ESCAPE); register_code16(KC_ESCAPE);
}
}

void dance_54_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_ESCAPE); break;
    case SINGLE_HOLD: unregister_code16(LALT(LGUI(KC_ESCAPE))); break;
    case DOUBLE_TAP: unregister_code16(KC_ESCAPE); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_ESCAPE); break;
}
dance_state.step = 0;
}
void on_dance_56(qk_tap_dance_state_t *state, void *user_data);
void dance_56_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_56_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_56(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_TAB);
    tap_code16(KC_TAB);
    tap_code16(KC_TAB);
}
if(state->count > 3) {
    tap_code16(KC_TAB);
}
}

void dance_56_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_TAB); break;
    case SINGLE_HOLD: register_code16(LGUI(KC_TAB)); break;
    case DOUBLE_TAP: register_code16(KC_TAB); register_code16(KC_TAB); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_TAB); register_code16(KC_TAB);
}
}

void dance_56_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_TAB); break;
    case SINGLE_HOLD: unregister_code16(LGUI(KC_TAB)); break;
    case DOUBLE_TAP: unregister_code16(KC_TAB); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_TAB); break;
}
dance_state.step = 0;
}
void on_dance_58(qk_tap_dance_state_t *state, void *user_data);
void dance_58_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_58_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_58(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_GRV);
    tap_code16(KC_GRV);
    tap_code16(KC_GRV);
}
if(state->count > 3) {
    tap_code16(KC_GRV);
}
}

void dance_58_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_GRV); break;
    case SINGLE_HOLD: register_code16(LGUI(KC_GRV)); break;
    case DOUBLE_TAP: register_code16(KC_GRV); register_code16(KC_GRV); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_GRV); register_code16(KC_GRV);
}
}

void dance_58_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_GRV); break;
    case SINGLE_HOLD: unregister_code16(LGUI(KC_GRV)); break;
    case DOUBLE_TAP: unregister_code16(KC_GRV); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_GRV); break;
}
dance_state.step = 0;
}

void on_dance_59(qk_tap_dance_state_t *state, void *user_data);
void dance_59_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_59_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_59(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_GRV);
    tap_code16(KC_GRV);
    tap_code16(KC_GRV);
}
if(state->count > 3) {
    tap_code16(KC_GRV);
}
}

void dance_59_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_GRV); break;
    case SINGLE_HOLD: register_code16(KC_TILD); break;
    case DOUBLE_TAP: register_code16(KC_GRV); register_code16(KC_GRV); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_GRV); register_code16(KC_GRV);
}
}

void dance_59_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_GRV); break;
    case SINGLE_HOLD: unregister_code16(KC_TILD); break;
    case DOUBLE_TAP: unregister_code16(KC_GRV); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_GRV); break;
}
dance_state.step = 0;
}

void on_dance_60(qk_tap_dance_state_t *state, void *user_data);
void dance_60_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_60_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_60(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_LEFT);
    tap_code16(KC_LEFT);
    tap_code16(KC_LEFT);
}
if(state->count > 3) {
    tap_code16(KC_LEFT);
}
}

void dance_60_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_LEFT); break;
    case SINGLE_HOLD: register_code16(LALT(KC_LEFT)); break;
    case DOUBLE_TAP: register_code16(KC_LEFT); register_code16(KC_LEFT); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_LEFT); register_code16(KC_LEFT);
}
}

void dance_60_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_LEFT); break;
    case SINGLE_HOLD: unregister_code16(LGUI(KC_LEFT)); break;
    case DOUBLE_TAP: unregister_code16(KC_LEFT); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_LEFT); break;
}
dance_state.step = 0;
}
void on_dance_63(qk_tap_dance_state_t *state, void *user_data);
void dance_63_finished(qk_tap_dance_state_t *state, void *user_data);
void dance_63_reset(qk_tap_dance_state_t *state, void *user_data);

void on_dance_63(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_RIGHT);
    tap_code16(KC_RIGHT);
    tap_code16(KC_RIGHT);
}
if(state->count > 3) {
    tap_code16(KC_RIGHT);
}
}

void dance_63_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_RIGHT); break;
    case SINGLE_HOLD: register_code16(LGUI(KC_RIGHT)); break;
    case DOUBLE_TAP: register_code16(KC_RIGHT); register_code16(KC_RIGHT); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_RIGHT); register_code16(KC_RIGHT);
}
}

void dance_63_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_RIGHT); break;
    case SINGLE_HOLD: unregister_code16(LALT(KC_RIGHT)); break;
    case DOUBLE_TAP: unregister_code16(KC_RIGHT); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_RIGHT); break;
}
dance_state.step = 0;
}

void on_bksl_hme(qk_tap_dance_state_t *state, void *user_data);
void bksl_hme_finished(qk_tap_dance_state_t *state, void *user_data);
void bksl_hme_reset(qk_tap_dance_state_t *state, void *user_data);

void on_bksl_hme(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_BSLS);
    tap_code16(KC_BSLS);
    tap_code16(KC_BSLS);
}
if(state->count > 3) {
    tap_code16(KC_BSLS);
}
}

void bksl_hme_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_BSLS); break;
    case SINGLE_HOLD: register_code16(KC_HOME); break;
    case DOUBLE_TAP: register_code16(KC_BSLS); register_code16(KC_BSLS); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_BSLS); register_code16(KC_BSLS);
}
}

void bksl_hme_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_BSLS); break;
    case SINGLE_HOLD: unregister_code16(KC_HOME); break;
    case DOUBLE_TAP: unregister_code16(KC_BSLS); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_BSLS); break;
}
dance_state.step = 0;
}

void on_pipe_end(qk_tap_dance_state_t *state, void *user_data);
void pipe_end_finished(qk_tap_dance_state_t *state, void *user_data);
void pipe_end_reset(qk_tap_dance_state_t *state, void *user_data);

void on_pipe_end(qk_tap_dance_state_t *state, void *user_data) {
if(state->count == 3) {
    tap_code16(KC_PIPE);
    tap_code16(KC_PIPE);
    tap_code16(KC_PIPE);
}
if(state->count > 3) {
    tap_code16(KC_PIPE);
}
}

void pipe_end_finished(qk_tap_dance_state_t *state, void *user_data) {
dance_state.step = dance_step(state);
switch (dance_state.step) {
    case SINGLE_TAP: register_code16(KC_PIPE); break;
    case SINGLE_HOLD: register_code16(KC_END); break;
    case DOUBLE_TAP: register_code16(KC_PIPE); register_code16(KC_PIPE); break;
    case DOUBLE_SINGLE_TAP: tap_code16(KC_PIPE); register_code16(KC_PIPE);
}
}

void pipe_end_reset(qk_tap_dance_state_t *state, void *user_data) {
wait_ms(10);
switch (dance_state.step) {
    case SINGLE_TAP: unregister_code16(KC_PIPE); break;
    case SINGLE_HOLD: unregister_code16(KC_END); break;
    case DOUBLE_TAP: unregister_code16(KC_PIPE); break;
    case DOUBLE_SINGLE_TAP: unregister_code16(KC_PIPE); break;
}
dance_state.step = 0;
}
qk_tap_dance_action_t tap_dance_actions[] = {
    [DOT_EXC]  = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_1, dance_1_finished, dance_1_reset),
    [COMM_MIN] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_2, dance_2_finished, dance_2_reset),
    [SLS_UNDS]  = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_3, dance_3_finished, dance_3_reset),
    [DOL_SCSH] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_31, dance_31_finished, dance_31_reset),
    [ESC_QUIT] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_54, dance_54_finished, dance_54_reset),
    [TAB_SEL] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_56, dance_56_finished, dance_56_reset),
    [GRV_SEL] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_58, dance_58_finished, dance_58_reset),
    [GRV_TIL] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_59, dance_59_finished, dance_59_reset),
    [LFT_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_60, dance_60_finished, dance_60_reset),
    [RGT_ALT] = ACTION_TAP_DANCE_FN_ADVANCED(on_dance_63, dance_63_finished, dance_63_reset),
    [BKSL_HME] = ACTION_TAP_DANCE_FN_ADVANCED(on_bksl_hme, bksl_hme_finished, bksl_hme_reset),
    [PIPE_END] = ACTION_TAP_DANCE_FN_ADVANCED(on_pipe_end, pipe_end_finished, pipe_end_reset),
};
