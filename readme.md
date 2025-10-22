# UPD16311

This is the core library for vacuum fluorescent displays (VFD) controlled by the NEC uPD16311 driver, providing methods to handle the lower-level hardware functions.

This driver use SPI to communicate, 3 to 4 pins are required to interface.

It needs to be paired with a display-specific library for each display model.

## Display Specific Libraries

The UPD16311 library has build in support for the following display models.

### Kenwood DVF-3070 DVD Player

![Kenwood DVF-3070](docs\Kenwood%20DVF-3070%20display.png)

Also used in Kenwood DVF-3060 DVD Player.
