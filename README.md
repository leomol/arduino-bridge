# Arduino Bridge
Arduino libraries for custom projects.

## Prerequisites
* [Arduino IDE][Arduino IDE] (last compiled and tested with Arduino 1.8.8)
* [Arduino Mega2560][ArduinoMega2560] or [ArduinoUNO] microcontroller.

## Installation
* Install Arduino IDE
* Add `Bridge package`:
  - Got to `File`>`Preferences`>`Settings`>`Additional Board Manager URLs`
  - Append the following line: `https://raw.githubusercontent.com/leomol/arduino-bridge/master/release/package_leomol_index.json`
  - Go to `Tools`>`Board:"..."`>`Boards Manager...`
  - Type `Bridge` in the input box
  - Hover the mouse over the result so that the `Install` button is revealed and click it. Close the window.
  - Go to `Tools`>`Board:"..."` and select the Arduino microcontroller of your choice in the category `Bridge AVR Boards`

## Updates
Library updates (including example code) will be shown as a pop-up at the bottom-left of the `Arduino IDE`. When prompted for an update, just follow the instructions. Should anything fail with the update, head back to `Boards Manager`, uninstall the current version, and choose to install a previous version from the drop-down menu.

## Usage
* Go to `File`>`Examples`> and select and example sketch under the sub-menu `Bridge` in the category `Examples for Arduino/Genuino Mega or Mega 2560`

## Examples
* `Bridge`: General purpose, firmata-like installation, to use in larger projects such as the [Octupus Maze][Octupus] and the [Tablet VR][TabletVR]
* `StandaloneTreadmill`: Setup an Arduino for a standalone treadmill task (without monitor tablets).
	The treadmill consists of a rotary encoder, a photo sensor, and a belt with a reflective tape to detect reward zones, also a pinch valve and a lick sensor for reward delivery.
* `BridgeForParser`: Minimalistic code compatible with `Parser` (unpublished app for Virtual Environments).
* `LapCounterIRx5`: Code to count laps in a square maze (unpublished work).

## Troubleshooting
* The compilation/upload process will fail if `Arduino IDE` finds conflicting code. Solution: remove `Documents/Arduino/Bridge`, `Documents/Arduino/libraries/Bridge`, and any files inside `Documents/Arduino` that use the namespace `bridge`.
* `Bridge examples` won't be shown in the menu unless a board from the category `Bridge AVR Boards` is selected. Solution: select a board from `Bridge AVR Boards` first.

## Version History
### 0.1.0
* Initial Release: Library and example code

## License
This project is licensed under the [GNU GPLv3 License][LICENSE.md].
© 2020 [Leonardo Molina][Leonardo Molina]

[Leonardo Molina]: https://github.com/leomol
[Arduino IDE]: https://www.arduino.cc/en/Main/Software
[ArduinoMega2560]: https://store.arduino.cc/usa/arduino-mega-2560-rev3
[ArduinoUno]:https://store.arduino.cc/usa/arduino-uno-rev3
[Octupus]:https://github.com/leomol/octupus-maze
[TabletVR]:https://github.com/leomol/tablet-vr
[LICENSE.md]: LICENSE.md