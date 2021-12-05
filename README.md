# Widgetpad firmware

Firmware for the [Widgetpad](https://github.com/widget-/widgetpad) dance pad
for rhythm games such as [StepMania](https://github.com/stepmania/stepmania),
Dance Dance Revolution, In The Groove, and Pump It Up.

Additionally, it should work for any FSR-based pad such as Bandit's 
[Build This Travel Pad (BTTP)](https://www.youtube.com/watch?v=y6wGYLE0YI4),
Teejusb's [teejusb-pad](https://github.com/teejusb/teejusb-pad), or my upcoming
budgetpad, conceptually similar to an FSR-modded LTEK.

The firmware was originally based on [Bandit's](https://github.com/andlehma)
[bttp-firmware](https://github.com/BanditsTerrificTravelPad/bttp-firmware)
under the "treat my code as whatever you want lmao" license,
but has since been rewritten.

The firmware is designed hand-in-hand with an upcoming  PCB breakout board for
the Teensy 4.0 and 4.1 to easily connect all the sensors, LEDs, and external
buttons needed.

## License

Licensed under the GNU GPL version 2.

See [LICENSE](LICENSE) for full terms.

## Requirements

This firmware is for the [Teensy 4.0](https://www.pjrc.com/store/teensy40.html)
or [Teensy 4.1](https://www.pjrc.com/store/teensy41.html)

The firmware is built with PlatformIO. One of the following is required:

* [VS Code](https://code.visualstudio.com/) with the [PlatformIO extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) (recommeded)
* [CLion](https://www.jetbrains.com/clion/) with the [PlatformIO plugin](https://plugins.jetbrains.com/plugin/13922-platformio-for-clion)
* Any other IDE with an appropriate PlatformIO plugin

## Features

### Done

* [x] Gamepad button HID input
* [x] Arbitrary amount of panels
* [x] Runtime-modifiable sensor thresholds over serial
* [x] Input smoothing
* [x] Buzzer/speaker feedback
* [x] Multiple sensors per panel
* [x] 1000Hz polling rate
* [x] ~~Saving configuration to onboard EEPROM~~
* [x] External button support (for arcade navigation buttons)
* [x] Convert "Arduino C" to C++
* [x] Use JSON data over Serial instead of "delicate" byte sequences
* [x] Runtime-configurable panel and sensor setup

### To do

* [ ] Write a [GUI configuration tool](https://github.com/widget-/widgetpad-configurator) for the runtime settings of the pad
* Re-do EEPROM saving to use [littlefs](https://github.com/PaulStoffregen/LittleFS/blob/main/src/littlefs/DESIGN.md) for arbitrarily-long config files
* [ ] Replace serial with HID (with a HID event to toggle serial for sending
      configurations)
* [ ] Support for LED status indicators (per sensor? per panel?)
* [ ] Pro Micro support (may require raw HID)
* [ ] LED support for external buttons
* [ ] Basic LED support for panels
* [ ] Onboard controls for LED brightness
* [ ] Extensible LED "programs" for various effects
* [ ] Piezoelectric/vibration motor tactile feedback
* [ ] Research what an on-device screen UI could look like
* [ ] Add multiple profile support

## Additional information

### Help and contact

If you need help, please raise an
[issue](https://github.com/widget-/widgetpad-firmware/issues). You can also get
in touch with me on Discord on the [Stamina Nation](https://discord.gg/6MbWMxaN3f)
server, or feel free to message me directly on Discord at `Widget#0817`.

### Nomenclature

A `pad` is a whole device, for one player.

It consists of (usually) four, five, or nine `panels`, each of which maps to an arrow in most dance rhythm games.

Each `panel` will have one or more `sensors` to decide whether they are
`pressed` once a `threshold` is crossed. The `panel` will remain `pressed` until
the `threshold` minus the `threshold gap` is crossed the other direction.

Additionally, `buttons` may be connected, such as the left/right/start/select
setup found on In The Groove cabinets.

### Development

This project was originally written in C but is shifting to C++ and attempting
to follow `clang-tidy`'s `modernize-*` linter suggestions when possible.

Architecture, code structure, and best practices help from more experienced
C++17 developers would be greatly appreciated.

Pull requests are of course appreciated but general instruction would also help
tremendously.
