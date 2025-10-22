#ifndef _KENWOOD_DVF3070_
#define _KENWOOD_DVF3070_

#include "upd16311.h"

// VFD definition
#define DVF3070_GRID_COUNT        12
#define DVF3070_DIGIT_COUNT       10
#define DVF3070_FIRST_DIGIT       1
#define DVF3070_FIRST_DIGIT_BYTE  (UPD16311_BYTES_PER_GRID * DVF3070_FIRST_DIGIT)
#define DVF3070_LAST_DIGIT_BYTE   (UPD16311_BYTES_PER_GRID * DVF3070_DIGIT_COUNT + DVF3070_FIRST_DIGIT_BYTE)

// Key definition
#define DVF3070_KEY_STOP    0x8000
#define DVF3070_KEY_PLAY    0x0004
#define DVF3070_KEY_FFW     0x0040
#define DVF3070_KEY_REW     0x0080
#define DVF3070_KEY_NEXT    0x0010
#define DVF3070_KEY_PREV    0x0020
#define DVF3070_KEY_OPEN    0x0008

// increase text buffer length if required
#ifndef DVF3070_TEXT_BUFFER_LENGTH
#define DVF3070_TEXT_BUFFER_LENGTH  170
#endif

#ifndef DVF3070_READ_KEYS_INTERVAL
#define DVF3070_READ_KEYS_INTERVAL  100
#endif

enum class DVF3070_Indicator {
  RESUME = 0,
  S1,
  S2,
  S3,
  S4,
  INTRO,
  PROG,
  PBC,
  AUDIO,
  DVD,
  VCD,
  VIDEO,
  CD,
  MP3,
  SEC,
  MIN,
  COL2,
  HOUR,
  COL1,
  TRACK,
  DP2,
  CHAP,
  DP1,
  TITLE,
  GROUP,
  PLAY,
  PAUSE,
  LOCK,
  RANDOM,
  REPEAT,
  ONE,
  ALL,
  A,
  B,
};

class Kenwood_DVF3070 : public UPD16311 {
public:
  Kenwood_DVF3070(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t stb);
  void begin();
  void println(const char *text, bool upper = true);
  void setIndicator(DVF3070_Indicator indi, bool set);
  void loop();
  void setScrollDelay(uint16_t sDelay) { scroll_delay = sDelay; };
  void setScrollDelayEnd(uint16_t sDelay) { scroll_delay_end = sDelay; };
private:
  uint8_t _index;
  bool _upper;
  char _txtbuf[DVF3070_TEXT_BUFFER_LENGTH];
  uint16_t scroll_delay = 500;
  uint16_t scroll_delay_end = 3000;
  enum class State
  {
    Idle,
    Show,
    Wait,
  } _state = State::Idle;
  void scroll();
  char upper(char c);
};

#endif // _KENWOOD_DVF3070_
