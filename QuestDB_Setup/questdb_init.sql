CREATE TABLE dht_sensors (
    timestamp TIMESTAMP,
    device_id SYMBOL,
    temperature DOUBLE,
    humidity DOUBLE
) timestamp(timestamp);

-- lihat data dlm tabel
SELECT * FROM dht_sensors;
