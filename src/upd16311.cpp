#include "upd16311.h"

/*!
    @brief  Constructor for SPI UPD16311 VFD controller,
            using software (bitbang) SPI.
    @param  mosi  Arduino pin # for bitbang SPI MOSI signal (required).
    @param  miso  Arduino pin # for bitbang SPI MISO signal (optional,
            use same pin as mosi if unused or both pins are tied together).
    @param  clk   Arduino pin # for bitbang SPI SCK signal (required).
    @param  stb   Arduino pin # for strobe (required).
    @return UPD16311 object.
    @note   Call the object's begin() function before use.
*/
UPD16311::UPD16311(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t stb)
{
  _mosi = mosi;
  _miso = miso;
  _clk = clk;
  _stb = stb;
}

/*!
    @brief  Initialize pins and sends initialization procedure commands.
    @param  mode
            The display mode. Sets number of grids and segments for the connected VFD.
            MUST be one of the UPD16311_DigSegMode values defined in upd16311.h.
    @return None (void).
    @note   MUST call this function before any updates!
*/
void UPD16311::begin(UPD16311_DigSegMode mode)
{
  pinMode(_clk, OUTPUT);
  pinMode(_mosi, OUTPUT);
  if (_miso != _mosi) {
    pinMode(_miso, INPUT);
  }
  pinMode(_stb, OUTPUT);

  _dimming = 0;

  sendCmd((uint8_t)mode);     // set digits and segments
  sendCmd(UPD16311_DATA | 0); // data to display, inc. address, normal mode
  sendCmd(UPD16311_CTRL | UPD16311_CTRL_DISP_ON | _dimming);  // Display on, full dimming
}

/*!
    @brief Issue single command to UPD16311, using soft SPI.
    This is a private function, not exposed.
    @param value
    The command character to send to the display.
    Refer to UPD16311 data sheet for commands.
    @return None (void).
*/
void UPD16311::sendCmd(uint8_t value)
{
  digitalWrite(_stb, LOW);    // STB low
  shiftOut(_mosi, _clk, LSBFIRST, value);
  digitalWrite(_stb, HIGH);   // STB high
}

/*!
    @brief Send data to UPD16311 display RAM, using soft SPI.
    @param addr
    Address in the display RAM.
    @param data
    A pointer to the data to be written.
    @param len
    Number of bytes to be written.
    @return None (void).
*/
void UPD16311::sendData(uint8_t addr, uint8_t *data, uint8_t len)
{
  digitalWrite(_stb, LOW);    // STB low
  shiftOut(_mosi, _clk, LSBFIRST, UPD16311_ADDRESS | addr);
  while (len--)
  {
    shiftOut(_mosi, _clk, LSBFIRST, *data++);
  }
  digitalWrite(_stb, HIGH);   // STB high
}

/*!
    @brief Read the complete key matrix data storage of the UPD16311.
    @return None (void).
    @note Call getKey() to check if a specific bit/key is set/pressed.
*/
void UPD16311::readKeys()
{
  digitalWrite(_stb, LOW);    // STB low
  shiftOut(_mosi, _clk, LSBFIRST, UPD16311_DATA | 2);
  
  if (_miso == _mosi) {
    pinMode(_miso, INPUT);
  }
  
  for (uint8_t idx = 0; idx<UPD16311_KEY_MATRIX_SIZE; idx++)
  {
    _keys.i[idx] = shiftIn(_mosi, _clk, LSBFIRST);
  }
  digitalWrite(_stb, HIGH);     // STB high
  
  if (_miso == _mosi) {
    pinMode(_mosi, OUTPUT);
  }
  
  sendCmd(UPD16311_DATA | 0);
}

/*!
    @brief  Clear contents of display buffer (set all segments to off).
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
*/
void UPD16311::clearScreen()
{
  memset(_fb, 0, UPD16311_FRAMEBUFFER_SIZE);
}

/*!
    @brief  Push data currently in RAM to UPD16311 controller.
    @return None (void).
    @note   Drawing operations are not visible until this function is
            called.
*/
void UPD16311::updateScreen()
{
  sendData(0, _fb, UPD16311_FRAMEBUFFER_SIZE);
}

/*!
    @brief  Set the LED port to control LED1 - LED5.
    @param  value
            The value to be written to the LED port.
            The data of bits 6 through 8 is ignored.
    @return None (void).
*/
void UPD16311::setLedPort(uint8_t value)
{
  sendCmd(UPD16311_DATA | 1);    // data to LED port, inc. address, normal mode
  sendData(0, &value, 1);
  sendCmd(UPD16311_DATA | 0);    // data to display, inc. address, normal mode
}

/*!
    @brief  Dim the display.
    @param  value
            Sets the display brightness. Must be a value between 0 and 7.
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            updateScreen() function -- buffer contents are not changed.
*/
void UPD16311::setDimming(uint8_t value)
{
  _dimming = min(value, (uint8_t)7);
  sendCmd(UPD16311_CTRL | UPD16311_CTRL_DISP_ON | _dimming);
}

/*!
    @brief  Turn on the display.
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            updateScreen() function -- buffer contents are not changed.
*/
void UPD16311::setDisplayOn()
{
  sendCmd(UPD16311_CTRL | UPD16311_CTRL_DISP_ON | _dimming);
}

/*!
    @brief  Turn off the display.
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            updateScreen() function -- buffer contents are not changed.
*/
void UPD16311::setDisplayOff()
{
  sendCmd(UPD16311_CTRL | UPD16311_CTRL_DISP_OFF | _dimming);
}
