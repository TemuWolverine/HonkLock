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
|[ESP32 S3 SuperMini](https://s.click.aliexpress.com/e/_c3BMFE2p) | 1 | Any ESP32 is going to work. The supermini's are just adorably tiny, and inexpensive, and relatively easy to come across from a range of vendors. S3 was chosen over C3 for the wifi-5ghz support. <br> The Supermini also has an onboard WS2812 LED (GPIO48), meaning gate status can be indicated (red = closed, green = open) with the included LED.
|[DFRobot DSS-P05](https://core-electronics.com.au/df05bb-standard-servo-5kg.html)|1| most likely any servo of the same size will work here |
|M4 x 8mm|4|To hold the motor in place|
|M4 nut| 4|^|
|M4 x 25mm?|4|To hold the brainbox to the gate|
|M4 Heatset Insert |4|^|
|M5 x 16mm |4| Holds the gate housing together |
|M5 Heatset Insert | 4 |^|


### DF Robot Motor Wiring
Brown = GND  
Red = VCC  
Yellow/Orange = Signal