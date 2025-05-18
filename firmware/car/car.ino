#define REMOTEXY_MODE__HARDSERIAL
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 9600
#define trigPin 9
#define echoPin 10
long duration, mesafe;
#include <RemoteXY.h>

// RemoteXY GUI configuration
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 175 bytes
    {255, 3, 0, 40, 0, 168, 0, 19, 0, 0, 0, 0, 31, 1, 106, 200, 1, 1, 15, 0,
     66, 53, 20, 50, 15, 129, 37, 26, 66, 3, 20, 50, 15, 193, 204, 26, 66, 3, 53, 102,
     14, 129, 165, 26, 129, 2, 7, 60, 11, 64, 1, 116, 101, 109, 112, 101, 114, 97, 116, 117,
     114, 101, 0, 129, 5, 40, 59, 11, 64, 164, 65, 105, 114, 32, 104, 117, 109, 105, 100, 105,
     116, 121, 0, 67, 65, 8, 40, 10, 68, 1, 26, 11, 67, 65, 41, 40, 10, 68, 165, 26,
     11, 69, 85, 181, 20, 20, 0, 137, 129, 86, 168, 20, 12, 64, 137, 103, 97, 115, 0, 4,
     0, 87, 106, 17, 128, 2, 26, 69, 0, 179, 20, 20, 0, 1, 129, 0, 167, 17, 12, 64,
     1, 102, 105, 114, 101, 0, 5, 11, 107, 79, 79, 32, 133, 24, 31, 67, 45, 71, 51, 14,
     70, 2, 26, 11, 129, 18, 70, 25, 15, 64, 2, 99, 109, 32, 0};

// this structure defines all the variables and events of your control interface
struct
{

    // input variables
    int8_t slider_01;     // from 0 to 100
    int8_t joystick_01_x; // from -100 to 100
    int8_t joystick_01_y; // from -100 to 100

    // output variables
    int8_t level_temp_up;   // from 0 to 100
    int8_t level_temp_down; // from 0 to 100
    int8_t level_hum;       // from 0 to 100
    char text_temp[11];     // string UTF8 end zero
    char text_hum[11];      // string UTF8 end zero
    int16_t sound_01;       // =0 no sound, else ID of sound, =1001 for example, look sound list in app
    int16_t sound_02;       // =0 no sound, else ID of sound, =1001 for example, look sound list in app
    char value_01[11];      // string UTF8 end zero
    uint8_t connect_flag;   // =1 if wire connected, else =0
} RemoteXY;
#pragma pack(pop)
#include "DHT.h"
#define DHTPIN 2
DHT dht(DHTPIN, DHT11);
#include <Servo.h>
Servo myservo;
#define PIN_MOTOR_RIGHT_UP 7
#define PIN_MOTOR_RIGHT_DN 6
#define PIN_MOTOR_RIGHT_SPEED 11
#define PIN_MOTOR_LEFT_UP 5
#define PIN_MOTOR_LEFT_DN 4
#define PIN_MOTOR_LEFT_SPEED 3
unsigned char RightMotor[3] =
    {PIN_MOTOR_RIGHT_UP, PIN_MOTOR_RIGHT_DN, PIN_MOTOR_RIGHT_SPEED};
unsigned char LeftMotor[3] =
    {PIN_MOTOR_LEFT_UP, PIN_MOTOR_LEFT_DN, PIN_MOTOR_LEFT_SPEED};
void Wheel(unsigned char *motor, int v)
{
    if (v > 100)
        v = 100;
    if (v < -100)
        v = -100;
    if (v > 0)
    {
        digitalWrite(motor[0], HIGH);
        digitalWrite(motor[1], LOW);
        analogWrite(motor[2], v * 2.55);
    }
    else if (v < 0)
    {
        digitalWrite(motor[0], LOW);
        digitalWrite(motor[1], HIGH);
        analogWrite(motor[2], (-v) * 2.55);
    }
    else
    {
        digitalWrite(motor[0], LOW);
        digitalWrite(motor[1], LOW);
        analogWrite(motor[2], 0);
    }
}
void setup()
{
    RemoteXY_Init();
    myservo.attach(12);
    RemoteXY.slider_01 = 50;
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(PIN_MOTOR_RIGHT_UP, OUTPUT);
    pinMode(PIN_MOTOR_RIGHT_DN, OUTPUT);
    pinMode(PIN_MOTOR_LEFT_UP, OUTPUT);
    pinMode(PIN_MOTOR_LEFT_DN, OUTPUT);
}
void loop()
{
    RemoteXY_Handler();
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    dtostrf(temp, 0, 1, RemoteXY.text_temp);
    dtostrf(hum, 0, 1, RemoteXY.text_hum);

    if (temp < 0)
    {
        RemoteXY.level_temp_up = 0;
        RemoteXY.level_temp_down = min(-temp * 2, 100);
    }
    else if (temp > 0)
    {
        RemoteXY.level_temp_up = min(temp * 2, 100);
        RemoteXY.level_temp_down = 0;
    }
    else
    {
        RemoteXY.level_temp_up = 0;
        RemoteXY.level_temp_down = 0;
    }
    RemoteXY.level_hum = hum;

    if (digitalRead(A0) == HIGH)
    {
        RemoteXY.sound_01 = 0;
    }
    else
    {
        RemoteXY.sound_01 = 1001;
    }

    if (digitalRead(A1) == HIGH)
    {
        RemoteXY.sound_02 = 0;
    }
    else
    {
        RemoteXY.sound_02 = 1001;
    }

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 100000);
    mesafe = (duration / 2) / 29.1;
    if (mesafe < 2 || mesafe > 300)
        strcpy(RemoteXY.value_01, "Out Range");
    else
        itoa(mesafe, RemoteXY.value_01, 10);
    delay(100);
    int ms = RemoteXY.slider_01 * 20 + 500;
    myservo.writeMicroseconds(ms);
    Wheel(RightMotor, RemoteXY.joystick_01_y - RemoteXY.joystick_01_x);
    Wheel(LeftMotor, RemoteXY.joystick_01_y + RemoteXY.joystick_01_x);
}