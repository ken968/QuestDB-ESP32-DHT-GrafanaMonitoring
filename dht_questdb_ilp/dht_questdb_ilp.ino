#include <WiFi.h>
#include <DHT.h>
#include <ArduinoHttpClient.h>

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_PASSWORD";
const char* questdbHost = "192.168.100.11"; // ip static
const int questdbPort = 9009;

#define DHTPIN 4
#define DHTTYPE DHT22

const char* DEVICE_ID = "ESP32_KAMAR_A";
const long SEND_INTERVAL = 15000; //15 detik

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
long lastSendTime = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();

  Serial.print("Menghubungkan ke WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi Terhubung!");
    Serial.print("Alamat IP ESP32: ");
    Serial.println(WiFi.localIP());
    Serial.println("--------------------------------");
}

void sendToQuestDB(float temp, float hum) {
    if (!client.connect(questdbHost, questdbPort)) {
        Serial.println("Koneksi QuestDB GAGAL!");
        return;
    }

    // QuestDB menggunakan InfluxDB Line Protocol (ILP)
    // Format: <table_name>,<tag_set> <field_set>
    
    // Tag: device_id=ESP32_KAMAR_A
    // Field: temperature=25.5,humidity=60.1
    
    String payload = "dht_sensors";
    payload += ",device_id=";
    payload += DEVICE_ID;
    payload += " temperature=";
    payload += String(temp, 2);
    payload += ",humidity=";
    payload += String(hum, 2);
    payload += "\n"; // BARIS BARU WAJIB untuk mengakhiri protokol ILP

    Serial.print("Mengirim: ");
    Serial.print(payload);

    client.print(payload);
    client.stop(); // Tutup koneksi TCP
    Serial.println(" [Terkirim!]");
}

void loop() {
  if (millis() - lastSendTime > SEND_INTERVAL) {
        float humidity = dht.readHumidity();
        float temperature = dht.readTemperature();

        if (isnan(humidity) || isnan(temperature)) {
            Serial.println("Gagal membaca dari sensor DHT!");
            return;
        }

        Serial.print("Pembacaan: Suhu=");
        Serial.print(temperature);
        Serial.print("C, Kelembaban=");
        Serial.print(humidity);
        Serial.println("%");

        sendToQuestDB(temperature, humidity);

        lastSendTime = millis();
    }
}
