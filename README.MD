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

### DF Robot Motor Wiring
Brown = GND  
Red = VCC  
Yellow/Orange = Signal