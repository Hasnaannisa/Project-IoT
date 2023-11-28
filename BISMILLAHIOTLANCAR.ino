#define BLYNK_TEMPLATE_ID "TMPL6sLIBaGox"
#define BLYNK_TEMPLATE_NAME "Monitoring TempHum Kumbung"
#define BLYNK_AUTH_TOKEN "tjwCeZd7ZCYcdBHB5dq6NHPazNw-XhTw"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define DHTPIN 5          // Pin data sensor DHT22 terhubung ke pin GPIO4
#define DHTTYPE DHT22     // Tipe sensor DHT

DHT dht(DHTPIN, DHTTYPE);

char auth[] = BLYNK_AUTH_TOKEN;  // Masukkan token dari proyek Blynk Anda
char ssid[] = "Devi";  // Masukkan nama WiFi Anda
char pass[] = "gaadapas";  // Masukkan kata sandi WiFi Anda

int lampPin = 4; // Pin GPIO yang terhubung ke lampu
int thresholdTemp = 15; // Ambang batas suhu
int soilPin = 35;

BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(lampPin, OUTPUT);

  timer.setInterval(1000L, sendSensorData); // Kirim data sensor setiap detik
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int soilMoisture = analogRead(soilPin); // Baca nilai kelembaban tanah

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Gagal membaca sensor DHT!");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Kelembaban: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Kelembaban Tanah: ");
  Serial.print(soilMoisture);
  Serial.println(" %"); // Tampilkan nilai kelembaban tanah di Serial Monitor


  Blynk.virtualWrite(V0, temperature); 
  Blynk.virtualWrite(V1, humidity);
  Blynk.virtualWrite(V2, soilMoisture);

  if (temperature > thresholdTemp) {
    digitalWrite(lampPin, HIGH); // Nyala lampu jika suhu melebihi ambang batas
    Blynk.virtualWrite(V3, 255); // Update widget Blynk ke posisi ON
  } else {
    digitalWrite(lampPin, LOW); // Matikan lampu jika suhu tidak melebihi ambang batas
    Blynk.virtualWrite(V3, 0); // Update widget Blynk ke posisi OFF
  }
}
BLYNK_WRITE(V3) { // Tangani perintah dari tombol switch di pin V3 di Blynk
  int lampControl = param.asInt();
  
  if (lampControl == 0) {
    digitalWrite(lampPin, LOW); // Matikan lampu jika tombol switch di Blynk ditekan
  } else if (lampControl == 1) {
    digitalWrite(lampPin, HIGH); // Nyalakan lampu jika tombol switch di Blynk ditekan
  }
}