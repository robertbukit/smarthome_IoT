#include <WiFi.h> // Library untuk koneksi WiFi.
#include <WiFiClient.h> // Library untuk komunikasi WiFi.
#include <Adafruit_MQTT.h> // Library untuk komunikasi MQTT.
#include <Adafruit_MQTT_Client.h> // Library MQTT Client untuk Adafruit IO.
#include <RTClib.h> // Library RTC untuk bekerja dengan modul RTC.
#include <ESP32Servo.h> // Library untuk mengontrol servo menggunakan ESP32.
WiFiClient client; // Objek WiFi untuk komunikasi jaringan.
RTC_DS3231 rtc; // Objek RTC untuk membaca dan mengatur waktu.
Servo myServo; // Objek servo pertama.
Servo myServo2; // Objek servo kedua.
#define AIO_SERVER "io.adafruit.com" // Server Adafruit IO.
#define AIO_SERVERPORT 1883 // Port MQTT (gunakan 8883 untuk SSL).
#define AIO_USERNAME "shakilakila31" // Username Adafruit IO.
#define AIO_KEY "aio_EHlX82vMLGU49QLeaoW2wosu2Zl9" // Key Adafruit IO.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT,
AIO_USERNAME, AIO_KEY);
// Membuat objek MQTT client.
Adafruit_MQTT_Subscribe servo2 = Adafruit_MQTT_Subscribe(&mqtt,
AIO_USERNAME "/feeds/Servo 2");
// Berlangganan ke feed "Servo 2" untuk menerima pesan dari Adafruit IO.
int servoPin = 32; // Pin untuk servo pertama (pintu).
int servoPin2 = 33; // Pin untuk servo kedua (jendela).
void MQTT_connect() {
int8_t ret;
if (mqtt.connected()) {
return; // Already connected
}
Serial.print("Connecting to MQTT... ");
uint8_t retries = 3;
while ((ret = mqtt.connect()) != 0) {
Serial.println(mqtt.connectErrorString(ret));
Serial.println("Retrying MQTT connection in 5 seconds...");
mqtt.disconnect();
delay(5000);
retries--;
if (retries == 0) {
while (1);
}
}
Serial.println("MQTT Connected!");
}
const char* ssid = "."; // Nama SSID WiFi.
const char* password = "12345678"; // Password WiFi.
void setup() {
Serial.begin(115200); // Memulai komunikasi serial dengan baud rate 115200.
// Inisialisasi servo
myServo.attach(servoPin); // Hubungkan servo pertama ke pin 32.
myServo.write(0); // Atur posisi awal servo pertama di 0 derajat.
myServo2.attach(servoPin2); // Hubungkan servo kedua ke pin 33.
myServo2.write(0); // Servo mulai di posisi 0 derajat
Serial.println();
Serial.print("Connecting to WiFi: ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
// Inisialisasi RTC
if (!rtc.begin()) { // Jika RTC tidak terdeteksi.
Serial.println("RTC tidak terdeteksi!");
while (1);
}
// Jika RTC kehilangan daya, set ulang waktu
if (rtc.lostPower()) {
Serial.println("RTC kehilangan daya, mengatur ulang waktu.");
rtc.adjust(DateTime(2024, 12, 03, 15, 40, 0)); // Set waktu awal (UTC)
}
Serial.println("WiFi Connected");
mqtt.subscribe(&servo2);
}
void loop() {
MQTT_connect();
DateTime now = rtc.now(); // Baca waktu sekarang dari RTC
Adafruit_MQTT_Subscribe *subscription;
while ((subscription = mqtt.readSubscription(10000))) {
if (subscription == &servo2) {
String message = String((char *)servo2.lastread);
if (message == "ON") { // Kontrol servo berdasarkan pesan.
myServo.write(90); // Gerakkan servo pertama ke 90 derajat.
Serial.println("Servo ON - Moving to 90 degrees");
} else if (message == "OFF") {
myServo.write(0); // Kembalikan servo pertama ke 0 derajat.
Serial.println("Servo OFF - Returning to 0 degrees");
}
}
}
// Tambahkan offset WIB (+7 jam)
int wibHour = now.hour() + 7;
if (wibHour >= 24) wibHour -= 24; // Pastikan jam tetap dalam format 24-jam
Serial.print("Waktu sekarang (WIB): ");
Serial.print(wibHour); // Tampilkan jam.
Serial.print(":");
Serial.println(wibMenit); // Tampilkan menit.
// Cek apakah waktu sesuai dengan target
if (wibHour == 15 && wibMenit == 20) {
Serial.println("Waktu target tercapai, gerakkan servo.");
myServo2.write(180); // Gerakkan servo ke 90 derajat
delay(5000); // Tunggu 5 detik
delay(60000); // Tunggu 1 menit agar tidak berulang dalam menit yang sama
}
if (wibHour == 15 && wibMenit == 21) {
Serial.println("Pukul 15:21 WIB, servo kembali ke 0 derajat.");
myServo2.write(0); // Kembali ke posisi 0 derajat
delay(60000); // Tunggu 1 menit agar tidak berulang dalam menit yang sama
}
delay(1000); // Delay untuk menghindari pembacaan terlalu cepat
}