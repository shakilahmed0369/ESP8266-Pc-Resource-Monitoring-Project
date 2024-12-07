
![467474850_1289481315807759_7171576935250104961_n (1)](https://github.com/user-attachments/assets/8b401bf4-db41-439b-a5dc-1e9ee1dc0de2)

# Open Hardware Monitor Data Display with ESP8266

This project allows an ESP8266 (ESP-12E) microcontroller to retrieve system performance metrics such as CPU and GPU temperatures and loads from the Open Hardware Monitor Web API and display them in real-time on a 16x2 I2C LCD. It's a lightweight and efficient solution for creating a hardware monitoring dashboard.


## Features
- Connects to a Wi-Fi network to retrieve hardware monitoring data from a web server.
- Parses JSON data from the Open Hardware Monitor API.
- Displays CPU and GPU temperatures and loads on a 16x2 I2C LCD screen.
- Automatically retries in case of Wi-Fi disconnection.

---

## Requirements
1. **Hardware:**
   - ESP8266 (ESP-12E or compatible board)
   - I2C 16x2 LCD Display
   - Power source for the ESP8266

2. **Software:**
   - Arduino IDE (with ESP8266 Board Package)
   - Required Arduino Libraries:
     - `ESP8266WiFi`
     - `Wire`
     - `LiquidCrystal_I2C`
     - `ArduinoJson`
     - `ESP8266HTTPClient`

3. **Open Hardware Monitor:**
   - Download and install Open Hardware Monitor.
   - Enable the web server API in the software (default port: `8085`).

---

## Wiring
Connect the I2C LCD display to the ESP8266 as follows:
| **LCD Pin** | **ESP8266 Pin** |
|-------------|-----------------|
| VCC         | 3.3V           |
| GND         | GND            |
| SDA         | D2             |
| SCL         | D1             |

---

## Installation
1. Clone or download this repository to your local machine.
2. Open the `.ino` file in Arduino IDE.
3. Update the following placeholders in the code:
   ```cpp
   const char* ssid = "";       // Your Wi-Fi SSID
   const char* password = "";   // Your Wi-Fi Password
   const char* serverName = "http://192.168.0.104:8085/data.json"; // Replace with your Open Hardware Monitor server IP
   ```
4. Install required libraries in the Arduino IDE:
   - Go to **Tools > Manage Libraries**, search for each library name, and install it.
5. Upload the code to your ESP8266 board.

---

## Usage
1. Start the Open Hardware Monitor application on your computer.
2. Ensure the ESP8266 and the computer are on the same Wi-Fi network.
3. Power up the ESP8266. It will:
   - Connect to the Wi-Fi network.
   - Fetch hardware data from the Open Hardware Monitor API every 2 seconds.
   - Display CPU and GPU statistics on the LCD.

---

## Code Structure
### Global Variables
- **Wi-Fi Credentials**: `ssid` and `password`.
- **API Server**: `serverName` (URL to the Open Hardware Monitor data).
- **LCD Setup**: `LiquidCrystal_I2C lcd` initializes the I2C LCD.
- **Fetch Timing**: `fetchInterval` controls the API data fetch frequency.

### Main Functions
- **`setup()`**: 
  - Initializes the LCD and Wi-Fi connection.
  - Displays IP address upon successful connection.
- **`loop()`**: 
  - Periodically fetches data from the API.
- **`fetchData()`**:
  - Sends an HTTP GET request to the API server.
  - Calls `parseAndDisplayData()` to process and display the response.
- **`parseAndDisplayData()`**:
  - Parses the JSON response and extracts CPU/GPU temperatures and loads.
  - Displays data on the LCD.
- **`reconnectWiFi()`**:
  - Reconnects to Wi-Fi if the connection is lost.
- **`getSubstring()`**:
  - Extracts a substring up to a specific delimiter.

---

## Sample Output on LCD
```
CPU: 55C° 40%
GPU: 60C° 70%
```

---

## Troubleshooting
1. **Wi-Fi Connection Issues**:
   - Verify SSID and password.
   - Ensure the ESP8266 and computer are on the same network.

2. **No Data on LCD**:
   - Check the Open Hardware Monitor web server status.
   - Verify the API server IP in the code.

3. **JSON Parsing Error**:
   - Confirm that the Open Hardware Monitor JSON structure matches the code.

---

## License
This project is open-source and available under the MIT License. Feel free to modify and use it for personal projects.
