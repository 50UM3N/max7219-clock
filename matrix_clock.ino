#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LedControl.h>
#include <RTClib.h>
#include <DHT.h>
#include <math.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include "LittleFS.h"
#include "utils.c"

// ----------------- Hardware devices config -----------------
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 8
#define ROWS 8
#define COLS 64
#define DATA_PIN 13    // D7
#define CS_PIN 15      // D8
#define CLK_PIN 14     // D5
#define DHTPIN 10      // SD3// set the DHT11 data pin to GPIO2
#define DHTTYPE DHT11  // set the DHT11 sensor type

RTC_DS3231 rtc;  // SCL -> D1,  SDA -> D2  RTC module pin config
DHT dht(DHTPIN, DHTTYPE);
MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
LedControl lc = LedControl(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
// ------------------------------------------------------------

// ----------------- Opcode config ----------------------
#define MODE 0
#define SET_TIME 1
#define SET_DATE 2
#define SET_DATE_FORMAT 3
#define SET_ANIMATION_SPEED 4
#define SET_STAY_TIME 5
#define SET_TEXT 6
// ------------------------------------------------------

// ----------------- Display mode config -----------------
#define DISPLAY_TIME 0
#define DISPLAY_DATE 1
#define DISPLAY_TEMPERATURE 2
#define DISPLAY_HUMIDITY 3
#define DISPLAY_SCEOLLING_TEXT 4
#define DISPLAY_LOOP 5

#define SCROLLING_SPEED 50
#define SCROLLING_TEXT_SIZE 100

#define HOLD_TIME 5000

char message[SCROLLING_TEXT_SIZE];
int display_mode = DISPLAY_TIME;                                 // as default display time
char scrolling_message[SCROLLING_TEXT_SIZE] = "Hello World";      // the scrolling message
int scrolling_speed = SCROLLING_SPEED;
int use_24_hour_format = 0;
int message_hold_time = HOLD_TIME;
// ----------------------------------------------------

// ------------------ Interrupt config -----------------------
#define ISR_PIN 12  // D6
//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;
unsigned long last_button_time = 0;
// ----------------------------------------------------

// ------------------ Wi-Fi & Server config -----------------------
const char *wifi_ssid = "SOUMEN";
const char *wifi_password = "soumen2015";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
// -------------------------------------------------------

unsigned long _time;
bool animate = true;

void ICACHE_RAM_ATTR isr_display_mode() {
  button_time = millis();
  if (button_time - last_button_time > 250) {
    // change the mode of the display one by one
    Serial.println("Interrupt button pressed");
    last_button_time = button_time;
  }
}

void setup() {
#ifndef ESP8266
  while (!Serial)
    ;
#endif

  // ---------------- Iint Serial Monitor -------------------
  Serial.begin(115200);
  delay(3000);  // wait for console opening
  // --------------------------------------------------------

  // ---------------- Iint display -------------------
  display.begin();
  display.setIntensity(0);
  display.displayClear();
  // -------------------------------------------------

  //-------------------Init LittleFS------------------------

  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
   Serial.println("LittleFS mounted successfully");
  }

  // ------------------Init Wifi ---------------------------
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    print_matrix(generate_text(1, 24, "WiFi."));
    delay(300);
    print_matrix(generate_text(1, 24, "WiFi.."));
    delay(300);
    print_matrix(generate_text(1, 24, "WiFi..."));
    delay(300);
  }
  String Ip = WiFi.localIP().toString();
  print_matrix(generate_text(1, ((64 - ((Ip.length() - 2) * 4)) / 2), Ip.c_str()));
  delay(10000);
  Serial.println(Ip);
  display.displayClear();
  // ------------------------------------------------------

  // ---------------- Iint RTC -------------------
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // --------------------------------------------------------


  // ---------------- Iint DHT -------------------
  dht.begin();
  // --------------------------------------------------------


  // ---------------- Iint ISR -------------------
  pinMode(ISR_PIN, INPUT_PULLUP);
  attachInterrupt(ISR_PIN, isr_display_mode, FALLING);
  // --------------------------------------------------------


  // ---------------- Init WS and Server -------------------
  ws.onEvent(onEvent);
  server.addHandler(&ws);
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
     request->send(LittleFS, "/index.html", "text/html");
   });
   server.serveStatic("/", LittleFS, "/");
  server.begin();
  // --------------------------------------------------------
  _time = millis();
}

void loop() {
  DateTime now = rtc.now();
  switch (display_mode) {
    case DISPLAY_TIME:
      print_matrix(generate_time(now.hour(), now.minute(), now.second(), use_24_hour_format));
      break;
    case DISPLAY_DATE:
      print_matrix(generate_date(now.dayOfTheWeek(), now.day(), now.month(), now.year()));
      break;
    case DISPLAY_TEMPERATURE:
      print_matrix(generate_temp(dht.readTemperature()));
      break;
    case DISPLAY_HUMIDITY:
      print_matrix(generate_humidity(dht.readHumidity()));
      break;
    case DISPLAY_SCEOLLING_TEXT:
      if (display.displayAnimate()) {
        display.displayReset();
      }
      break;
    case DISPLAY_LOOP:
      print_matrix(generate_time(now.hour(), now.minute(), now.second(), use_24_hour_format));
      break;
  }
}

void clearDisplay() {
  for (int i = 0; i < MAX_DEVICES; i++) {
    lc.clearDisplay(i);
  }
}

void print_matrix(int **matrix) {
  for (int i = 0; i < ROWS; i++) {
    int k = MAX_DEVICES - 1, j = 0;
    while (j < COLS) {
      lc.setLed(k, i, j % ROWS, matrix[i][j++]);
      if (j % ROWS == 0)
        k--;
    }
  }
  for (int i = 0; i < ROWS; i++) {
    free(matrix[i]);
  }
  free(matrix);
}

String getMatrixConfig() {
  JSONVar config;
  config["displayMode"] = String(display_mode);
  config["use24HourFormat"] = String(use_24_hour_format);
  config["scrollingSpeed"] = String(scrolling_speed);
  config["messageHoldTime"] = String(message_hold_time);
  config["scrollingMessage"] = String(scrolling_message);
  return JSON.stringify(config);
}

void notifyClients() {
  Serial.println("Notifying clients...");
  ws.textAll(getMatrixConfig());
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      notifyClients();
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  String message = "";
  int data_size = 0;
  bool changed = false;
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data_size = sizeof(data) / sizeof(data[0]);
    for (int i = 0; i < len; i++) {
      message += char(data[i]);
    }
    Serial.println(message);
    String first_field = message.substring(0, 1);
    String second_field = message.substring(2, len);
    int opcode = first_field.toInt();
    int number;
    switch (opcode) {
      case MODE:
        Serial.println("Setting up display mode");
        number = second_field.toInt();
        display_mode = number;
        changed = true;
        break;
      case SET_TIME:
        Serial.println("Setting up Time");
        Serial.println(second_field);
        // set the time and set it
        break;
      case SET_DATE:
        Serial.println("Setting up Date");
        Serial.println(second_field);
        // set the date and set it
        break;
      case SET_DATE_FORMAT:
        Serial.println("Setting up Date Format");
        number = second_field.toInt();
        use_24_hour_format = number;
        changed = true;
        break;
      case SET_ANIMATION_SPEED:
        number = second_field.toInt();
        if (scrolling_speed != number) {
          Serial.println("Setting up Scrolling speed");
          scrolling_speed = number;
          display.setSpeed(scrolling_speed);
          changed = true;
        }
        break;
      case SET_STAY_TIME:
        number = second_field.toInt();
        if (message_hold_time != number) {
          Serial.println("Setting up Message  hold time");
          message_hold_time = number;
          changed = true;
        }
        break;
      case SET_TEXT:
        Serial.println("Setting up scrolling message");
        second_field.toCharArray(scrolling_message, second_field.length() + 1);
        display.displayClear();
        break;
    }
    if (changed) {
      notifyClients();
      displayModeChange();
    }
  }
}

void displayModeChange() {
  switch (display_mode) {
    case DISPLAY_TIME:
      break;
    case DISPLAY_DATE:
      break;
    case DISPLAY_TEMPERATURE:
      break;
    case DISPLAY_HUMIDITY:
      break;
    case DISPLAY_SCEOLLING_TEXT:
      display.setSpeed(SCROLLING_SPEED);
      display.displayText(scrolling_message, PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      break;
    case DISPLAY_LOOP:
      break;
  }
}
