#ifndef _UPD16311_
#define _UPD16311_

#include "Arduino.h"

// commands
#define UPD16311_MODE               (0x00)    // Initialize, select segments
#define UPD16311_DATA               (0x40u)   // write or read, address increment or fix
#define UPD16311_ADDRESS            (0xC0u)   // set display memory address
#define UPD16311_CTRL               (0x80u)   // Display ON/OFF and dimm
#define UPD16311_CTRL_DISP_OFF      (0b0000)  // Display OFF
#define UPD16311_CTRL_DISP_ON       (0b1000)  // Display ON
// SOME DEFINES AND STATIC VARIABLES USED INTERNALLY
#define UPD16311_BYTES_PER_GRID     3
#define UPD16311_MAX_GRID_COUNT     16
#define UPD16311_FRAMEBUFFER_SIZE   (UPD16311_BYTES_PER_GRID * UPD16311_MAX_GRID_COUNT)
#define UPD16311_KEY_MATRIX_SIZE    6

// display modes
enum class UPD16311_DigSegMode
{
  DIG8_SEG20 = 0b0000,
  DIG9_SEG19 = 0b1000,
  DIG10_SEG18 = 0b1001,
  DIG11_SEG17 = 0b1010,
  DIG12_SEG16 = 0b1011,
  DIG13_SEG15 = 0b1100,
  DIG14_SEG14 = 0b1101,
  DIG15_SEG13 = 0b1110,
  DIG16_SEG12 = 0b1111,
};
  
/*!
    @brief Class to manage hardware interface with UPD16311 chipset
*/
class UPD16311 {
public:
  UPD16311(uint8_t mosi, uint8_t miso, uint8_t clk, uint8_t stb);
  void begin(UPD16311_DigSegMode mode);
  uint64_t getKey() { return _keys.d; };
  void readKeys();
  void clearScreen();
  void updateScreen();
  void setLedPort(uint8_t value);
  void setDimming(uint8_t value);
  void setDisplayOn();
  void setDisplayOff();
  void sendData(uint8_t addr, uint8_t *data, uint8_t len);
protected:
  uint8_t _fb[UPD16311_FRAMEBUFFER_SIZE];
private:
  uint8_t _mosi;
  uint8_t _miso;
  uint8_t _clk;
  uint8_t _stb;
  union keys_t {
    uint8_t i[8];
    uint64_t d;
  } _keys;
  uint8_t _dimming;
  void sendCmd(uint8_t value);
};

#endif // _UPD16311_
