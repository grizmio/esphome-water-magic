#pragma once

#include <Arduino.h>
#include "esphome/core/log.h"

class Meador {
public:
  int echo_pin;
  int trigger_pin;
  double distance;
  bool setuped = false;
  const char* TAG;

  Meador(const char* tag) : echo_pin(-1), trigger_pin(-1), distance(0.0), TAG(tag) {}


  void setup() {
    ESP_LOGD(this->TAG, "setup() begin");
    if (this->echo_pin == -1 || this->trigger_pin == -1) {
      ESP_LOGD(this->TAG, "pines no inicializados, no es posible medir");
      return;
    }
    pinMode(this->trigger_pin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(this->echo_pin, INPUT);     // Sets the echoPin as an INPUT
    // this->distance = this->get_distance();
    this->setuped = true;
  }
  unsigned long get_duration() {
    int t_out = 1000 * 1000;
    digitalWrite(this->echo_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(this->trigger_pin, HIGH);
    delayMicroseconds(20);
    unsigned long p = pulseIn(this->echo_pin, HIGH); //, t_out);
    return p;
  }

  double get_distance(int meas_count) {
    // ESP_LOGD(TAG, "get_distance() inicio");
    unsigned long avg = get_duration();
    // ESP_LOGD(TAG, "get_distance()| distancia inicial: %lu", avg);
    size_t i = 0;

    unsigned long duration;
    do {
      duration = get_duration();
      avg = (avg + duration) / 2;
      if (meas_count > 1) {
        vTaskDelay(pdMS_TO_TICKS(1));
      }
      i++;
    } while (i < meas_count);
    double distance =
        avg * 0.038 / 2; // Speed of sound wave divided by 2 (go and back)

    // ESP_LOGD(TAG, "get_distance()| distancia final| avg: %lu", avg);
    // ESP_LOGD(TAG, "get_distance()| distancia final| distance: %lf",
    // distance);
    return distance;
  }

  void set_distance() { this->distance = this->get_distance(3); }
};
