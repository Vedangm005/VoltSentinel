#include "VoltSentinel_AI.h"

#include <Wire.h>

#include <string.h>

#include <U8g2lib.h>

#include <Adafruit_INA219.h>

#include <OneWire.h>

#include <DallasTemperature.h>

#include <Adafruit_NeoPixel.h>

#include <WiFi.h>

#include <WebServer.h>

#include <ESPmDNS.h>

#include "Dashboard.h"

// =====================================================

// VOLTSENTINEL

// Battery Monitoring + Trend Analysis

// Monitor Mode + Data Collection Mode

// =====================================================

// =====================================================

// PIN CONFIGURATION

// =====================================================

#define SDA_PIN 8

#define SCL_PIN 9

#define DS18B20_PIN 4

#define BUTTON_PIN 5

#define SWITCH_PIN 6

#define RGB_PIN 48

// =====================================================

// BUZZER

// =====================================================

//

// Buzzer is NOT connected yet.

// GPIO 7 is reserved for future buzzer use.

// =====================================================

#define BUZZER_ENABLED false

#define BUZZER_PIN 7

// =====================================================

// OBJECTS

// =====================================================

U8G2_SH1106_128X64_NONAME_F_HW_I2C display(

  U8G2_R0,

  U8X8_PIN_NONE

);

Adafruit_INA219 ina219;

OneWire oneWire(

  DS18B20_PIN

);

DallasTemperature tempSensor(

  &oneWire

);

Adafruit_NeoPixel rgb(

  1,

  RGB_PIN,

  NEO_GRB + NEO_KHZ800

);

// =====================================================

// MONITORING THRESHOLDS

// =====================================================

//

// IMPORTANT:

//

// These are preliminary monitoring thresholds.

// They are NOT being claimed as the exact limits

// of your particular 18650.

//

// We deliberately do NOT use 4.15 V as WARNING,

// because charging can legitimately raise the

// terminal voltage close to the normal full-charge

// region.

//

// We will finalize battery-specific limits after

// identifying the exact cell.

// =====================================================

const float LOW_VOLTAGE_WARNING = 3.60;

const float CRITICAL_LOW_VOLTAGE = 3.00;

const float CRITICAL_HIGH_VOLTAGE = 4.20;

const float TEMP_WARNING = 45.0;

const float TEMP_CRITICAL = 50.0;

// =====================================================

// SENSOR VARIABLES

// =====================================================

float voltage = 0.0;

float current = 0.0;

float power = 0.0;

float temperature = 0.0;

// =====================================================

// BEHAVIOR / AI FEATURES

// =====================================================

float voltageSlope = 0.0;

float currentSlope = 0.0;

float temperatureSlope = 0.0;

// AI health estimate (shown only when valid input data is available)
float aiSOH = 0.0f;
bool aiEstimateAvailable = false;

// Dataset-only inference demo; never represents the connected cell's SOH.
bool aiDemoRun = false;

float previousVoltage = 0.0;

float previousCurrent = 0.0;

float previousTemperature = 0.0;

unsigned long previousFeatureTime = 0;

// =====================================================
// AI FEATURE COLLECTOR — EXPERIMENTAL
// =====================================================

const int AI_MAX_SAMPLES = 300;

float aiVoltageSamples[AI_MAX_SAMPLES];
float aiCurrentSamples[AI_MAX_SAMPLES];
float aiTemperatureSamples[AI_MAX_SAMPLES];

int aiSampleCount = 0;

unsigned long aiLastSampleTime = 0;
const unsigned long AI_SAMPLE_INTERVAL = 1000;

bool aiCollectionReady = false;


// =====================================================

// SAFETY STATE

// =====================================================

enum SafetyState {

  SAFETY_NORMAL,

  SAFETY_WARNING,

  SAFETY_CRITICAL

};

SafetyState safetyState = SAFETY_NORMAL;

// =====================================================

// SYSTEM MODE

// =====================================================

enum SystemMode {

  MODE_MONITOR,

  MODE_DATA

};

SystemMode systemMode = MODE_MONITOR;

// =====================================================

// TIMING

// =====================================================

unsigned long lastSensorUpdate = 0;

unsigned long lastDisplayUpdate = 0;

unsigned long lastButtonTime = 0;

const unsigned long SENSOR_INTERVAL = 1000;

const unsigned long DISPLAY_INTERVAL = 200;

// =====================================================

// BUTTON

// =====================================================

bool lastButtonState = HIGH;

// =====================================================

// OLED SCREENS

// =====================================================

//

// 0 = Live monitoring

// 1 = Voltage graph

// 2 = Temperature graph

// 3 = System analysis

//

// =====================================================

int screen = 0;

const int TOTAL_SCREENS = 5;

// =====================================================

// RECENT GRAPH HISTORY

// =====================================================

//

// 30 measurements

// One measurement per second

// Approximately 30 seconds

// =====================================================

#define RECENT_POINTS 30

float recentVoltage[RECENT_POINTS];

float recentTemperature[RECENT_POINTS];

int recentIndex = 0;

bool recentFilled = false;

// =====================================================

// LONG-TERM HISTORY

// =====================================================

//

// One point every 10 seconds

//

// 30 points × 10 seconds

// = approximately 5 minutes

// =====================================================

#define LONG_POINTS 30

float longVoltage[LONG_POINTS];

float longTemperature[LONG_POINTS];

int longIndex = 0;

bool longFilled = false;

unsigned long lastLongSample = 0;

const unsigned long LONG_SAMPLE_INTERVAL = 10000;

// =====================================================

// RGB CONTROL

// =====================================================

void setRGB(

  uint8_t r,

  uint8_t g,

  uint8_t b

) {

  rgb.setPixelColor(

    0,

    rgb.Color(r, g, b)

  );

  rgb.show();
}

// =====================================================

// RGB OFF

// =====================================================

void rgbOff() {

  rgb.clear();

  rgb.show();
}

// =====================================================

// UPDATE RGB SYSTEM STATUS

// =====================================================

//

// Priority:

//

// CRITICAL → RED

// WARNING  → ORANGE

// DATA     → BLUE

// MONITOR  → GREEN

//

// =====================================================

void updateRGB() {

  // Critical always has highest priority

  if (

    safetyState ==

    SAFETY_CRITICAL

  ) {

    setRGB(

      255,

      0,

      0

    );

    return;
  }

  // Warning

  if (

    safetyState ==

    SAFETY_WARNING

  ) {

    setRGB(

      255,

      100,

      0

    );

    return;
  }

  // Data collection

  if (

    systemMode ==

    MODE_DATA

  ) {

    setRGB(

      0,

      0,

      255

    );

    return;
  }

  // Normal monitor

  setRGB(

    0,

    255,

    0

  );
}

// =====================================================

// BUZZER

// =====================================================

void buzzerOff() {

  if (

    BUZZER_ENABLED

  ) {

    digitalWrite(

      BUZZER_PIN,

      LOW

    );
  }
}

void buzzerCritical() {

  if (

    !BUZZER_ENABLED

  ) {

    return;
  }

  static unsigned long lastBeep = 0;

  static bool state = false;

  if (

    millis() -

      lastBeep
    >=

    500

  ) {

    lastBeep =

      millis();

    state =

      !state;

    digitalWrite(

      BUZZER_PIN,

      state

        ? HIGH

        : LOW

    );
  }
}

// =====================================================

// UPDATE RECENT HISTORY

// =====================================================

void updateRecentHistory() {

  recentVoltage[

    recentIndex

  ] = voltage;

  recentTemperature[

    recentIndex

  ] = temperature;

  recentIndex++;

  if (

    recentIndex >=

    RECENT_POINTS

  ) {

    recentIndex = 0;

    recentFilled = true;
  }
}

// =====================================================

// UPDATE LONG-TERM HISTORY

// =====================================================

void updateLongHistory() {

  if (

    millis() -

      lastLongSample
    <

    LONG_SAMPLE_INTERVAL

  ) {

    return;
  }

  lastLongSample =

    millis();

  longVoltage[

    longIndex

  ] = voltage;

  longTemperature[

    longIndex

  ] = temperature;

  longIndex++;

  if (

    longIndex >=

    LONG_POINTS

  ) {

    longIndex = 0;

    longFilled = true;
  }
}

// =====================================================

// SAFETY ANALYSIS

// =====================================================

void analyzeSafety() {

  safetyState =

    SAFETY_NORMAL;

  // ---------------------------------------------------

  // TEMPERATURE

  // ---------------------------------------------------

  if (

    temperature >=

    TEMP_CRITICAL

  ) {

    safetyState =

      SAFETY_CRITICAL;

  }

  else if (

    temperature >=

    TEMP_WARNING

  ) {

    safetyState =

      SAFETY_WARNING;
  }

  // ---------------------------------------------------

  // LOW VOLTAGE

  // ---------------------------------------------------

  if (

    voltage <=

    CRITICAL_LOW_VOLTAGE

  ) {

    safetyState =

      SAFETY_CRITICAL;

  }

  else if (

    voltage <

    LOW_VOLTAGE_WARNING

  ) {

    if (

      safetyState !=

      SAFETY_CRITICAL

    ) {

      safetyState =

        SAFETY_WARNING;
    }
  }

  // ---------------------------------------------------

  // HIGH VOLTAGE

  // ---------------------------------------------------

  if (

    voltage >

    CRITICAL_HIGH_VOLTAGE

  ) {

    safetyState =

      SAFETY_CRITICAL;
  }

  // ---------------------------------------------------

  // UPDATE RGB

  // ---------------------------------------------------

  updateRGB();

  // ---------------------------------------------------

  // BUZZER

  // ---------------------------------------------------

  if (

    safetyState ==

    SAFETY_CRITICAL

  ) {

    buzzerCritical();

  }

  else {

    buzzerOff();
  }
}

// =====================================================

// SAFETY TEXT

// =====================================================

String getSafetyText() {

  if (

    safetyState ==

    SAFETY_NORMAL

  ) {

    return "NORMAL";
  }

  if (

    safetyState ==

    SAFETY_WARNING

  ) {

    return "WARNING";
  }

  return "CRITICAL";
}

// =====================================================

// MODE TEXT

// =====================================================

String getModeText() {

  if (

    systemMode ==

    MODE_DATA

  ) {

    return "DATA";
  }

  return "MONITOR";
}

// =====================================================

// VOLTAGE STATUS

// =====================================================

String getVoltageStatus() {

  if (

    voltage <=

    CRITICAL_LOW_VOLTAGE

  ) {

    return "CRITICAL";
  }

  if (

    voltage <

    LOW_VOLTAGE_WARNING

  ) {

    return "LOW";
  }

  if (

    voltage >

    CRITICAL_HIGH_VOLTAGE

  ) {

    return "CRITICAL";
  }

  return "NORMAL";
}

// =====================================================

// TEMPERATURE STATUS

// =====================================================

String getTemperatureStatus() {

  if (

    temperature >=

    TEMP_CRITICAL

  ) {

    return "CRITICAL";
  }

  if (

    temperature >=

    TEMP_WARNING

  ) {

    return "HIGH";
  }

  return "NORMAL";
}

// =====================================================

// VOLTAGE TREND

// =====================================================

String getVoltageTrend() {

  int count =

    recentFilled

      ? RECENT_POINTS

      : recentIndex;

  if (

    count < 5

  ) {

    return "WAIT";
  }

  int newest =

    (

      recentIndex -

      1 +

      RECENT_POINTS

      )
    %

    RECENT_POINTS;

  int older =

    (

      recentIndex -

      6 +

      RECENT_POINTS

      )
    %

    RECENT_POINTS;

  float difference =

    recentVoltage[newest] -

    recentVoltage[older];

  if (

    difference > 0.005

  ) {

    return "RISING";
  }

  if (

    difference < -0.005

  ) {

    return "FALLING";
  }

  return "STABLE";
}

// =====================================================

// TEMPERATURE TREND

// =====================================================

String getTemperatureTrend() {

  int count =

    recentFilled

      ? RECENT_POINTS

      : recentIndex;

  if (

    count < 5

  ) {

    return "WAIT";
  }

  int newest =

    (

      recentIndex -

      1 +

      RECENT_POINTS

      )
    %

    RECENT_POINTS;

  int older =

    (

      recentIndex -

      6 +

      RECENT_POINTS

      )
    %

    RECENT_POINTS;

  float difference =

    recentTemperature[newest] -

    recentTemperature[older];

  if (

    difference > 0.20

  ) {

    return "RISING";
  }

  if (

    difference < -0.20

  ) {

    return "FALLING";
  }

  return "STABLE";
}

// =====================================================

// CALCULATE BEHAVIOR FEATURES

// =====================================================

void calculateFeatures() {

  unsigned long now =

    millis();

  // First measurement

  if (

    previousFeatureTime ==

    0

  ) {

    previousVoltage =

      voltage;

    previousCurrent =

      current;

    previousTemperature =

      temperature;

    previousFeatureTime =

      now;

    voltageSlope =

      0.0;

    currentSlope =

      0.0;

    temperatureSlope =

      0.0;

    return;
  }

  float deltaTime =

    (

      now -

      previousFeatureTime

      )
    / 1000.0;

  if (

    deltaTime <=

    0

  ) {

    return;
  }

  voltageSlope =

    (

      voltage -

      previousVoltage

      )
    /

    deltaTime;

  currentSlope =

    (

      current -

      previousCurrent

      )
    /

    deltaTime;

  temperatureSlope =

    (

      temperature -

      previousTemperature

      )
    /

    deltaTime;

  previousVoltage =

    voltage;

  previousCurrent =

    current;

  previousTemperature =

    temperature;

  previousFeatureTime =

    now;
}



void collectAIData() {
  unsigned long now = millis();

  if (now - aiLastSampleTime < AI_SAMPLE_INTERVAL) {
    return;
  }

  aiLastSampleTime = now;

  if (aiSampleCount >= AI_MAX_SAMPLES) {
    if (!aiCollectionReady) {
      aiCollectionReady = true;
      Serial.println("AI data collection complete: 300 samples");
    }
    return;
  }

  if (!isfinite(voltage) || !isfinite(current) || !isfinite(temperature) || temperature == DEVICE_DISCONNECTED_C) {
    return;
  }

  aiVoltageSamples[aiSampleCount] = voltage;
  aiCurrentSamples[aiSampleCount] = current / 1000.0f;
  aiTemperatureSamples[aiSampleCount] = temperature;

  aiSampleCount++;

  if (aiSampleCount % 30 == 0) {
    Serial.print("AI samples collected: ");
    Serial.println(aiSampleCount);
  }

  if (aiSampleCount >= AI_MAX_SAMPLES) {
    aiCollectionReady = true;
    Serial.println("AI data collection complete: 300 samples");
  }
}


void dumpAIData() {
  if (!aiCollectionReady || aiSampleCount == 0) {
    Serial.println("ERROR: AI data collection is not complete.");
    Serial.print("Samples available: ");
    Serial.println(aiSampleCount);
    return;
  }

  Serial.println();
  Serial.println("=== VOLTSENTINEL CSV EXPORT ===");
  Serial.println("sample,voltage_V,current_A,temperature_C");

  for (int i = 0; i < aiSampleCount; i++) {
    Serial.print(i + 1);
    Serial.print(",");
    Serial.print(aiVoltageSamples[i], 4);
    Serial.print(",");
    Serial.print(aiCurrentSamples[i], 6);
    Serial.print(",");
    Serial.println(aiTemperatureSamples[i], 2);
  }

  Serial.println("=== CSV EXPORT COMPLETE ===");
}


void handleSerialCommands() {
  static char command[16];
  static int pos = 0;

  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\r') {
      continue;
    }

    if (c == '\n') {
      command[pos] = '\0';

      if (strcmp(command, "DUMP") == 0) {
        dumpAIData();
      } else if (strcmp(command, "STATUS") == 0) {
        Serial.print("AI samples collected: ");
        Serial.println(aiSampleCount);

        Serial.print("Collection complete: ");
        Serial.println(aiCollectionReady ? "YES" : "NO");
      } else if (strcmp(command, "AITEST") == 0) {
        // Fixed feature vector from the validated NASA-model parity test.
        // This demonstrates on-device inference only; it is NOT the connected battery's SOH.
        const float nasaTestFeatures[29] = {
          3.53443074f, 0.04792569f, 3.12088370f, 3.92845297f,
          0.16359234f, 0.69155920f, -0.00018838f, 0.00077921f,
          -1.84753311f, 0.11219701f, 0.34035432f, 2.01661372f,
          0.00004551f, 0.00185463f, 32.80110931f, 0.44147322f,
          25.85039902f, 37.54111481f, 0.44147322f, 1.39557838f,
          3.60045528f, 0.00277732f, 0.00565592f, 12.0f,
          3.20743203f, 3.43788242f, 3.53487587f, 3.67390347f,
          3.78550220f
        };

        aiSOH = predictSOH(nasaTestFeatures);
        aiEstimateAvailable = true;
        aiDemoRun = true;

        Serial.println("VOLTSENTINEL AI INFERENCE DEMO");
        Serial.println("Input: fixed NASA battery-dataset feature vector");
        Serial.print("Predicted dataset SOH: ");
        Serial.print(aiSOH, 4);
        Serial.println("%");
        Serial.println("NOT an estimate of the connected physical battery.");
        Serial.println("Press the OLED button to view AI page.");
      } else if (pos > 0) {
        Serial.println("Unknown command. Use DUMP, STATUS, or AITEST.");
      }

      pos = 0;
    } else {
      if (pos < (int)sizeof(command) - 1) {
        command[pos++] = c;
      } else {
        pos = 0;
        Serial.println("Command too long.");
      }
    }
  }
}


// =====================================================

// SCREEN 0

// LIVE MONITOR

// =====================================================

void displayLiveMonitor() {

  display.clearBuffer();

  display.setFont(

    u8g2_font_6x10_tf

  );

  // Header

  display.drawStr(

    0,

    9,

    "VOLTSENTINEL"

  );

  display.setCursor(

    88,

    9

  );

  display.print(

    getModeText()

  );

  display.drawHLine(

    0,

    12,

    128

  );

  // Voltage

  display.setCursor(

    0,

    25

  );

  display.print(

    "V: "

  );

  display.print(

    voltage,

    2

  );

  display.print(

    " V"

  );

  // Current

  display.setCursor(

    0,

    37

  );

  display.print(

    "I: "

  );

  display.print(

    current,

    1

  );

  display.print(

    " mA"

  );

  // Power

  display.setCursor(

    0,

    49

  );

  display.print(

    "P: "

  );

  display.print(

    power,

    1

  );

  display.print(

    " mW"

  );

  // Temperature

  display.setCursor(

    0,

    61

  );

  display.print(

    "T: "

  );

  display.print(

    temperature,

    1

  );

  display.print(

    " C"

  );

  display.sendBuffer();
}

// =====================================================

// SCREEN 1

// VOLTAGE GRAPH

// =====================================================

void displayVoltageGraph() {

  display.clearBuffer();

  display.setFont(

    u8g2_font_6x10_tf

  );

  display.drawStr(

    0,

    8,

    "VOLTAGE 30s"

  );

  display.setCursor(

    88,

    8

  );

  display.print(

    voltage,

    2

  );

  const int left = 14;

  const int right = 127;

  const int top = 15;

  const int bottom = 52;

  // Fixed display range

  const float minV = 3.40;

  const float maxV = 4.25;

  // ---------------------------------------------------

  // 4.20 V reference

  // ---------------------------------------------------

  int highY =

    bottom -

    (

      (

        CRITICAL_HIGH_VOLTAGE -

        minV

        )
      *

      (

        bottom -

        top

        )

        )
      /

      (

        maxV -

        minV

      );

  for (

    int x = left;

    x < right;

    x += 5

  ) {

    display.drawPixel(

      x,

      highY

    );
  }

  // ---------------------------------------------------

  // 3.60 V reference

  // ---------------------------------------------------

  int lowY =

    bottom -

    (

      (

        LOW_VOLTAGE_WARNING -

        minV

        )
      *

      (

        bottom -

        top

        )

        )
      /

      (

        maxV -

        minV

      );

  for (

    int x = left;

    x < right;

    x += 5

  ) {

    display.drawPixel(

      x,

      lowY

    );
  }

  // Axes

  display.drawLine(

    left,

    top,

    left,

    bottom

  );

  display.drawLine(

    left,

    bottom,

    right,

    bottom

  );

  // ---------------------------------------------------

  // DRAW GRAPH

  // ---------------------------------------------------

  int count =

    recentFilled

      ? RECENT_POINTS

      : recentIndex;

  if (

    count >= 2

  ) {

    for (

      int i = 1;

      i < count;

      i++

    ) {

      int previousIndex =

        recentFilled

          ?

          (

            recentIndex +

            i -

            1

            )
            %

            RECENT_POINTS

          :

          i - 1;

      int currentIndex =

        recentFilled

          ?

          (

            recentIndex +

            i

            )
            %

            RECENT_POINTS

          :

          i;

      float v1 =

        recentVoltage[

          previousIndex

      ];

      float v2 =

        recentVoltage[

          currentIndex

      ];

      int x1 =

        left +

        (

          (

            i - 1

            )
          *

          (

            right -

            left

            )

            )
          /

          (

            count - 1

          );

      int x2 =

        left +

        (

          i *

          (

            right -

            left

            )

            )
          /

          (

            count - 1

          );

      int y1 =

        bottom -

        (

          (

            v1 -

            minV

            )
          *

          (

            bottom -

            top

            )

            )
          /

          (

            maxV -

            minV

          );

      int y2 =

        bottom -

        (

          (

            v2 -

            minV

            )
          *

          (

            bottom -

            top

            )

            )
          /

          (

            maxV -

            minV

          );

      y1 =

        constrain(

          y1,

          top,

          bottom

        );

      y2 =

        constrain(

          y2,

          top,

          bottom

        );

      display.drawLine(

        x1,

        y1,

        x2,

        y2

      );
    }
  }

  display.setCursor(

    0,

    61

  );

  display.print(

    "3.6"

  );

  display.setCursor(

    92,

    61

  );

  display.print(

    getVoltageTrend()

  );

  display.sendBuffer();
}

// =====================================================

// SCREEN 2

// TEMPERATURE GRAPH

// =====================================================

void displayTemperatureGraph() {

  display.clearBuffer();

  display.setFont(

    u8g2_font_6x10_tf

  );

  display.drawStr(

    0,

    8,

    "TEMP 30s"

  );

  display.setCursor(

    88,

    8

  );

  display.print(

    temperature,

    1

  );

  display.print(

    "C"

  );

  const int left = 14;

  const int right = 127;

  const int top = 15;

  const int bottom = 52;

  const float minT = 20.0;

  const float maxT = 55.0;

  // Warning line

  int warningY =

    bottom -

    (

      (

        TEMP_WARNING -

        minT

        )
      *

      (

        bottom -

        top

        )

        )
      /

      (

        maxT -

        minT

      );

  for (

    int x = left;

    x < right;

    x += 5

  ) {

    display.drawPixel(

      x,

      warningY

    );
  }

  // Critical line

  int criticalY =

    bottom -

    (

      (

        TEMP_CRITICAL -

        minT

        )
      *

      (

        bottom -

        top

        )

        )
      /

      (

        maxT -

        minT

      );

  for (

    int x = left;

    x < right;

    x += 5

  ) {

    display.drawPixel(

      x,

      criticalY

    );
  }

  // Axes

  display.drawLine(

    left,

    top,

    left,

    bottom

  );

  display.drawLine(

    left,

    bottom,

    right,

    bottom

  );

  // Graph

  int count =

    recentFilled

      ? RECENT_POINTS

      : recentIndex;

  if (

    count >= 2

  ) {

    for (

      int i = 1;

      i < count;

      i++

    ) {

      int previousIndex =

        recentFilled

          ?

          (

            recentIndex +

            i -

            1

            )
            %

            RECENT_POINTS

          :

          i - 1;

      int currentIndex =

        recentFilled

          ?

          (

            recentIndex +

            i

            )
            %

            RECENT_POINTS

          :

          i;

      float t1 =

        recentTemperature[

          previousIndex

      ];

      float t2 =

        recentTemperature[

          currentIndex

      ];

      int x1 =

        left +

        (

          (

            i - 1

            )
          *

          (

            right -

            left

            )

            )
          /

          (

            count - 1

          );

      int x2 =

        left +

        (

          i *

          (

            right -

            left

            )

            )
          /

          (

            count - 1

          );

      int y1 =

        bottom -

        (

          (

            t1 -

            minT

            )
          *

          (

            bottom -

            top

            )

            )
          /

          (

            maxT -

            minT

          );

      int y2 =

        bottom -

        (

          (

            t2 -

            minT

            )
          *

          (

            bottom -

            top

            )

            )
          /

          (

            maxT -

            minT

          );

      y1 =

        constrain(

          y1,

          top,

          bottom

        );

      y2 =

        constrain(

          y2,

          top,

          bottom

        );

      display.drawLine(

        x1,

        y1,

        x2,

        y2

      );
    }
  }

  display.setCursor(

    0,

    61

  );

  display.print(

    "45/50"

  );

  display.setCursor(

    92,

    61

  );

  display.print(

    getTemperatureTrend()

  );

  display.sendBuffer();
}

// =====================================================

// SCREEN 3

// SYSTEM ANALYSIS

// =====================================================

void displaySafety() {

  display.clearBuffer();

  display.setFont(

    u8g2_font_6x10_tf

  );

  display.drawStr(

    0,

    8,

    "SYSTEM ANALYSIS"

  );

  display.drawHLine(

    0,

    11,

    128

  );

  // Voltage status

  display.setCursor(

    0,

    22

  );

  display.print(

    "V:"

  );

  display.print(

    getVoltageStatus()

  );

  // Temperature status

  display.setCursor(

    65,

    22

  );

  display.print(

    "T:"

  );

  display.print(

    getTemperatureStatus()

  );

  // dV/dt

  display.setCursor(

    0,

    34

  );

  display.print(

    "dV:"

  );

  display.print(

    voltageSlope,

    3

  );

  display.print(

    "V/s"

  );

  // dT/dt

  display.setCursor(

    0,

    46

  );

  display.print(

    "dT:"

  );

  display.print(

    temperatureSlope,

    2

  );

  display.print(

    "C/s"

  );

  // dI/dt

  display.setCursor(

    0,

    58

  );

  display.print(

    "dI:"

  );

  display.print(

    currentSlope,

    1

  );

  display.print(

    "mA/s"

  );

  display.setCursor(

    82,

    58

  );

  display.print(

    getSafetyText()

  );

  display.sendBuffer();
}

// =====================================================

// UPDATE DISPLAY

// =====================================================

void updateDisplay() {

  if (screen == 0) {
    displayLiveMonitor();
  } else if (screen == 1) {
    displayVoltageGraph();
  } else if (screen == 2) {
    displayTemperatureGraph();
  } else if (screen == 3) {
    displaySafety();
  } else if (screen == 4) {
    display.clearBuffer();
    display.setFont(u8g2_font_6x10_tf);

    display.drawStr(0, 9, "AI BATTERY HEALTH");
    display.drawHLine(0, 12, 128);

    if (aiDemoRun) {
      display.drawStr(0, 25, "NASA DATASET TEST");

      display.setFont(u8g2_font_helvB14_tf);
      display.setCursor(0, 46);
      display.print(aiSOH, 2);
      display.print("%");

      display.setFont(u8g2_font_6x10_tf);
      display.drawStr(0, 59, "Not physical-cell SOH");
    } else {
      display.drawStr(0, 26, "AI MODEL: LOADED");
      display.drawStr(0, 40, "Serial: type AITEST");
      display.drawStr(0, 54, "NASA sample only");
    }

    display.sendBuffer();
  }
}

// =====================================================

// BUTTON

// =====================================================

void checkButton() {

  bool state =

    digitalRead(

      BUTTON_PIN

    );

  if (

    lastButtonState == HIGH &&

    state == LOW

  ) {

    if (

      millis() -

        lastButtonTime
      >

      250

    ) {

      lastButtonTime =

        millis();

      screen++;

      if (

        screen >=

        TOTAL_SCREENS

      ) {

        screen = 0;
      }
    }
  }

  lastButtonState =

    state;
}

// =====================================================

// SLIDE SWITCH

// =====================================================

void checkModeSwitch() {

  int state =

    digitalRead(

      SWITCH_PIN

    );

  if (

    state == LOW

  ) {

    systemMode =

      MODE_DATA;

  }

  else {

    systemMode =

      MODE_MONITOR;
  }
}

// =====================================================

// CSV DATA OUTPUT

// =====================================================

void printCSV() {

  Serial.print(

    millis()

  );

  Serial.print(",");

  Serial.print(

    voltage,

    4

  );

  Serial.print(",");

  Serial.print(

    current,

    3

  );

  Serial.print(",");

  Serial.print(

    power,

    3

  );

  Serial.print(",");

  Serial.print(

    temperature,

    3

  );

  Serial.print(",");

  Serial.print(

    voltageSlope,

    6

  );

  Serial.print(",");

  Serial.print(

    currentSlope,

    6

  );

  Serial.print(",");

  Serial.println(

    temperatureSlope,

    6

  );
}

// =====================================================

// SERIAL HEADER

// =====================================================

void printCSVHeader() {

  Serial.println(

    "timestamp_ms,voltage_V,current_mA,power_mW,temperature_C,dVdt_Vs,dIdt_mAs,dTdt_Cs"

  );
}

// =====================================================

// NORMAL SERIAL INFORMATION

// =====================================================

void printNormalData() {

  Serial.println();

  Serial.println(

    "========================================"

  );

  Serial.println(

    "          VOLTSENTINEL"

  );

  Serial.println(

    "========================================"

  );

  Serial.print(

    "Mode          : "

  );

  Serial.println(

    getModeText()

  );

  Serial.print(

    "Voltage       : "

  );

  Serial.print(

    voltage,

    3

  );

  Serial.println(

    " V"

  );

  Serial.print(

    "Current       : "

  );

  Serial.print(

    current,

    2

  );

  Serial.println(

    " mA"

  );

  Serial.print(

    "Power         : "

  );

  Serial.print(

    power,

    2

  );

  Serial.println(

    " mW"

  );

  Serial.print(

    "Temperature   : "

  );

  Serial.print(

    temperature,

    2

  );

  Serial.println(

    " C"

  );

  Serial.println(

    "------------- SAFETY ------------------"

  );

  Serial.print(

    "Voltage       : "

  );

  Serial.println(

    getVoltageStatus()

  );

  Serial.print(

    "Temperature   : "

  );

  Serial.println(

    getTemperatureStatus()

  );

  Serial.print(

    "System        : "

  );

  Serial.println(

    getSafetyText()

  );

  Serial.println(

    "------------- BEHAVIOR ----------------"

  );

  Serial.print(

    "Voltage trend : "

  );

  Serial.println(

    getVoltageTrend()

  );

  Serial.print(

    "Temp trend    : "

  );

  Serial.println(

    getTemperatureTrend()

  );

  Serial.println(

    "------------- AI FEATURES -------------"

  );

  Serial.print(

    "dV/dt         : "

  );

  Serial.print(

    voltageSlope,

    6

  );

  Serial.println(

    " V/s"

  );

  Serial.print(

    "dI/dt         : "

  );

  Serial.print(

    currentSlope,

    6

  );

  Serial.println(

    " mA/s"

  );

  Serial.print(

    "dT/dt         : "

  );

  Serial.print(

    temperatureSlope,

    6

  );

  Serial.println(

    " C/s"

  );

  Serial.println(

    "========================================"

  );
}

// =====================================================

// WEB DASHBOARD (Wi-Fi)

// =====================================================

//
// Set your home Wi-Fi below. If the connection fails,
// the board starts its own access point instead:
//   Network  : VoltSentinel
//   Password : voltsentinel
//   Address  : http://192.168.4.1
//
// On your home network open:
//   http://voltsentinel.local   (or the IP printed on Serial)
//
// =====================================================

const char* WIFI_SSID = "Nothing Phone (3a) Pro";

const char* WIFI_PASS = "phone3apro";

const char* AP_SSID = "VoltSentinel";

const char* AP_PASS = "voltsentinel";

const unsigned long WIFI_TIMEOUT_MS = 10000;

WebServer server(80);

bool wifiApMode = false;

// Keeps the CSV stream clean in DATA mode.
static void webLog(const String &m) {

  if (systemMode == MODE_MONITOR) {
    Serial.println(m);
  }
}

// ---------- JSON helpers ----------

static void jNum(String &s, const char* key, float v, int dec) {

  s += '"';
  s += key;
  s += "\":";

  if (isfinite(v)) {
    s += String(v, dec);
  } else {
    s += "null";
  }

  s += ',';
}

static void jStr(String &s, const char* key, const String &v) {

  s += '"';
  s += key;
  s += "\":\"";
  s += v;
  s += "\",";
}

static bool tempValid() {

  return isfinite(temperature) &&
         temperature != DEVICE_DISCONNECTED_C;
}

// ---------- GET /api/state ----------

void handleApiState() {

  String j;
  j.reserve(900);

  j += '{';

  j += "\"ready\":";
  j += (previousFeatureTime != 0) ? "true," : "false,";

  j += "\"up\":";
  j += String(millis());
  j += ',';

  j += "\"at\":";
  j += String(lastSensorUpdate);
  j += ',';

  jNum(j, "v", voltage, 3);
  jNum(j, "i", current, 2);
  jNum(j, "p", power, 1);

  if (tempValid()) {
    jNum(j, "c", temperature, 2);
  } else {
    j += "\"c\":null,";
  }

  jNum(j, "dv", voltageSlope, 6);
  jNum(j, "di", currentSlope, 4);
  jNum(j, "dt", temperatureSlope, 6);

  jStr(j, "safety", getSafetyText());
  jStr(j, "mode", getModeText());
  jStr(j, "vs", getVoltageStatus());
  jStr(j, "tst", getTemperatureStatus());
  jStr(j, "vt", getVoltageTrend());
  jStr(j, "tt", getTemperatureTrend());

  j += "\"ai\":{";
  j += "\"n\":" + String(aiSampleCount) + ',';
  j += "\"max\":" + String(AI_MAX_SAMPLES) + ',';
  j += "\"ready\":" + String(aiCollectionReady ? "true" : "false") + ',';
  j += "\"avail\":" + String(aiEstimateAvailable ? "true" : "false") + ',';
  j += "\"demo\":" + String(aiDemoRun ? "true" : "false") + ',';
  j += "\"soh\":" + String(aiEstimateAvailable ? aiSOH : 0.0f, 2);
  j += "},";

  j += "\"lim\":{";
  j += "\"lowWarn\":" + String(LOW_VOLTAGE_WARNING, 2) + ',';
  j += "\"lowCrit\":" + String(CRITICAL_LOW_VOLTAGE, 2) + ',';
  j += "\"highCrit\":" + String(CRITICAL_HIGH_VOLTAGE, 2) + ',';
  j += "\"tWarn\":" + String(TEMP_WARNING, 1) + ',';
  j += "\"tCrit\":" + String(TEMP_CRITICAL, 1);
  j += "},";

  j += "\"net\":{";
  j += "\"ip\":\"" + (wifiApMode ? WiFi.softAPIP().toString() : WiFi.localIP().toString()) + "\",";
  j += "\"rssi\":" + String(wifiApMode ? 0 : WiFi.RSSI()) + ',';
  j += "\"ap\":" + String(wifiApMode ? "true" : "false");
  j += "}}";

  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", j);
}

// ---------- GET /api/history (last 30 s, oldest first) ----------

void handleApiHistory() {

  int count = recentFilled ? RECENT_POINTS : recentIndex;
  int start = recentFilled ? recentIndex : 0;

  String v = "[", c = "[", z = "[";

  for (int k = 0; k < count; k++) {

    int idx = (start + k) % RECENT_POINTS;

    if (k) {
      v += ',';
      c += ',';
      z += ',';
    }

    v += String(recentVoltage[idx], 3);
    z += "null";

    float t = recentTemperature[idx];

    if (isfinite(t) && t != DEVICE_DISCONNECTED_C) {
      c += String(t, 2);
    } else {
      c += "null";
    }
  }

  v += ']';
  c += ']';
  z += ']';

  // History only stores voltage + temperature.
  // Current / power are not buffered, so send nulls.
  String j = "{\"v\":" + v + ",\"i\":" + z + ",\"p\":" + z + ",\"c\":" + c + "}";

  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "application/json", j);
}

// ---------- GET /api/ai.csv ----------

void handleApiAiCsv() {

  server.sendHeader("Content-Disposition", "attachment; filename=\"voltsentinel_ai.csv\"");
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/csv", "");

  server.sendContent("sample,voltage_V,current_A,temperature_C\n");

  for (int k = 0; k < aiSampleCount; k++) {

    String line = String(k + 1) + ',' +
                  String(aiVoltageSamples[k], 4) + ',' +
                  String(aiCurrentSamples[k], 6) + ',' +
                  String(aiTemperatureSamples[k], 2) + '\n';

    server.sendContent(line);
  }

  server.sendContent("");
}

// ---------- POST /api/ai/reset ----------

void handleApiAiReset() {

  aiSampleCount = 0;
  aiCollectionReady = false;
  aiLastSampleTime = 0;

  webLog("AI data collection restarted from dashboard.");

  server.send(200, "application/json", "{\"ok\":true}");
}

// ---------- GET / ----------

void handleRoot() {

  server.send_P(200, "text/html", DASHBOARD_HTML);
}

// ---------- SETUP ----------

void setupWeb() {

  display.clearBuffer();
  display.setFont(u8g2_font_6x10_tf);
  display.drawStr(10, 25, "VOLTSENTINEL");
  display.drawStr(16, 42, "WIFI...");
  display.sendBuffer();

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long t0 = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - t0 < WIFI_TIMEOUT_MS) {

    delay(200);
  }

  if (WiFi.status() == WL_CONNECTED) {

    wifiApMode = false;

    webLog("Dashboard: http://" + WiFi.localIP().toString());

  } else {

    wifiApMode = true;

    WiFi.disconnect(true);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASS);

    webLog("Wi-Fi failed. Access point '" + String(AP_SSID) +
           "' started. Dashboard: http://" + WiFi.softAPIP().toString());
  }

  if (MDNS.begin("voltsentinel")) {

    webLog("Also available at http://voltsentinel.local");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/state", HTTP_GET, handleApiState);
  server.on("/api/history", HTTP_GET, handleApiHistory);
  server.on("/api/ai.csv", HTTP_GET, handleApiAiCsv);
  server.on("/api/ai/reset", HTTP_POST, handleApiAiReset);

  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
}

// =====================================================

// SETUP

// =====================================================

void setup() {

  Serial.begin(

    115200

  );

  delay(1000);

  // ===================================================

  // I2C

  // ===================================================

  Wire.begin(

    SDA_PIN,

    SCL_PIN

  );

  // ===================================================

  // OLED

  // ===================================================

  display.begin();

  display.clearBuffer();

  display.setFont(

    u8g2_font_6x10_tf

  );

  display.drawStr(

    10,

    25,

    "VOLTSENTINEL"

  );

  display.drawStr(

    16,

    42,

    "INITIALIZING..."

  );

  display.sendBuffer();

  // ===================================================

  // RGB

  // ===================================================

  rgb.begin();

  rgb.setBrightness(

    40

  );

  rgb.clear();

  rgb.show();

  // ===================================================

  // BUTTON

  // ===================================================

  pinMode(

    BUTTON_PIN,

    INPUT_PULLUP

  );

  // ===================================================

  // SLIDE SWITCH

  // ===================================================

  pinMode(

    SWITCH_PIN,

    INPUT_PULLUP

  );

  // ===================================================

  // BUZZER

  // ===================================================

  if (

    BUZZER_ENABLED

  ) {

    pinMode(

      BUZZER_PIN,

      OUTPUT

    );

    buzzerOff();
  }

  // ===================================================

  // INA219

  // ===================================================

  if (

    !ina219.begin()

  ) {

    display.clearBuffer();

    display.setFont(

      u8g2_font_6x10_tf

    );

    display.drawStr(

      0,

      25,

      "INA219 ERROR"

    );

    display.drawStr(

      0,

      42,

      "CHECK WIRING"

    );

    display.sendBuffer();

    setRGB(

      255,

      0,

      0

    );

    while (1) {

      delay(100);
    }
  }

  ina219.setCalibration_32V_2A();

  // ===================================================

  // DS18B20

  // ===================================================

  tempSensor.begin();

  // ===================================================

  // HISTORY INITIALIZATION

  // ===================================================

  for (

    int i = 0;

    i < RECENT_POINTS;

    i++

  ) {

    recentVoltage[i] =

      0.0;

    recentTemperature[i] =

      0.0;
  }

  for (

    int i = 0;

    i < LONG_POINTS;

    i++

  ) {

    longVoltage[i] =

      0.0;

    longTemperature[i] =

      0.0;
  }

  // ===================================================

  // INITIAL MODE

  // ===================================================

  checkModeSwitch();

  updateRGB();

  // ===================================================

  // STARTUP MESSAGE

  // ===================================================

  Serial.println();

  Serial.println(

    "========================================"

  );

  Serial.println(

    "          VOLTSENTINEL v5"

  );

  Serial.println(

    "Battery Monitoring + Data Collection"

  );

  Serial.println(

    "========================================"

  );

  Serial.println();

  if (

    systemMode ==

    MODE_DATA

  ) {

    printCSVHeader();
  }

  // ===================================================

  // WEB DASHBOARD

  // ===================================================

  setupWeb();

  delay(1000);

  // ============================================================

  // Temporary hard-coded AI test removed.
}

// =====================================================

// MAIN LOOP

// =====================================================

void loop() {
   
  handleSerialCommands();

  server.handleClient();

  // ===================================================

  // CHECK MODE SWITCH

  // ===================================================

  checkModeSwitch();

  // ===================================================

  // BUTTON

  // ===================================================

  checkButton();

  // ===================================================

  // SENSOR UPDATE

  // ===================================================

  if (

    millis() -

      lastSensorUpdate
    >=

    SENSOR_INTERVAL

  ) {

    lastSensorUpdate =

      millis();

    // -------------------------------------------------

    // INA219

    // -------------------------------------------------

    voltage =

      ina219.getBusVoltage_V();

    current =

      ina219.getCurrent_mA();

    power =

      ina219.getPower_mW();

    // -------------------------------------------------

    // DS18B20

    // -------------------------------------------------

    tempSensor.requestTemperatures();

    temperature =

      tempSensor.getTempCByIndex(0);

    // -------------------------------------------------

    // HISTORY

    // -------------------------------------------------

    updateRecentHistory();

    updateLongHistory();

    // -------------------------------------------------

    // BEHAVIOR FEATURES

    // -------------------------------------------------

    calculateFeatures();

    collectAIData();

    // -------------------------------------------------

    // SAFETY

    // -------------------------------------------------

    analyzeSafety();

    // -------------------------------------------------

    // SERIAL

    // -------------------------------------------------

    if (

      systemMode ==

      MODE_DATA

    ) {

      printCSV();

    }

    else {

      printNormalData();
    }
  }

  // ===================================================

  // DISPLAY

  // ===================================================

  if (

    millis() -

      lastDisplayUpdate
    >=

    DISPLAY_INTERVAL

  ) {

    lastDisplayUpdate =

      millis();

    updateDisplay();
  }
}
