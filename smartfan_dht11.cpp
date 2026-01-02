#include <DHT.h> // Mengimpor library DHT untuk bekerja dengan sensor DHT11.
DHT dht(2, DHT11); // Mendeklarasikan objek DHT dengan pin data di pin 2 dan jenis
sensor DHT11.
#define fan 5 // Mendefinisikan pin 5 sebagai pin yang terhubung dengan kipas.
void setup(){
Serial.begin(9600); // Memulai komunikasi serial dengan baud rate 9600 untuk
debugging.
pinMode(fan, OUTPUT); // Mengatur pin fan (pin 5) sebagai output.
digitalWrite(fan, LOW); // Memastikan fan dalam keadaan mati pada awal program.
dht.begin(); // Memulai fungsi sensor DHT.
}
void loop(){
// Membaca nilai kelembapan dan suhu dari sensor DHT11.
float kelembaban = dht.readHumidity(); // Membaca nilai kelembapan dari sensor DHT11.
float suhu = dht.readTemperature(); // Membaca nilai suhu dari sensor DHT11.
Serial.println(suhu); // Mencetak nilai suhu ke monitor serial untuk debugging.
delay(100); // Memberikan jeda selama 100 ms antara pembacaan data
sensor.
// Mengontrol fan berdasarkan nilai suhu.
if (suhu >= 30){ // Jika suhu mencapai atau melebihi 30 derajat Celsius,
digitalWrite(fan, HIGH); // Hidupkan kipas.
}
else { // Jika suhu di bawah 30 derajat Celsius,
digitalWrite(fan, LOW); // Matikan kipas.
}