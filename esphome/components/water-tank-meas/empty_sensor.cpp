#include "empty_sensor.h"

#include "esphome/core/log.h"

namespace esphome {
  namespace empty_sensor {

    static
    const char * TAG = "empty_sensor.sensor";
    int _trigPin = 35;
    int _echoPin = 34;
    void EmptySensor::setup() {
    	pinMode(_trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
      pinMode(_echoPin, INPUT); // Sets the echoPin as an INPUT
    }

    void EmptySensor::loop() {
      int distance = get_distance();
      Serial.print("distance: ");
      Serial.println(distance);

      delay(500);
    }

    void EmptySensor::update() {}

    void EmptySensor::dump_config() {
      ESP_LOGCONFIG(TAG, "Empty custom sensor");
    }
    unsigned long get_duration() {
      digitalWrite(_trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(_trigPin, HIGH);
      delayMicroseconds(20);
      return pulseIn(_echoPin, HIGH);
    }

    int get_distance() {
      unsigned long avg = get_duration();
      size_t i = 0;
      unsigned long duration;
      size_t meas = 4;
      do {
        duration = get_duration();
        avg = (avg + duration) / 2;
        delay(110);
        i++;
      } while (i < meas);
      int distance = avg * 0.038 / 2; // Speed of sound wave divided by 2 (go and back)
      return distance;
    }

  } // namespace empty_sensor
} // namespace esphome