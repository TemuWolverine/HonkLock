![](Images/Logo.png)

Open-source automated blast gate system, running on ESP32-S3s.

## Implementation
Currently, this is implemented using ESPHome with dependencies on Home Assistant to handle the syncing between gates.

Eventually, the goal is to have two versions - one with ESPHome, one standalone using something like ESP32-IDF/ESP-Now for communication between gates.
ESPHome's advantage is any buttons/remotes (Zigbee, ZWave, Matter, Wifi) connected to Home Assistant can be used to trigger each gate, as well as rapid development and deployment.

## BOM
  
###  Hardware
|Item|Quantity|Note|
|---|---|---|
|'Standard Servo'|1|  |
|M4 x 8mm|4|To hold the motor in place|
|M4 nut| 4|^|
|M4 x 25mm?|4|To hold the brainbox to the gate|
|M4 Heatset Insert |4|^|
|M5 x 16mm |4| Holds the gate housing together |
|M5 Heatset Insert | 4 |^|

### Brain board

|Item|Quantity|Note|
|---|---|---|
|[ESP32 S3 SuperMini](https://s.click.aliexpress.com/e/_c3BMFE2p) | 1 | (Just about) Any ESP32 is going to work. The supermini's are just adorably tiny, inexpensive, and relatively easy to come across from a range of vendors. <br> The Supermini range also has an onboard WS2812 LED (GPIO48), meaning gate status can be indicated (red = closed, green = open) with the included LED.<br><br>**Other supermini variants <ins>except H2 (no wifi)</ins> _should_ work just fine, you will just need to adjust the code slightly.** |
|0.1uF ceramic cap|2|0805 size|
|1uF ceramic cap|1|0805 size|
|10k resistor|2|0805 size|
|33k resistor|2|0805 size|
|P82B715 (SOIC)|1|I2C Extender|
|USB jack|1|Ignore if not using USB for power|
|5.1k|2|Ignore if not using USB for power, 0805 size|
|RJ-45 jack|1||
|3 pin JST XH|1|S3B-XH-A-LF|


||||

### Sensor
|Item|Quantity|Note|
|---|---|---|
|MLX90393|1|magnetometer for detecting tool on/off
|0.1uF ceramic cap|2|0805 size|
|1uF ceramic cap|1|0805 size|
|10k resistor|2|0805 size|
|33k resistor|2|0805 size|
|P82B715 (SOIC)|1|I2C Extender|
|RJ-45 jack|1||

## Alternative power wiring
If you'd rather have centralised power that runs out to each gate, you'll need to [calculate the wire required](https://www.omnicalculator.com/physics/wire-size).

## ESP32 SuperMini Variants  
There are a range of ESP32 "Supermini dev boards", most of which have identical pinouts, but not all are suitable.

|Variant|Suitable|Note|
|---|---|---|
|S3|✅|Dual core Xtensa CPU, previous top of the line chip. Total overkill for this application, but so common that it isn't expensive |
|C3|✅|A lower power/slower alternative to the S3 using a RISC-V singlecore CPU and has 2.4ghz wifi |
|C3 Plus|✅|Seems to be a C3 with external antenna option|
|C6|✅|Single core RISC-V chip, with Wifi 6 (2.4ghz) and thread/zigbee/matter support|
|H2|❌|ESP32-H2 does not have Wifi at all, it has Bluetooth 5LE and thread/zigbee. Currently Wifi is a HonkLock requirement |
