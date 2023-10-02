# Max7219 Clock IoT Project

Max7219 Clock is an IoT project utilizing the NodeMCU development board ESP8266 and 8 Max7219 LED dot matrix modules stacked together to form a display of 8x64 dots. This project functions as a clock but also offers an interactive web interface for users to control and customize its settings.

## Project Overview

The clock displays the current time by default, but its capabilities extend far beyond that. It connects via Wi-Fi and displays its IP address briefly upon startup. By connecting to the same network, users can access a web interface hosted on the ESP8266 file system. Through this interface, users can set the time, date, switch between 12-hour and 24-hour formats, display scrolling messages, view room temperature and humidity, and more.

## Project Files

- **matrix_clock.ino:** The main file containing the setup and loop functions for the clock.
- **font.c:** Contains the fonts used in the project.
- **utils.c:** Includes functions to generate display fonts.
- **matrix-client (folder):** Contains the Preact UI for the Matrix Clock.

## Hardware Configuration

```c
// ----------------- Hardware devices config -----------------
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define ROWS 8
#define COLS 64
#define DATA_PIN 13    // D7
#define CS_PIN 15      // D8
#define CLK_PIN 14     // D5
#define DHTPIN 10      // SD3 // set the DHT11 data pin to GPIO2
#define DHTTYPE DHT11  // set the DHT11 sensor type

RTC_DS3231 rtc;  // SCL -> D1,  SDA -> D2  RTC module pin config
DHT dht(DHTPIN, DHTTYPE);
MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
// ------------------------------------------------------------
```

## Libraries Used

- MD_Parola
- MD_MAX72xx
- SPI
- LedControl
- RTClib
- DHT
- ESP8266WiFi
- ESPAsyncTCP
- ESPAsyncWebServer
- Arduino_JSON
- LittleFS

## WiFi Configuration

```c
// ------------------ Wi-Fi & Server config -----------------------
const char *wifi_ssid = "wifi ssid";
const char *wifi_password = "wifi password";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
// -------------------------------------------------------
```

## LittleFS Configuration

After building the client, upload all files using LittleFS in the NodeMCU board. LittleFS is a lightweight file system designed for embedded systems, perfect for the limited resources of the ESP8266.

Here's how to upload files using LittleFS:

1. **Install LittleFS Tool:**

   - In Arduino IDE, go to **Tools > Manage Libraries**.
   - Search for "LittleFS" and install the LittleFS filesystem by *Earle F. Philhower, III*.

2. **Build and Upload the Client Files:**

   - Build your Preact UI or client files, ensuring they are in the `build` or `dist` folder, respectively.
   - In the Arduino IDE, go to **Sketch > Export Compiled Binary**. This will generate a `.bin` file.

3. **Upload Files to ESP8266 using LittleFS:**

   - In the Arduino IDE, go to **Sketch > Include Library > LittleFS**.
   - Upload the generated `.bin` file to the ESP8266 by going to **Tools > ESP8266 LittleFS Data Upload**.

This process uploads the client files to the ESP8266's filesystem, making them accessible to the web server.

## Components Used

- ESP8266 (NodeMCU)
- DHT11
- DS3231 Real-Time Clock Module
- USB Type-C Breakout Board

## Getting Started

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/your-username/max7219-clock.git
   ```

2. **Open in Arduino IDE:**

   - Open the Arduino IDE.
   - Load the `matrix_clock.ino` file.
   - Make sure to install all the required libraries.

3. **Configure Hardware:**

   - Connect the components according to the hardware configuration specified.

4. **Upload and Run:**

   - Upload the code to your NodeMCU board.
   - Open the Serial Monitor to view the IP address.
   - Access the clock's settings through the provided IP address on your web browser.

## Features

- Real-time clock display with 8x64 dots resolution.
- Web interface for setting time, date, and other preferences.
- 12-hour and 24-hour time format support.
- Scrolling messages display.
- Room temperature and humidity monitoring.

## Contribution

Contributions and improvements are welcome! Feel free to open issues and pull requests for any enhancements or bug fixes.

