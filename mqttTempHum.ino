void mqttConnect() {
  while (!client.connected()) {
    Serial.print(F("Attempting MQTT connection..."));
    if (client.connect(MQTT_CLIENT)) {
      Serial.println(F("connected"));
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(client.state());
      Serial.println(F(" try again in 5 seconds"));
      delay(5000);
    }
  }
}


void readAndPublishDHT22() {
  float h1 = dht22.readHumidity();
  float t1 = dht22.readTemperature();
  if (isnan(h1) || isnan(t1)) {
    Serial.println(F("Failed to read from DHT22 sensor!"));
    sendActiveState(false);
    return;
  }
  Serial.println("DHT22 Humidity:    " + String(h1));
  Serial.println("DHT22 Temperature: " + String(t1));

  client.publish(MQTT_TOPIC_H1, String(h1).c_str(), true);
  client.publish(MQTT_TOPIC_T1, String(t1).c_str(), true); 
  sendActiveState(true);

  Serial.println(F("------------------------"));
}

void sendActiveState(boolean isActive) {
  client.publish(MQTT_TOPIC_ACTIVE, String(isActive).c_str(), true); 
}

void tempHumPublish() {
  if((millis() - lastUpdateTime) > (REPORT_DELAY_SECS * 1000)) {
    lastUpdateTime = millis();
     if (!client.connected()) {
      mqttConnect();
    }
    client.loop(); 
    readAndPublishDHT22();
  }
}

