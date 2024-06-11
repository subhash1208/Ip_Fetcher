#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <map>

const String serverName = "http://backend.santhosh-toorpu.workers.dev/api/v1/sensor/data";
WiFiClient wifiClient;

// Define a struct to hold WiFi network credentials
struct WifiCredentials {
  String ssid;
  String password;
};

// Create an array to store predefined WiFi network credentials
WifiCredentials knownNetworks[] = {
  {"Online", "123456789"},
  {"Rahul", "password123"},
  {"Airtel_Srinivasu_4899","Srinu@1975"}
  // Add more known networks here
};

// Create a map to store the IP address to floor number mapping
std::map<String, int> ipFloorMap = {
  {"192.168.70.166", 1},
  {"192.168.61.166", 2},
  {"192.168.1.102", 3},
  // Add more IP address to floor number mappings here
};

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  scanAndConnectToNetwork();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(wifiClient, serverName);
    http.setTimeout(10000);
    http.addHeader("Content-Type", "application/json");

    // Use a dynamic JSON document to avoid stack overflow
    DynamicJsonDocument doc(256);
    doc["ipAddress"] = WiFi.localIP().toString();

    String ipAddress = WiFi.localIP().toString();
    if (ipFloorMap.count(ipAddress) > 0) {
      doc["floorNumber"] = ipFloorMap[ipAddress];
    } else {
      doc["floorNumber"] = 0;
    }

    String requestBody;
    serializeJson(doc, requestBody);
    Serial.println("JSON Request Body: " + requestBody);

    int httpResponseCode = http.POST(requestBody);
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response Code: " + String(httpResponseCode));
      Serial.println("Response: " + response);
    } else {
      Serial.print("Error Code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
    scanAndConnectToNetwork();
  }

  delay(3000);
}

void scanAndConnectToNetwork() {
  Serial.println("Scanning for networks...");
  int n = WiFi.scanNetworks();
  Serial.println("Scan done");

  if (n == 0) {
    Serial.println("No networks found");
  } else {
    Serial.printf("%d networks found:\n", n);
    for (int i = 0; i < n; ++i) {
      String currentSSID = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);
      String encryptionType = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "Open" : "Encrypted";
      Serial.printf("SSID: %s, Signal Strength: %d dBm, Encryption: %s\n", currentSSID.c_str(), rssi, encryptionType.c_str());
    }

    Serial.println("Attempting to connect to known networks...");
    for (const auto& network : knownNetworks) {
      for (int i = 0; i < n; ++i) {
        String currentSSID = WiFi.SSID(i);
        if (currentSSID == network.ssid) {
          Serial.printf("Connecting to %s...", currentSSID.c_str());
          WiFi.begin(network.ssid, network.password);

          int connectionRetries = 0;
          const int maxConnectionRetries = 20;
          while (WiFi.status() != WL_CONNECTED && connectionRetries < maxConnectionRetries) {
            delay(500);
            Serial.print(".");
            connectionRetries++;
          }

          if (WiFi.status() == WL_CONNECTED) {
            Serial.println(" Connected!");
            Serial.print("Connected to WiFi network: ");
            Serial.println(WiFi.SSID());
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
            return;
          } else {
            Serial.println(" Failed to connect.");
            WiFi.disconnect();
          }
        }
      }
    }

    Serial.println("Failed to connect to any known networks.");
  }
}
