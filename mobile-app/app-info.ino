/*
  -- car WATAR --
  هذا الكود تم توليده تلقائيًا من خلال RemoteXY editor
  ويتحكم في واجهة تطبيق للتواصل مع Arduino عبر Bluetooth أو USB
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// يمكنك تفعيل سجل التصحيح على Serial بسرعة 115200 إن أردت
// #define REMOTEXY__DEBUGLOG

// اختيار وضع الاتصال: Serial عادي (HardSerial)
#define REMOTEXY_MODE__HARDSERIAL

// إعدادات الاتصال بـ RemoteXY
#define REMOTEXY_SERIAL Serial     // نستخدم منفذ Serial الأساسي (USB أو بلوتوث)
#define REMOTEXY_SERIAL_SPEED 9600 // سرعة الباود للاتصال

// تضمين مكتبة RemoteXY
#include <RemoteXY.h>

// تكوين واجهة التطبيق - هذه البيانات يتم توليدها من RemoteXY editor
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =                            // 144 bytes
    {255, 4, 0, 27, 0, 137, 0, 19, 0, 0, 0, 0, ...}; // (بيانات خاصة بتخطيط الواجهة)

// هيكل RemoteXY يحتوي على كل المتغيرات بين التطبيق و Arduino
struct
{
    // متغيرات الإدخال (من المستخدم)
    int8_t slider_01;     // سلايدر من 0 إلى 100 - ربما للتحكم في سرعة أو زاوية
    int8_t joystick_01_x; // محور X في الجويستيك (يسار/يمين)
    int8_t joystick_01_y; // محور Y في الجويستيك (أمام/خلف)
    int8_t slider_02;     // سلايدر إضافي - الاستخدام حسب التطبيق

    // متغيرات الإخراج (تُعرض على التطبيق)
    int8_t level_temp_up;   // مؤشر لدرجة الحرارة الموجبة
    int8_t level_temp_down; // مؤشر لدرجة الحرارة السالبة
    int8_t level_hum;       // مؤشر للرطوبة
    char text_temp[11];     // نص لعرض درجة الحرارة
    char text_hum[11];      // نص لعرض الرطوبة
    int16_t sound_01;       // صوت تنبيهي رقم 1 - مثل 1001 = صوت صفارة بالتطبيق

    // متغير الحالة - يحدد إذا ما كان هناك اتصال
    uint8_t connect_flag; // 1 = متصل، 0 = غير متصل

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

// دالة الإعداد - يتم تنفيذها مرة واحدة عند تشغيل Arduino
void setup()
{
    RemoteXY_Init(); // تهيئة الاتصال مع التطبيق
                     // يمكنك هنا كتابة إعدادات إضافية مثل pinMode أو setup للحساسات
}

// دالة التكرار - تعمل باستمرار بعد التشغيل
void loop()
{
    RemoteXY_Handler(); // تحديث البيانات بين التطبيق و Arduino

    // هنا تكتب كودك الخاص بالتحكم، القراءة من الحساسات، أو المحركات
    // لا تستخدم delay()، بل استخدم RemoteXY_delay(ms) لتفادي مشاكل الاتصال
}
