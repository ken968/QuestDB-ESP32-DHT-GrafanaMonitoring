# QuestDB-ESP32-DHT-GrafanaMonitoring
Sistem monitoring suhu dan kelembaban real-time menggunakan ESP32, penyimpanan Time Series dengan QuestDB via ILP, dan visualisasi Grafana

1. Daftar Kebutuhan (Prerequisites)
   - ESP32
   - DHT22 Sensor
   - Docker Desktop (untuk start QuestDB & Grafana)
   - Grafana
   - Questdb
   - Arduino IDE
   - Library (DHT sensor library, Adafruit Unified Sensor, ArduinoHttpClient)

2. Setup Database & Visualisasi (Docker Compose)
   menggunakan Docker Compose untuk menjalankan QuestDB dan Grafana secara instan.

   *  Jalankan Stack:
     
        ```bash
        docker-compose up -d
        ```
   
    *  Akses Konsol:
      
        i  QuestDB Web Console: `http://localhost:9000`
       
        ii Grafana Dashboard: `http://localhost:3000` (user/pass: admin/admin) 
 
    *  Buat Tabel di QuestDB:
       Akses QuestDB Web Console untuk buat tabel data sensor:
        
       ```sql
       CREATE TABLE dht_sensor_data (
           timestamp TIMESTAMP,
           temperature DOUBLE,
           humidity DOUBLE
       ) TIMESTAMP (timestamp);
       ```
    
3. Setup ESP32
   pin --> 4

   * konfigurasi source code
     
     i. uae file dhtquestdb.ino
     
     ii. update ssid & passwd
     
     iii. update questdb host (set ke ip lokal yg menjalankan docker)
     
5. Setup Grafana
   Plugin QuestDB Data Source sudah terinstal otomatis (karna `docker-compose.yml`).
   kalau mau hanya install pluggin use 'docker run -p 3000:3000 -e GF_INSTALL_PLUGINS=questdb-questdb-datasource grafana/grafana-oss'

   * Tambahkan Data Source: Di Grafana, tambahkan Data Source baru:
     
      i. Tipe: QuestDB
     
      ii Host: `http://questdb:8812` (Gunakan nama *service* Docker)
     
   * Import Dashboard: Impor file `Grafana_Setup/dashboard.json` (jika disediakan) atau buat dashboard secara manual menggunakan Query SQL:
  
      ```sql
      SELECT
        timestamp,
        temperature,
        humidity
      FROM dht_sensor_data
      WHERE timestamp IN (@__timeFilter)
      ORDER BY timestamp DESC
      ```
      
8. Hasil (Dasboard monitoring)
   ![Dashboard Monitoring DHT22](grafana_dashboard.png)
