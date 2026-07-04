![](Images/Logo.png)

Open-source automated blast gate system, running on ESP32-S3s.

### Implementations
There will be two implementations, the first will use ESPHome, Home Assistant and Node Red.  
ESPHome's advantage is any buttons/remotes (Zigbee, ZWave, Matter, Wifi) connected to Home Assistant can be used to trigger each gate.

The second, ESPNow, will be standalone, requiring no central server to manage the state of the gates.  
ESPNow's advantage is the auto-discovery of gates

| Name   | Configurable Without Flashing  | No Infrastructure Required  | Functional  | Works With Any (HA-connected) remote   |
|---|---|---|---|---|
| ESPHome   | ✅|❌| ✅  | ✅  |
| ESPNow  | ❌  |✅   | ❌  | ❌  |

### Hardware (Per Gate)
  
|Item|Quantity|Note|
|---|---|---|
|[ESP32 S3 SuperMini](https://s.click.aliexpress.com/e/_c3BMFE2p) | 1 | (Just about) Any ESP32 is going to work. The supermini's are just adorably tiny, inexpensive, and relatively easy to come across from a range of vendors. <br> The Supermini range also has an onboard WS2812 LED (GPIO48), meaning gate status can be indicated (red = closed, green = open) with the included LED.<br><br>**Other supermini variants <ins>except H2 (no wifi)</ins> _should_ work just fine, you will just need to adjust the code slightly.**
|[DFRobot DSS-P05](https://core-electronics.com.au/df05bb-standard-servo-5kg.html)|1| most likely any other servo of the same size will work here |
| [USB-C Breakout Board](https://s.click.aliexpress.com/e/_c4Ki5OT3) | 1| See alternative power wiring below |
|M4 x 8mm|4|To hold the motor in place|
|M4 nut| 4|^|
|M4 x 25mm?|4|To hold the brainbox to the gate|
|M4 Heatset Insert |4|^|
|M5 x 16mm |4| Holds the gate housing together |
|M5 Heatset Insert | 4 |^|


### Alternative power wiring
If you'd rather have centralised power that runs out to each gate, you'll need to [calculate the wire required](https://www.omnicalculator.com/physics/wire-size).
The motor will draw a max of 350mA(@5v), with the ESP32 drawing ~300mA(@3.3v) with wifi active and the LED on. If you assume a safe 1A per gate, 5m of wiring will require 15 AWG/1.32mm wiring.

### ESP32 SuperMini Variants  
There are a range of ESP32 "Supermini dev boards", most of which have identical pinouts, but not all are suitable.

|Variant|Suitable|Note|
|---|---|---|
|S3|✅|Dual core Xtensa CPU, previous top of the line chip. Total overkill for this application, but so common that it isn't expensive |
|C3|✅|A lower power/slower alternative to the S3 using a RISC-V singlecore CPU and has 2.4ghz wifi |
|C3 Plus|✅|Seems to be a C3 with external antenna option|
|C6|✅|Single core RISC-V chip, with Wifi 6 (2.4ghz) and thread/zigbee/matter support|
|H2|❌|ESP32-H2 does not have Wifi at all, it has Bluetooth 5LE and thread/zigbee. Currently Wifi is a HonkLock requirement |

### DF Robot Motor Wiring
Brown = GND  
Red = VCC  
Yellow/Orange = Signal