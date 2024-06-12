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