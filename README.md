# IP Fetcher

## Overview
The IP Fetcher is an ESP8266-based project that connects to predefined WiFi networks, retrieves the device's IP address, determines its corresponding floor number, and sends this information to a specified backend server. This system is designed for environments where devices need to connect to multiple known WiFi networks and report their location via their IP address.

## Demo Video
Watch the demo video to see the IP Fetcher in action:

   ### Local Video
   <img src="gifs/p1p1_step.gif" alt="step" width="270">

## Features
- **WiFi Auto-Connect:** Scans for and connects to predefined WiFi networks.
- **IP Address Retrieval:** Fetches the device's local IP address upon successful WiFi connection.
- **Floor Mapping:** Maps the device's IP address to a specific floor number.
- **Data Reporting:** Sends the IP address and floor number to a backend server via HTTP POST requests.
   ### Offline Mode:
   
   In Offline Mode, the device operates independently without being connected to any WiFi network. Here's a breakdown of how it works:
   
   1. **Continuous Network Scanning:**
      - The device continuously scans for available WiFi networks in its vicinity using the `WiFi.scanNetworks()` function provided by the ESP8266WiFi library.
      - It retrieves information about each detected network, including the SSID (network name), signal strength (RSSI), and encryption type.
   
   2. **Storage of Network Information:**
      - Once networks are detected, the device stores their information locally in the EEPROM (Electrically Erasable Programmable Read-Only Memory).
      - The EEPROM acts as persistent storage, allowing the device to retain information even when powered off or reset.
   
   3. **Capacity Limitation:**
      - To prevent EEPROM overflow, the device imposes a maximum limit on the number of networks it can store. This limit ensures efficient memory management and prevents resource depletion.
   
   4. **Purpose and Use Cases:**
      - Offline Mode is essential for scenarios where the device cannot establish a connection to a known WiFi network due to reasons such as network unavailability, incorrect credentials, or environmental factors like signal interference.
      - It enables the device to remember encountered networks and prepares it for subsequent attempts to connect when transitioning to Online Mode.
   
   ### Online Mode:
   
   In Online Mode, the device successfully connects to a known WiFi network and performs the following tasks:
   
   1. **WiFi Connection Establishment:**
      - The device attempts to connect to a predefined set of known WiFi networks stored in its memory.
      - It iterates through the list of known networks and tries to establish a connection using the SSID and password provided for each network.
   
   2. **Connection Status Monitoring:**
      - During the connection process, the device monitors its WiFi connection status using the `WiFi.status()` function.
      - If a connection attempt fails, the device retries connecting to other known networks, implementing a retry mechanism to ensure robustness in network connectivity.
   
   3. **IP Address Retrieval:**
      - Upon successfully connecting to a WiFi network, the device retrieves its local IP address within the network using the `WiFi.localIP()` function.
      - The IP address serves as a unique identifier for the device within the network and is crucial for determining its location.
   
   4. **Floor Number Determination:**
      - With the acquired IP address, the device looks up a predefined IP-to-floor mapping stored in its memory.
      - This mapping associates specific IP addresses with corresponding floor numbers, enabling the device to identify its current floor level within a building or facility.
   
   5. **Data Transmission to Backend Server:**
      - Once the device determines its IP address and corresponding floor number, it constructs a JSON payload containing this information.
      - Using an HTTP POST request, the device sends the JSON payload to a specified backend server endpoint, typically hosted on the internet.
      - The backend server processes the received data, potentially logging it in a database or triggering additional actions based on predefined rules.
   
   By seamlessly transitioning between Offline and Online Modes, the device optimizes its functionality, ensuring reliable operation and accurate reporting of location data under varying network conditions and environments.


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
