#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
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

const int EEPROM_SIZE = 512;
const int MAX_IP_COUNT = 20; // Maximum number of IP addresses to store

void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  connectToKnownNetwork();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Online mode");
    sendConnectedIP();
  } else {
    Serial.println("Offline mode");
    scanAndStoreNetworks();
  }

  delay(3000);
}

void connectToKnownNetwork() {
  int retries = 0;
  const int maxRetries = 10;

  while (retries < maxRetries) {
    Serial.println("Scanning for networks...");
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");

    if (n == 0) {
      Serial.println("No networks found");
      retries++;
      delay(1000);
    } else {
      Serial.printf("%d networks found:\n", n);
      for (int i = 0; i < n; ++i) {
        String currentSSID = WiFi.SSID(i);
        Serial.printf("SSID: %s\n", currentSSID.c_str());

        // Check if the current SSID matches a known network
        for (const auto& network : knownNetworks) {
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
              sendStoredData(); // Send stored data after connecting
              return;
            } else {
              Serial.println(" Failed to connect.");
              WiFi.disconnect();
            }
          }
        }
      }
      retries++;
      delay(30000);  // Wait for 30 seconds before retrying the scan
    }
  }

  Serial.println("No known networks found after maximum retries.");
}

void scanAndStoreNetworks() {
  int n = WiFi.scanNetworks();
  Serial.println("Scanning for networks to store...");

  if (n == 0) {
    Serial.println("No networks found to store");
  } else {
    int ipCount = 0;
    for (int i = 0; i < n && ipCount < MAX_IP_COUNT; ++i) {
      String currentSSID = WiFi.SSID(i);
      int32_t rssi = WiFi.RSSI(i);
      String encryptionType = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "Open" : "Encrypted";
      String networkInfo = currentSSID + "," + String(rssi) + "," + encryptionType;

      // Store network info in EEPROM
      int addr = ipCount * 32;
      for (int j = 0; j < networkInfo.length(); ++j) {
        EEPROM.write(addr + j, networkInfo[j]);
      }
      EEPROM.write(addr + networkInfo.length(), '\0'); // Null-terminate the string
      ipCount++;
    }
    EEPROM.commit();
    Serial.println("Stored network info to EEPROM");
  }
}

void sendConnectedIP() {
  HTTPClient http;
  http.begin(wifiClient, serverName);
  http.setTimeout(10000);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<200> doc;
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
}

void sendStoredData() {
  HTTPClient http;
  http.begin(wifiClient, serverName);
  http.setTimeout(10000);
  http.addHeader("Content-Type", "application/json");

  StaticJsonDocument<512> doc;

  // Read stored network info from EEPROM
  JsonArray networksArray = doc.createNestedArray("availableNetworks");
  for (int i = 0; i < MAX_IP_COUNT; ++i) {
    int addr = i * 32;
    char networkBuffer[32];
    int j = 0;
    while (j < 32 && EEPROM.read(addr + j) != '\0') {
      networkBuffer[j] = EEPROM.read(addr + j);
      j++;
    }
    networkBuffer[j] = '\0';
    if (j > 0) {
      networksArray.add(networkBuffer);
    }
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
}
