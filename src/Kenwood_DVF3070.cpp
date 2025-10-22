#include "upd16311.h"
#include "Kenwood_DVF3070.h"


const PROGMEM uint16_t Kenwood_DVF3070_font[96] = {
  0x0000, // space
  0x4000, // ! --> _
  0x0028, // " 
  0x55a8, // # 
  0x55ca, // $ 
  0x1fdc, // % 
  0x630e, // & 
  0x0008, // ' 
  0x0210, // ( 
  0x0804, // ) 
  0x0f9c, // * 
  0x0588, // + 
  0x0800, // , 
  0x0180, // - 
  0x0000, // . 
  0x0810, // slash
  0x7062, // 0
  0x1020, // 1 
  0x61a2, // 2 
  0x50a2, // 3 
  0x11e0, // 4 
  0x51c2, // 5 
  0x71c2, // 6 
  0x1022, // 7 
  0x71e2, // 8 
  0x51e2, // 9 
  0x0408, // : 
  0x0808, // ; 
  0x0310, // < 
  0x4180, // = 
  0x0884, // > 
  0x04a2, // ? 
  0x60ea, // @ 
  0x31e2, // A 
  0x54aa, // B 
  0x6042, // C 
  0x542a, // D 
  0x6142, // E 
  0x2142, // F 
  0x70c2, // G 
  0x31e0, // H 
  0x440a, // I 
  0x7020, // J 
  0x2350, // K 
  0x6040, // L 
  0x3074, // M 
  0x3264, // N 
  0x7062, // O 
  0x21e2, // P 
  0x7262, // Q 
  0x23e2, // R 
  0x51c2, // S 
  0x040a, // T 
  0x7060, // U 
  0x2850, // V 
  0x3a60, // W 
  0x0a14, // X 
  0x51e0, // Y 
  0x4812, // Z 
  0x6042, // [ 
  0x0204, // backslash
  0x5022, // ] 
  0x0a00, // ^ 
  0x4000, // _ 
  0x0004, // ` 
  0x6500, // a 
  0x6340, // b 
  0x6180, // c 
  0x58a0, // d 
  0x6900, // e 
  0x0590, // f 
  0x50b0, // g 
  0x2540, // h 
  0x0400, // i 
  0x2808, // j 
  0x0618, // k 
  0x2040, // l 
  0x3580, // m 
  0x2500, // n 
  0x7180, // o 
  0x2144, // p 
  0x10b0, // q 
  0x2100, // r 
  0x4280, // s 
  0x6140, // t 
  0x7000, // u 
  0x2800, // v 
  0x3a00, // w 
  0x0a14, // x 
  0x50a8, // y 
  0x4900, // z 
  0x4906, // { 
  0x0408, // | 
  0x4292, // } 
  0x0990, // ~ 
  0x0000, // (del) 
};

const PROGMEM uint8_t Kenwood_DVF3070_indicator[68] = {
  0, 0x01,  // RESUME
  0, 0x02,  // S1
  0, 0x04,  // S2
  0, 0x08,  // S3
  0, 0x10,  // S4
  0, 0x20,  // INTRO
  0, 0x40,  // PROG
  0, 0x80,  // PBC
  1, 0x01,  // AUDIO
  1, 0x02,  // DVD
  1, 0x04,  // VCD
  1, 0x08,  // VIDEO
  1, 0x10,  // CD
  1, 0x20,  // MP3
  3, 0x01,  // SEC
  9, 0x01,  // MIN
  10, 0x80, // COL
  15, 0x01, // HOUR
  16, 0x80, // COL
  18, 0x01, // TRACK
  19, 0x80, // DP
  21, 0x01, // CHAP
  22, 0x80, // DP
  27, 0x01, // TITLE
  30, 0x01, // GROUP
  33, 0x01, // PLAY
  33, 0x02, // PAUSE
  33, 0x04, // LOCK
  33, 0x08, // RANDOM
  33, 0x10, // REPEAT
  33, 0x20, // ONE
  33, 0x40, // ALL
  33, 0x80, // A
  34, 0x01, // B
};

/*!
    @brief  Constructor for Kenwood DVF3070 display using the UPD16311 controller,
            using software (bitbang) SPI.
    @param  mosi  Arduino pin # for bitbang SPI MOSI signal (required).
    @param  miso  Arduino pin # for bitbang SPI MISO signal (optional,
            use same pin as mosi if unused or both pins are tied together).
    @param  clk   Arduino pin # for bitbang SPI SCK signal (required).
    @param  stb   Arduino pin # for strobe (required).
    @return Kenwood_DVF3070 object.
    @note   Call the object's begin() function before use.
*/
Kenwood_DVF3070::Kenwood_DVF3070(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t stb)
  :UPD16311(mosi, miso, clk, stb) {
}

/*!
    @brief  Initialize pins and sends initialization procedure commands.
    @return None (void).
    @note   MUST call this function before any updates!
*/
void Kenwood_DVF3070::begin()
{
  UPD16311::begin(UPD16311_DigSegMode::DIG12_SEG16);
  clearScreen();
  updateScreen();
}

/*!
    @brief  Writes text to the display buffer.
    @return None (void).
*/
void Kenwood_DVF3070::scroll()
{
  char c;
  uint16_t val;
  uint8_t idx = DVF3070_LAST_DIGIT_BYTE;
  bool fill = false;
  
  const char *_str;
  _str = _txtbuf + _index;
  while (idx > DVF3070_FIRST_DIGIT_BYTE)
  {
	if (!fill) {
      c = (_upper) ? upper(*_str++) : *_str++;
      if (c != 0) {
        val = pgm_read_word(&Kenwood_DVF3070_font[c - 32]);
      } else {
        fill = true;
        val = 0;
      }
    }
    idx--; // skip unused byte in display RAM
	idx--;
    _fb[idx] = (_fb[idx] & 0x80) | (uint8_t)(val >> 8);
	idx--;
    _fb[idx] = (_fb[idx] & 0x01) | (uint8_t)(val & 0x00ff);
  }

  if (*_str && !fill) {
    // str length > display char count, need to scroll the text.
	_index++;
  } else if (_index == 0) {
    // str length <= display char count.
    _state = State::Idle;
  } else {
    // str length > display char count and just printed the last char. Restart printing at index 0.
    _index = 0;
  }
}

/*!
    @brief  Copies text to the text buffer.
    @param  text  Pointer to the text to be displayed.
    @param  upper If true, the text is converted to upper case.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
*/
void Kenwood_DVF3070::println(const char *text, bool upper) {
  strncpy(_txtbuf, text, DVF3070_TEXT_BUFFER_LENGTH);
  _txtbuf[DVF3070_TEXT_BUFFER_LENGTH-1] = 0; // make sure that we have a null terminated string if source is bigger than destination
  _upper = upper;
  _index = 0;
  _state = State::Show;
  scroll();
  _index = 0;
}

/*!
    @brief  Set or reset an indicator on the Kenwood DVF3070 display.
    @param  indi  The indicator. Must be one of DVF3070_Indicator values.
    @param  set   true to show the indicator, false to hide the indicator.
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
*/
void Kenwood_DVF3070::setIndicator(DVF3070_Indicator indi, bool set) {
  uint8_t idx = pgm_read_byte(&Kenwood_DVF3070_indicator[(uint8_t)indi * 2]);
  uint8_t val = pgm_read_byte(&Kenwood_DVF3070_indicator[(uint8_t)indi * 2 + 1]);
  if (set) {
    _fb[idx] |= val;
  } else {
    _fb[idx] &= ~val;
  }
}

/*!
    @brief  Should be called regulary to scroll text over the display.
    @return None (void).
*/
void Kenwood_DVF3070::loop()
{
  static uint32_t scroll_time = 0;
  if (_state == State::Wait && millis() > scroll_time)
  {
    _state = State::Show;
  }
  if (_state == State::Show)
  {
    scroll();
    UPD16311::updateScreen();
    if (_index < 2)
      scroll_time = millis() + scroll_delay_end;
    else
      scroll_time = millis() + scroll_delay;
    _state = State::Wait;
  }
  
  // read keys
  static uint32_t key_time = 0;
  if (millis() > key_time)
  {
    readKeys();
    key_time = millis() + DVF3070_READ_KEYS_INTERVAL;
  }
}

/*!
    @brief  Converted a char to upper case.
    @param  c the char to be converted
    @return char in upper case
*/
char Kenwood_DVF3070::upper(char c)
{
  if (c >= 97 && c <= 122)
    return c - 32;
  else
    return c;
}
