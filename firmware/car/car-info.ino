// 1. إعداد الاتصال مع تطبيق RemoteXY عبر Serial
#define REMOTEXY_MODE__HARDSERIAL  // وضع الاتصال: Serial عادي (كابل أو بلوتوث)
#define REMOTEXY_SERIAL Serial     // نستخدم منفذ Serial الرئيسي
#define REMOTEXY_SERIAL_SPEED 9600 // سرعة الاتصال: 9600 بت في الثانية

// 2. تعريف أرجل حساس الموجات فوق الصوتية
#define trigPin 9      // الرجل المسؤولة عن إرسال النبضة
#define echoPin 10     // الرجل المسؤولة عن استقبال النبضة
long duration, mesafe; // لحساب الزمن والمسافة

// 3. تضمين مكتبة RemoteXY
#include <RemoteXY.h>

// 4. إعداد واجهة التطبيق (RemoteXY GUI Configuration)
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] = // 175 bytes
    {255, 3, 0, 40, ...}; // (تم توليد هذه البيانات تلقائيًا من RemoteXY editor)

// 5. تعريف هيكل البيانات المرتبط بالواجهة
struct
{
    // عناصر إدخال من التطبيق
    int8_t slider_01;     // سلايدر من 0 إلى 100 للتحكم في السيرفو
    int8_t joystick_01_x; // جويستيك المحور X (يمين/يسار)
    int8_t joystick_01_y; // جويستيك المحور Y (أمام/خلف)

    // عناصر إخراج تظهر في التطبيق
    int8_t level_temp_up;   // عداد لدرجة الحرارة الموجبة
    int8_t level_temp_down; // عداد لدرجة الحرارة السالبة
    int8_t level_hum;       // عداد للرطوبة
    char text_temp[11];     // نص لعرض درجة الحرارة
    char text_hum[11];      // نص لعرض الرطوبة
    int16_t sound_01;       // تشغيل صوت تنبيهي 1
    int16_t sound_02;       // تشغيل صوت تنبيهي 2
    char value_01[11];      // نص لعرض المسافة
    uint8_t connect_flag;   // 1 إذا في اتصال، 0 إذا مفصول
} RemoteXY;
#pragma pack(pop)

// 6. تعريف حساس الحرارة والرطوبة DHT11
#include "DHT.h"
#define DHTPIN 2        // DHT موصول على البن 2
DHT dht(DHTPIN, DHT11); // إنشاء كائن الحساس

// 7. مكتبة ومحرك السيرفو
#include <Servo.h>
Servo myservo; // إنشاء كائن السيرفو

// 8. تعريف أرجل المواتير اليمنى واليسرى
#define PIN_MOTOR_RIGHT_UP 7
#define PIN_MOTOR_RIGHT_DN 6
#define PIN_MOTOR_RIGHT_SPEED 11
#define PIN_MOTOR_LEFT_UP 5
#define PIN_MOTOR_LEFT_DN 4
#define PIN_MOTOR_LEFT_SPEED 3

// 9. مصفوفات تسهل تمرير أرجل التحكم لكل موتور
unsigned char RightMotor[3] = {PIN_MOTOR_RIGHT_UP, PIN_MOTOR_RIGHT_DN, PIN_MOTOR_RIGHT_SPEED};
unsigned char LeftMotor[3] = {PIN_MOTOR_LEFT_UP, PIN_MOTOR_LEFT_DN, PIN_MOTOR_LEFT_SPEED};

// 10. دالة التحكم في حركة كل موتور
void Wheel(unsigned char *motor, int v)
{
    if (v > 100)
        v = 100; // أقصى سرعة للأمام
    if (v < -100)
        v = -100; // أقصى سرعة للخلف

    if (v > 0) // الحركة للأمام
    {
        digitalWrite(motor[0], HIGH); // اتجاه أمامي
        digitalWrite(motor[1], LOW);
        analogWrite(motor[2], v * 2.55); // تحويل القيمة لـ PWM (0-255)
    }
    else if (v < 0) // الحركة للخلف
    {
        digitalWrite(motor[0], LOW);
        digitalWrite(motor[1], HIGH); // اتجاه خلفي
        analogWrite(motor[2], (-v) * 2.55);
    }
    else // توقف
    {
        digitalWrite(motor[0], LOW);
        digitalWrite(motor[1], LOW);
        analogWrite(motor[2], 0);
    }
}

// 11. دالة الإعدادات setup
void setup()
{
    RemoteXY_Init();         // بدء الاتصال مع تطبيق RemoteXY
    myservo.attach(12);      // توصيل السيرفو على البن 12
    RemoteXY.slider_01 = 50; // تعيين قيمة افتراضية للسلايدر

    // إعداد أرجل الحساسات والموتورات
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    pinMode(PIN_MOTOR_RIGHT_UP, OUTPUT);
    pinMode(PIN_MOTOR_RIGHT_DN, OUTPUT);
    pinMode(PIN_MOTOR_LEFT_UP, OUTPUT);
    pinMode(PIN_MOTOR_LEFT_DN, OUTPUT);
}

// 12. دالة التكرار loop
void loop()
{
    RemoteXY_Handler(); // تحديث البيانات بين التطبيق وArduino

    // قراءة درجة الحرارة والرطوبة
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    dtostrf(temp, 0, 1, RemoteXY.text_temp); // تحويل الرقم إلى نص
    dtostrf(hum, 0, 1, RemoteXY.text_hum);

    // عرض مؤشرات الحرارة
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

    // عرض مؤشر الرطوبة
    RemoteXY.level_hum = hum;

    // قراءة حالتي A0 و A1 وتحديد الأصوات
    if (digitalRead(A0) == HIGH)
        RemoteXY.sound_01 = 0;
    else
        RemoteXY.sound_01 = 1001;

    if (digitalRead(A1) == HIGH)
        RemoteXY.sound_02 = 0;
    else
        RemoteXY.sound_02 = 1001; 

    // قراءة المسافة من حساس الموجات فوق الصوتية
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 100000); // مدة ارتداد النبضة
    mesafe = (duration / 2) / 29.1;            // تحويل الزمن إلى سم

    if (mesafe < 2 || mesafe > 300)
        strcpy(RemoteXY.value_01, "Out Range");
    else
        itoa(mesafe, RemoteXY.value_01, 10); // تحويل الرقم إلى نص

    delay(100); // تأخير بسيط

    // التحكم في السيرفو بناءً على السلايدر
    int ms = RemoteXY.slider_01 * 20 + 500;
    myservo.writeMicroseconds(ms);

    // التحكم في المواتير حسب الجويستيك
    Wheel(RightMotor, RemoteXY.joystick_01_y - RemoteXY.joystick_01_x);
    Wheel(LeftMotor, RemoteXY.joystick_01_y + RemoteXY.joystick_01_x);
}
