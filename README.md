
# IRRemoteESP32_IoT
Sender/Receiver IR Remote for Home IoT.

# Description

I decided to create this simple home project to reduce the inconvenience of having multiple remote controls in the main room, TV, HomeTheater, HDMI Switch, SateliteTV, AC, etc.
Using some simple libraries we can dump the codes from a IR Remote to replicate from our device.

# Characteristics
Send/Receive IR Codes in multiple formats.
Setup and start a local Web Service for wireless connection.
Create a simple WebApp for communication with the ESP32 and send IR Codes.

# Technologies

Arudino IDE
ESP32 (I used a DEV KIT V1)
HTML/JavaScript
C++

# Clone repository

1. Clone this repo: `git clone https://github.com/FireMask/IRRemoteESP32_IoT.git`
2. Use the .ino file to compile and upload in your ESP32
> You need to have installed the IRremoteESP8266 Library
> If you are using Arduino IDE you have to had the ESP32 previously configured.
3. The index.html file is a unencoded version of the already loaded html in the .ino file.
> If you need or have a change to do in the html file, you have to do it in the unencoded file, then offuscate and replace on the .ino file.

# Wires

1. The default PIN configuration for the IR Receiver and Sender (IR Led) are:
- Receiver IR Led: Pin 27
- IR Led: Pin 26
2. Once you have configured and mapped the codes you need, you don't need to have the IR Receiver, and change to false the flag. 
  ```
Line: 15 RemoteArudiono.ino
const bool EnableIRRead =  false;
```

# Configuration

  1. Set the variable to true.
  ```
Line: 15 RemoteArudiono.ino
const bool EnableIRRead =  true;
```
2. This will start the read/reception of the IR codes of every IR Remote.
3. In the Serial Monitor you wil get the HEX Code and the Protocol (if it is recognized).
4. You will map this HEX Code to a button in the HTML code
  ```
Index.html
<div id="1" class="button red" onclick="actionButton('NEC', '0x20df10ef')">Power</div>
```
5. You can have as many buttons and IR Codes as you want, just have in mind that you need to take count of the response time of the device you are trying to control.
	> For example, my TV is not the newest, and it takes ~1 sec at most to respond the command, even on the real TV Remote.
6. Having this, i made a function to make "automations", running a secuence of buttons to make an action.
	> In my case, for Turn Off/On every device in my room or change the input HDMI. 

# Use

1. Connect the ESP32 to a computer, using the Serial Monitor in the Arudino IDE we can get the local IP that the DHCP assigned to our device.
2. Having the local IP assigned to our device, we can connect the ESP32 to a 3.3v or 5v source.
> This will start the Web Service and connect to the local network, at this point, we don't need the computer anymore.
2. In a Web Browser (PC or Mobile) go to `http://ESP32-local-IP:80`.
3. Use the WebUI to interact with the buttons.
> Make sure the IR Diode is pointing to the device you are trying to control.

# Contribution
If you have any suggestions or improvements, feel free to make a pull request.

# Licence