#include <Adafruit_Sensor.h>  //humidity/temp sensor libraries
#include <Adafruit_AM2320.h>
#include <Wire.h>  //display libraries
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <NTPClient.h>  //wifi libraries
#include <WiFi.h>
#include <WiFiUdp.h>

#include "OpenWeatherMapCurrent.h"  //weather libraries
#include "OpenWeatherMapForecast.h"
#include <JsonListener.h>
#include <time.h>
#include <ESP32Time.h>

#include "soc/soc.h"  //disable brownout detector libraries
#include "soc/rtc_cntl_reg.h"

#include <initializer_list>  //Library to pass array elements directly to functions

OpenWeatherMapCurrent client;
OpenWeatherMapForecast client2;
WiFiUDP ntpUDP;
WiFiClient wifiClient;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 60);
char weekDay[7][6] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
char monthName[12][6] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

uint8_t MAX_FORECASTS = 19;

//////      CHANGE THESE      //////
const char* WIFI_SSID = "T";  //Replace with your wifi credentials
const char* WIFI_PASSWORD = "12345678";

String OPEN_WEATHER_MAP_LANGUAGE = "en";
String OPEN_WEATHER_MAP_APP_ID = "a754b7b33d8e1aca1b0e14ce8ff422f2";
String OPEN_WEATHER_MAP_LOCATION_ID = "2641544";

const int R1 = 105;  //voltage divider resistor values
const int R2 = 215;


//button pin declaration
const int Btn1 = 2;
const int Btn2 = 15;
const int Btn3 = 13;
const int y_pixels = 64;
const int x_pixels = 128;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)


Adafruit_SSD1306 display(128, 64, &Wire, -1);


//   BLUETOOTH INITIALISATION    //
#include "BluetoothSerial.h"


String device_name = "Marcel smartwatch";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

#include <BleKeyboard.h>

BleKeyboard bleKeyboard("Marcel smartwatch");



#include <Fonts/FreeSerif9pt7b.h>
// 'reactionstar', 128x32px
const unsigned char Star[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x1f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x84, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x60, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x80, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 528)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
  Star
};


Adafruit_AM2320 AM2320 = Adafruit_AM2320();
bool change = false;
RTC_DATA_ATTR int Mode = 3;
unsigned long pauseTime = 0;
int previousButtonState = 0;
int i = 0;
bool paused = true;
unsigned long Start, finished, elapsed;


bool btnState = false;
byte lastButtonState[] = { LOW, LOW, LOW };
unsigned long debounceDuration = 20;  // millis
unsigned long lastTimeButtonStateChanged = 0;

//reaction game variables
int displayDelay = 1000;
int score = 1;
bool reactionReady = true;
unsigned long reactionReadyMillis = 0;
int maxReactionTime = 2000;
int userReactionTime = 0;
bool gameRunning = false;
int totalReaction = 0;
float tempC;
float humidity;

//Time start Settings:

int Seconds = 1;
int prevSeconds = 0;
int Min = 33;


int weatherMode = 0;  //weather variables
unsigned long wifiMillis;
bool wifiToggle;
uint8_t foundForecasts = 0;
RTC_DATA_ATTR float temp[20];  //weather data storage
RTC_DATA_ATTR float feelsLike[20];
RTC_DATA_ATTR float windSpeed[20];
RTC_DATA_ATTR int pressure[20];
RTC_DATA_ATTR int hum[20];
RTC_DATA_ATTR String description[20];
RTC_DATA_ATTR String observationTime[20];
int timeLength;
String beforeTime;
String concatString;
int concatTime;


//   PONG GAME VARIABLES   //
// Ball velocities:
int x_vel = 1;
int y_vel = 1;

// Ball position:
int x_pos = 5;
int y_pos = 32;

// Paddle positions:
int l_pos = 0;
int r_pos = 0;

// Player scores
int l_score = 0;
int r_score = 0;

//Paddle Parameters
int paddle_height = 6;
int paddle_width = 2;


//bluetooth variables
bool bluetoothEnabled = false;
bool bluetoothMusicEnabled = false;
int state;
RTC_DATA_ATTR char message[168] = "Please connect to    Bluetooth";
RTC_DATA_ATTR int messageLen = 30;


bool flashLightOn = false;

RTC_DATA_ATTR int menuPos[] = { 0, 0 };

float batteryVoltage;

ESP32Time rtc(0);

void callback() {}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  //disable brownout detector


  change = true;
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //Display initialisation
  //display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  touchAttachInterrupt(T7, callback, 40);
  touchAttachInterrupt(T5, callback, 40);
  esp_sleep_enable_touchpad_wakeup();

  if (touchRead(12) < 40) {
    display.setTextSize(2);
    display.setFont(&FreeSerif9pt7b);
    while (touchRead(12) < 40) {
      timePeek();
    }
    delay(500);
    display.setTextSize(1);
    display.setFont();
    display.clearDisplay();
    display.display();
    esp_deep_sleep_start();
  } else if (touchRead(14) < 40) {

  } else {
    WiFi.mode(WIFI_STA);
    timeClient.begin();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    wifiMillis = millis();
    wifiToggle = true;
  }

  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(Btn3, INPUT_PULLUP);
  pinMode(Btn2, INPUT_PULLUP);  // BTN2
  pinMode(Btn1, INPUT_PULLUP);  // BTN1
  display.clearDisplay();
}



void timePeek() {
  // Check if the time has changed or it's the initial display
  if ((rtc.getSecond()) != prevSeconds) {
    // Clear the previous time display
    display.clearDisplay();

    // Set the cursor position and font for displaying the time
    display.setCursor(0, 40);

    // Display hours
    display.print(rtc.getHour());
    display.print(":");

    // Display minutes
    if ((rtc.getMinute()) < 10) {
      display.print("0" + String(rtc.getMinute()));
    } else {
      display.print(rtc.getMinute());
    }
    display.print(":");

    // Display seconds
    if ((rtc.getSecond()) < 10) {
      display.print("0" + String((rtc.getSecond())));
    } else {
      display.print((rtc.getSecond()));
    }

    // Update the previous seconds
    prevSeconds = (rtc.getSecond());

    // Update the display
    display.display();
  }
}

bool displayTime() {
  // Check if the time has changed or it's the initial display
  if (((rtc.getSecond()) != prevSeconds) || (change)) {
    // Clear the previous time display
    display.fillRect(0, 0, 128, 15, SSD1306_BLACK);

    // Set the cursor position and font for displaying the time
    display.setCursor(30, 7);
    display.setFont(&FreeSerif9pt7b);

    // Display hours
    display.print(rtc.getHour());
    display.print(":");

    // Display minutes
    if ((rtc.getMinute()) < 10) {
      display.print("0" + String(rtc.getMinute()));
    } else {
      display.print(rtc.getMinute());
    }
    display.print(":");

    // Display seconds
    if ((rtc.getSecond()) < 10) {
      display.print("0" + String((rtc.getSecond())));
    } else {
      display.print((rtc.getSecond()));
    }

    // Reset the font and set the cursor position for additional information
    display.setFont();
    display.setCursor(0, 16);

    // Update the previous seconds
    prevSeconds = (rtc.getSecond());

    // Update the display
    display.display();

    return true;
  } else {
    return false;
  }
}

void displayResult() {


  float h, m, s, ms;
  unsigned long over;
  if (change == false) {
    elapsed = millis() - Start;
  }
  h = int(elapsed / 3600000);
  over = elapsed % 3600000;
  m = int(over / 60000);
  over = over % 60000;
  s = int(over / 1000);
  ms = over % 1000;
  display.fillRect(0, 19, 128, 64, SSD1306_BLACK);
  display.setCursor(0, 20);
  //Serial.print ("Raw elapsed time: ");
  //Serial.println (elapsed);
  //Serial.print ("Elapsed time: ");
  display.print(h, 0);
  display.print("h ");
  display.print(m, 0);
  display.print("m ");
  display.print(s, 0);
  display.print("s ");
  display.print(ms, 0);
  display.println("ms ");
  display.println("Stopwatch");
  display.display();
  change = false;
}

void stopwatch() {
  displayTime();

  if (change) {
    elapsed = pauseTime;
    displayResult();
  }

  if (!paused) {
    displayResult();
  }

  if (btnCheck(Btn1, 0)) {
    // Pause/unpause
    paused = !paused;
    if (paused) {
      pauseTime = elapsed;
    } else {
      Start = millis() - pauseTime;
    }
  }

  if (btnCheck(Btn3, 2)) {
    Start = millis();  // reset
    pauseTime = 0;
    displayResult();
    paused = true;
  }
}

void pongGame() {
  if (change) {
    gameRunning = false;
    display.setCursor(0, 0);
    display.clearDisplay();
    display.println("PONG");
    display.print("Paddle width: ");
    display.print(paddle_height);
    display.print(" px");
    display.display();
  }

  if (gameRunning == true) {
    // Update position of left paddle:
    if ((touchRead(27) < 40) && l_pos < (y_pixels - paddle_height)) {
      l_pos += 1;
    } else if (l_pos > 0) {
      l_pos -= 1;
    }

    // Update position of right paddle:
    if ((touchRead(12) < 40) && r_pos < (y_pixels - paddle_height)) {
      r_pos += 1;
    } else if (r_pos > 0) {
      r_pos -= 1;
    }

    // Check for ball hitting a wall:

    if (x_pos > x_pixels - 1) {
      ball_reset(false);
      l_score += 1;
    }

    else if (x_pos < 0) {
      ball_reset(true);
      r_score += 1;
    }

    // Check for ball bouncing off ceiling:

    if (y_pos > y_pixels - 1 || y_pos < 0) {
      y_vel = -y_vel;
    }

    // Check for ball bouncing off paddle:

    // Update ball position:
    x_pos += x_vel;
    y_pos += y_vel;

    // Draw pong elements to display:
    display.clearDisplay();
    display.drawPixel(x_pos, y_pos, WHITE);
    display.fillRect(0, l_pos, paddle_width, paddle_height, WHITE);
    display.fillRect(x_pixels - paddle_width, r_pos, paddle_width, paddle_height, WHITE);

    // Display scores
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(display.width() / 4, 0);
    display.println(l_score);
    display.setCursor(display.width() * 3 / 4, 0);
    display.println(r_score);
    // Display all elements
    display.display();

    // Check for ball bouncing off paddles:
    if (ball_on_right_paddle()) {
      x_vel = -x_vel;
    } else if (ball_on_left_paddle()) {
      x_vel = -x_vel;
    }
  } else {
    if (digitalRead(Btn3) == LOW) {
      gameRunning = true;
      l_pos = 0;
      r_pos = 0;
      l_score = 0;
      r_score = 0;

      ball_reset(true);
    }
    if (btnCheck(Btn1, 0) == true) {
      paddle_height = paddle_height + 1;
      if (paddle_height > 15) {
        paddle_height = 1;
      }
      display.setCursor(0, 0);
      display.clearDisplay();
      display.println("PONG");
      display.print("Paddle width: ");
      display.print(paddle_height);
      display.print(" px");
      display.display();
      btnState = false;
    }
  }
}

void reactionGame() {
  if (change) {
    score = 1;
    reactionReady = true;
    gameRunning = false;
  }

  displayTime();

  if (gameRunning) {
    if ((millis() - displayDelay) > reactionReadyMillis && !reactionReady) {
      reactionReadyMillis = millis();
      reactionReady = true;
      display.drawBitmap(0, 15, Star, 128, 32, WHITE);
      display.display();
      maxReactionTime *= 0.9;
      displayDelay = random(800, 1800);
    }

    if (btnCheck(Btn1, 0)) {
      display.fillRect(0, 19, 128, 64, SSD1306_BLACK);
      userReactionTime = millis() - reactionReadyMillis;

      if (userReactionTime > maxReactionTime || !reactionReady) {
        display.print("Game over!");
        display.display();
        reactionReady = true;
        gameRunning = false;
        yield();
        delay(1000);
      } else {
        score++;
        display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
        display.setFont(&FreeSerif9pt7b);
        display.setCursor(5, 35);
        display.print(score);
        display.display();
        display.setFont();
        //gameRunning = false;
        reactionReady = false;
        totalReaction += userReactionTime;
      }
      reactionReadyMillis = millis();
    }
  } else if (reactionReady) {
    //Serial.println("Press button 1 to Start!");
    display.fillRect(0, 19, 128, 64, SSD1306_BLACK);
    display.setCursor(0, 20);
    display.print("Score:");
    display.println(score);
    display.print("Reaction time:");
    display.print(totalReaction / score);
    display.print("ms");
    display.display();
    reactionReady = false;
  } else {
    if (digitalRead(Btn1) == LOW) {
      gameRunning = true;
      reactionReady = false;
      maxReactionTime = 2000;
      score = 0;
      btnState = false;
      totalReaction = 0;
      display.clearDisplay();
      display.setFont(&FreeSerif9pt7b);
      display.setCursor(5, 35);
      display.print(score);
      display.display();
      display.setFont();
    }
  }
}

void humidityTemp() {


  if (btnCheck(Btn1, 0)) {
    weatherMode = weatherMode + 1;
    if (weatherMode > foundForecasts) {
      weatherMode = 0;
    }
    change = true;
  }
  if (btnCheck(Btn3, 2)) {
    weatherMode = weatherMode - 1;
    if (weatherMode < 0) {
      weatherMode = foundForecasts;
    }
    change = true;
  } else if (weatherMode == 0) {
    displayTime();
  }
  if (change == true) {
    if (weatherMode == 0) {
      //tempC = AM2320.readTemperature();
      //humidity = AM2320.readHumidity();

      // Print the temperature and humidity in the Serial Monitor:
      display.clearDisplay();
      displayTime();
      display.println(String(weekDay[rtc.getDayofWeek()]) + ", " + String(rtc.getDay()) + " " + String(monthName[rtc.getMonth() - 1]) + ", " + String((rtc.getYear())));
      display.print("Temperature:");
      display.print(tempC);
      display.println("C");
      display.print("Humidity:");
      display.print(humidity);
      display.println("%RH");
      display.println("SENSOR DATA");
      display.display();
    } else {
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print(weatherMode);
      display.print("|");
      display.println(String(observationTime[weatherMode - 1]));
      display.println(description[weatherMode - 1]);
      display.print(("Temp:") + String(temp[weatherMode - 1]));
      display.println((" Hum:") + String(hum[weatherMode - 1]));
      display.println(("Wind: ") + String(windSpeed[weatherMode - 1]));
      display.println(("Pressure: ") + String(pressure[weatherMode - 1]));
      display.display();
    }
    change = false;
  }
}

void flashLight() {


  if (flashLightOn == false) {
    displayTime();
  }
  if (change == true) {
    flashLightOn = false;
    display.fillRect(0, 15, 128, 64, SSD1306_BLACK);
    display.setCursor(0, 16);
    display.println("Press btn1 to");
    display.print("toggle flashlight");
    display.display();
    change = false;
  }

  if (btnCheck(Btn1, 0) == true) {
    flashLightOn = !flashLightOn;
    if (flashLightOn == true) {
      display.fillRect(0, 0, 128, 64, SSD1306_WHITE);
      display.display();
    } else {
      change = true;
    }
  }
}

void blueTooth() {
  if (change) {
    SerialBT.begin(device_name);
    bluetoothEnabled = true;
    if (messageLen > 0) {
      // State 2: Displaying the existing message again
      display.clearDisplay();
      display.setCursor(0, 0);
      for (i = 0; i < messageLen; i++) {
        display.print(message[i]);
      }
      display.display();
    }
  }
  // Check if there is data available from bluetooth
  if (SerialBT.available() > 0) {
    state = SerialBT.read();
    Serial.println(state);

    // Handle different states
    if (state == 1) {
      // State 1: Receiving a new message
      i = 0;
      while ((SerialBT.available() > 0) && (i < 168)) {
        Serial.println(i);
        message[i++] = char(SerialBT.read());
      }
      messageLen = i;
      message[i] = '\0';

      // Display the message on the screen
      display.clearDisplay();
      display.setCursor(0, 0);
      for (i = 0; i < messageLen; i++) {
        display.print(message[i]);
      }
      display.display();
    } else if (state == 2) {
      // State 2: Displaying the existing message again
      display.clearDisplay();
      display.setCursor(0, 0);
      for (i = 0; i < messageLen; i++) {
        display.print(message[i]);
      }
      display.display();
    } else if (state == 3) {
      // State 3: Clearing the display
      display.clearDisplay();
      display.display();
    }
  }
}

void BLE() {
  if (change) {
    if (bluetoothMusicEnabled == false) {
      bleKeyboard.begin();
      bluetoothMusicEnabled = true;
    }
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("PREVIOUS TRACK");
    display.setCursor(0, 24);
    display.print("Bottom left touch to play/pause");
    display.setCursor(0, 56);
    display.print("NEXT TRACK");
    display.display();
    change = false;
  }
  if (bleKeyboard.isConnected()) {
    if (btnCheck(Btn1, 0)) {
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    } else if (btnCheck(Btn3, 2)) {
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
    } else if (touchRead(27) < 40) {
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
      while (touchRead(27) < 40) {}
    }
  }
}

void Battery() {

  if (displayTime()) {

    display.fillRect(0, 15, 128, 64, SSD1306_BLACK);

    batteryVoltage = (analogRead(36) * 3.3 / 4095);
    display.println("Raw voltage:");
    display.print(batteryVoltage);
    display.println("V");

    batteryVoltage = ((batteryVoltage * (R1 + R2) / R2));
    display.println("Battery voltage:");
    display.print(batteryVoltage);
    display.println("V");

    batteryVoltage = 123.0 - 123.0 / pow((1.0 + pow((batteryVoltage / 3.7), 80.0)), 0.165);
    display.println("Battery percent:");
    display.print(batteryVoltage);

    display.display();
  }
}

void menuDisplay() {
  if (menuPos[0] == 0) {
    menuDisplay2({ "Stopwatch", "Reaction game", "Flashlight", "Weather", "Pong game", "Bluetooth text", "Bluetooth music", "Battery" });
  }
}

void menuDisplay2(std::initializer_list<const char*> words) {

  i = 0;
  display.clearDisplay();
  display.fillRect(0, (8 * menuPos[1]), 2, 7, WHITE);

  // Display static text
  for (const char* word : words) {
    display.setCursor(3, (i * 8));
    display.print(word);
    i++;
  }
  /**display.setCursor(3, 0);
  display.print("Stopwatch");
  display.setCursor(3, 8);
  display.print("Reaction game");
  display.setCursor(3, 16);
  display.print("Flashlight");
  display.setCursor(3, 24);
  display.print("Weather");
  display.setCursor(3, 32);
  display.print("Pong game");
  display.setCursor(3, 40);
  display.print("Bluetooth text");
  display.setCursor(3, 48);
  display.print("Bluetooth media");
  display.setCursor(3,56);
  display.print("Battery") **/
  display.display();
}

void loop() {
  if ((millis() - lastTimeButtonStateChanged > 20000) && (Mode != 0) && (Mode != 4) && (Mode != 5)) {  //ENTER DEEP SLEEP
    display.clearDisplay();
    display.display();
    esp_deep_sleep_start();
  } else {

    //BLE media control timeout

    //running different modes
    if (Mode == 0) {
      stopwatch();
    } else if (Mode == 1) {
      reactionGame();
    } else if (Mode == 2) {
      flashLight();
    } else if (Mode == 3) {
      humidityTemp();
    } else if (Mode == 4) {
      pongGame();
    } else if (Mode == 5) {
      blueTooth();
    } else if (Mode == 6) {
      BLE();
    } else if (Mode == 7) {
      Battery();
    } else if (Mode == 8) {

      //Menu mode

      if (change) {
        menuDisplay();
      }
      if (btnCheck(Btn1, 0) == true) {
        menuPos[1] = menuPos[1] + 1;
        if (menuPos[1] > 7) {
          menuPos[1] = 0;
        }
        menuDisplay();
      }
      if (btnCheck(Btn3, 2) == true) {
        menuPos[1] = menuPos[1] - 1;
        if (menuPos[1] < 0) {
          menuPos[1] = 7;
        }
        menuDisplay();
      }
      if (btnCheck(Btn2, 1)) {  //Choose mode

        Mode = menuPos[1] + (menuPos[0] * 10);

        change = true;
        if (Mode == 0) {         //Stopwatch
        } else if (Mode == 1) {  //Reaction game
        } else if (Mode == 2) {  //Flashlight
        } else if (Mode == 3) {  //HumidityTemp
        } else if (Mode == 4) {  //Pong game
        } else if (Mode == 5) {  //Bluetooth text
        } else if (Mode == 6) {  //Bluetooth media control
        } else if (Mode == 7) {  //Battery
        }
      }
    }
    if (btnCheck(Btn2, 1)) { //Enter menu mode
      if (bluetoothEnabled) {
        SerialBT.end();
        bluetoothEnabled = false;
      }

      menuDisplay();
      Mode = 8;
    }

    if ((wifiToggle == true) && ((millis() - wifiMillis) < 10000)) {  //WIFI CONNECTION
      if (WiFi.status() == WL_CONNECTED) {

        timeClient.update();
        Min = (timeClient.getMinutes());
        Seconds = (timeClient.getSeconds());
        time_t epochTime = timeClient.getEpochTime();
        struct tm* ptm = gmtime((time_t*)&epochTime);

        rtc.setTime(Seconds, Min, (timeClient.getHours() + 1), (ptm->tm_mday), (ptm->tm_mon + 1), (ptm->tm_year + 1900));

        OpenWeatherMapCurrentData data;
        client.setLanguage(OPEN_WEATHER_MAP_LANGUAGE);
        client.setMetric(true);
        client.updateCurrentById(&data, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID);
        time_t time;
        //Print complete date:
        //String currentDate = String(monthDay) + "-" + String(currentMonth) + "-" + String(currentYear);
        description[0] = (data.description.c_str());
        // "icon": "09d" String icon; String iconMeteoCon;
        // "temp": 290.56, float temp;
        temp[0] = (data.temp);
        // "pressure": 1013, uint16_t pressure;
        pressure[0] = (data.pressure);
        // "humidity": 87, uint8_t humidity;
        hum[0] = ("humidity: %d\n", data.humidity);
        // "temp_min": 289.15, float tempMin;
        //Serial.printf("tempMin: %f\n", data.tempMin);
        // "temp_max": 292.15 float tempMax;
        //Serial.printf("tempMax: %f\n", data.tempMax);
        // "wind": {"speed": 1.5}, float windSpeed;
        windSpeed[0] = (data.windSpeed);

        time = data.observationTime;
        beforeTime = String(ctime(&time));
        timeLength = beforeTime.length();
        if (String(beforeTime[timeLength - 14]) == "0") {
          concatString = beforeTime[timeLength - 13];
        } else {
          concatString = String(beforeTime[timeLength - 14]) + String(beforeTime[timeLength - 13]);
        }
        concatTime = (concatString.toInt());
        //for (int i2 = 1; i2 < 15; i2++) {
        //beforeTime[timeLength - i2] = '\0';
        //}
        beforeTime.remove(timeLength - 14);  //Remove 6 characters starting from position 3
        if (concatTime > 12) {
          observationTime[0] = ((beforeTime) + String(concatTime - 12) + ("PM"));
        } else {
          observationTime[0] = ((beforeTime) + String(concatTime) + ("AM"));
        }
        OpenWeatherMapForecastData data2[MAX_FORECASTS];
        uint8_t allowedHours[] = { 6, 12, 18 };
        client2.setAllowedHours(allowedHours, 4);
        foundForecasts = client2.updateForecastsById(data2, OPEN_WEATHER_MAP_APP_ID, OPEN_WEATHER_MAP_LOCATION_ID, MAX_FORECASTS);
        for (i = 0; i < foundForecasts; i++) {
          yield();
          temp[i + 1] = (data2[i].temp);
          pressure[i + 1] = (data2[i].pressure);
          hum[i + 1] = (data2[i].humidity);
          description[i + 1] = (data2[i].description.c_str());
          windSpeed[i + 1] = (data2[i].windSpeed);
          time = data2[i].observationTime;
          beforeTime = String(ctime(&time));
          timeLength = beforeTime.length();
          if (String(beforeTime[timeLength - 14]) == "0") {
            concatString = beforeTime[timeLength - 13];
          } else {
            concatString = String(beforeTime[timeLength - 14]) + String(beforeTime[timeLength - 13]);
          }
          concatTime = (concatString.toInt());
          beforeTime.remove(timeLength - 14);  //Remove 6 characters starting from position 3
          if (concatTime > 12) {
            observationTime[i + 1] = ((beforeTime) + String(concatTime - 12) + ("PM"));
          } else if (concatTime == 12) {
            observationTime[i + 1] = ((beforeTime) + String(concatTime) + ("PM"));
          } else {
            observationTime[i + 1] = ((beforeTime) + String(concatTime) + ("AM"));
          }
        }
        wifiToggle = false;
        WiFi.disconnect();
        WiFi.mode(WIFI_OFF);
      }
    }
  }
}




bool btnCheck(int x, int y) {


  bool btnState = false;
  if (millis() - lastTimeButtonStateChanged > debounceDuration) {
    byte buttonState = (digitalRead(x));
    if (buttonState != lastButtonState[y]) {
      lastTimeButtonStateChanged = millis();
      lastButtonState[y] = buttonState;
      if (buttonState == LOW) {
        // do an action, for example print on Serial
        btnState = true;
      }
    }
  }
  return btnState;
}

/////////////////PONG GAME FUNCTIONS////////////////////
bool ball_on_right_paddle() {
  // If ball is heading towards paddle and is at the surface of paddle between the top and bottom of the paddle, then it's a hit
  return (x_pos == 128 - paddle_width - 1 && y_pos >= r_pos && y_pos <= (r_pos + paddle_height) && x_vel == 1);
}

bool ball_on_left_paddle() {
  return (x_pos == paddle_width - 1 && y_pos >= l_pos && y_pos <= (l_pos + paddle_height) && x_vel == -1);
}

void ball_reset(bool left) {
  //If left is true, then ball should launch from the left, otherwise launch from the right
  //Ball should launch at a random Y location and at a random Y velocity

  y_pos = random(display.height());
  if (random(2) > 0) {
    y_vel = 1;
  } else {
    y_vel = -1;
  }

  if (left) {
    x_vel = 1;
    x_pos = paddle_width - 1;
  }

  else {
    x_vel = -1;
    x_pos = display.width() - paddle_width;
  }
}