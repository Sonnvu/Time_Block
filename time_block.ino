#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "WiFi.h"
#include <HTTPClient.h>

#include "time.h"

const char* ssid = "Fios-V9jLG"; // Fios-V9jLG
const char* password = "rot54cane73flax"; //rot54cane73flax
const char* api_key = "NWE2NWM4ODMtNmZkZi00YmJjLWE0YjktOGQzMzFmMGYyYjZk";

const char* workspace_id = "629e353aee936d4be89efa9d";
const char* user_id = "629e3539ee936d4be89efa9a";

const char* project_id = "629e3b82ee936d4be89f9b21";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -18000;   //Replace with your GMT offset (seconds)
const int   daylightOffset_sec = 3600;  //Replace with your daylight offset (seconds)

const float SHAKE_THRESHOLD = 20.0;

char* projectIDs[] = {
  "None",
  "62b5becf786604599fc1c1cd", // study
  "629e3b76c83f906b1b7630ac", // sleep
  "629e3b5dee936d4be89f97de", // relax
  "629e3b8bc83f906b1b7632b3", // gym
  "62a7bf10478998412d4e3321", // eat
  "629e3b82ee936d4be89f9b21"  // cooking
};

Adafruit_MPU6050 mpu;
 
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
 
int x;
int y;
int z;
int side1, side2, side3, side4, side5, side6;
int prevSide1 = 0, prevSide2 = 0, prevSide3 = 0, prevSide4 = 0, prevSide5 = 0, prevSide6 = 0;

String printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    String response = "Failed to obtain time";
    Serial.println(response);
    return response;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S-04:00", &timeinfo);
  return String(buffer);
}

void addTimeEntry(String project_id) {
  // Make API request
  HTTPClient http;

  String url = "https://api.clockify.me/api/v1/workspaces/";
  url += workspace_id;
  url += "/time-entries";

  http.begin(url);
  http.addHeader("X-Api-Key", api_key);
  http.addHeader("Content-Type", "application/json");

  // Concatenate date and time strings
  String startTime = printLocalTime(); 

  String jsonBody = "{\"projectId\":\"" + String(project_id) + "\",";
  jsonBody += "\"start\":\"" + startTime + "\"}";

  int httpCode = http.POST(jsonBody);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error: " + http.errorToString(httpCode));
  }

  http.end();
}

void stopTimeEntry() {
  // Make API request
  HTTPClient http;

  String url = "https://api.clockify.me/api/v1/workspaces/";
  url += workspace_id;
  url += "/user/";
  url += user_id;
  url += "/time-entries";

  http.begin(url);
  http.addHeader("X-Api-Key", api_key);
  http.addHeader("Content-Type", "application/json");

  // Concatenate date and time strings
  String endTime = printLocalTime(); 

  String jsonBody = "{\"end\":\"" + endTime + "\"}";

  int httpCode = http.PATCH(jsonBody);

  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error: " + http.errorToString(httpCode));
  }

  http.end();
}
 
void setup()
{
  Serial.begin(115200);

  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(5, HIGH);
    delay(1000);
    digitalWrite(5, LOW);
    delay(1000);
    Serial.println(".");
  }

  Serial.println("Connected to WiFi");
  digitalWrite(18, HIGH);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }
  Serial.println("");
  delay(100);
}

int prevSide = 0;


void updateSide(int x, int y, int z) {
  int absX = abs(x);
  int absY = abs(y);
  int absZ = abs(z);
  int maxAbs = max(max(absX, absY), absZ);
  float normX = (float)x / maxAbs;
  float normY = (float)y / maxAbs;
  float normZ = (float)z / maxAbs;

  int side = 0;
  if (normZ >= 0.9) {
    side = 1;
  } else if (normZ <= -0.9) {
    side = 6;
  } else if (normX >= 0.9) {
    side = 2;
  } else if (normX <= -0.9) {
    side = 4;
  } else if (normY >= 0.9) {
    side = 5;
  } else if (normY <= -0.9) {
    side = 3;
  } else {
    Serial.println("Orientation not detected");
  }

  if (side != prevSide) {
    Serial.print("Side: ");
    Serial.println(side);
    stopTimeEntry();
    addTimeEntry(String(projectIDs[side]));
    prevSide = side;
  }

  delay(100);
}

int ON = 1;
unsigned long previousMillis = 0;
unsigned long interval = 30000;

void loop()
{
  // Reconnect after WiFi connection lost
  unsigned long currentMillis = millis();
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    Serial.println("Reconnecting to WiFi...");
    digitalWrite(18, LOW);
    WiFi.disconnect();
    WiFi.reconnect();
    digitalWrite(18, HIGH);
    previousMillis = currentMillis;
  }
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  x = a.acceleration.x;
  y = a.acceleration.y;
  z = a.acceleration.z;

  float magnitude = sqrt(x*x + y*y + z*z);
  if (magnitude > 12.0) {
    stopTimeEntry();
    Serial.println("Shake detected!");
    if (ON == 0) {
      ON = 1;
      Serial.println("Device is ON"); 
      digitalWrite(5, LOW); 
      digitalWrite(18, HIGH); 
    }
    else if (ON == 1) {
      ON = 0;
      Serial.println("Device is OFF");
      digitalWrite(5, HIGH); 
      digitalWrite(18, LOW);
    }
  }

  if (ON == 1) {
    updateSide(x, y, z);
  }

  // publishMessage();
  delay(1000);
}