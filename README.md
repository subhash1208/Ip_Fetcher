# IP Fetcher

## Overview
The IP Fetcher is an ESP8266-based project that connects to predefined WiFi networks, retrieves the device's IP address, determines its corresponding floor number, and sends this information to a specified backend server. This system is designed for environments where devices need to connect to multiple known WiFi networks and report their location via their IP address.

## Features
- **WiFi Auto-Connect:** Scans for and connects to predefined WiFi networks.
- **IP Address Retrieval:** Fetches the device's local IP address upon successful WiFi connection.
- **Floor Mapping:** Maps the device's IP address to a specific floor number.
- **Data Reporting:** Sends the IP address and floor number to a backend server via HTTP POST requests.

## Installation
### Prerequisites
Ensure you have the following installed:
- Arduino IDE
- ESP8266 board package
- Required libraries: `ESP8266WiFi`, `ESP8266HTTPClient`, `ArduinoJson`

### Steps
1. **Clone the repository:**
   ```bash
      git clone https://github.com/subhash1208/Ip_Fetcher.git
   ```
2. **Open the project in Arduino IDE:**
   - Open the Arduino IDE.
   - Navigate to `File -> Open...` and select the `ip_fetcher.ino` file from the cloned repository.
3. **Install the required libraries:**
   - Go to `Sketch -> Include Library -> Manage Libraries...`
   - Install the `ESP8266WiFi`, `ESP8266HTTPClient`, and `ArduinoJson` libraries.
## Usage
1. **Configure WiFi Credentials:**
   - Modify the `knownNetworks` array in the code to include your predefined WiFi network SSIDs and passwords.
   ```cpp
   WifiCredentials knownNetworks[] = {
     {"SSID1", "password1"},
     {"SSID2", "password2"},
     // Add more known networks here
   };
   ```
2. **Configure IP to Floor Mapping:**
   - Modify the `ipFloorMap` to map IP addresses to corresponding floor numbers.
   ```cpp
   std::map<String, int> ipFloorMap = {
   {"192.168.1.100", 1},
   {"192.168.1.101", 2},
   // Add more IP to floor mappings here
   };
   ```
3. **Upload the Code:**
   - Connect your ESP8266 device to your computer.
   - Select the appropriate board and port in the Arduino IDE.
   - Click the upload button to flash the code onto the ESP8266.

4. **Monitor the Output:**
   - Open the Serial Monitor from the Arduino IDE (`Tools -> Serial Monitor`) to view debug prints and confirm the device is connecting to WiFi and sending data.

## Important Configuration
- Ensure the backend server URL (`serverName`) is correct and accessible.
- Verify WiFi credentials and IP-to-floor mappings are correctly configured.

## Environmental Requirements
- The device should be within the range of one of the predefined WiFi networks.
- Ensure the backend server is running and accessible from the network the device connects to.


