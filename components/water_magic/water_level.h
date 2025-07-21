#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include <Arduino.h>

namespace esphome {
namespace water_magic {

class WaterMagic : public sensor::Sensor, public PollingComponent {
public:
  WaterMagic();

  void update() override;

  void setup() override;
  void dump_config() override;

  float get_setup_priority() const override;
  void set_echo_pin(int pin);
  void set_trigger_pin(int pin);
  void set_distance_adjustment(double distance_adjustment);
  void set_distance_to_liters_factor(double distance_to_liters_factor);
  double to_liters(double distance);

  int echo_pin = -1;
  int trigger_pin = -1;
  double distance_adjustment = 0;
  double distance_to_liters_factor = 0;
  uint32_t time_period_ = 5 * 1000;
};

}; // namespace water_magic
}; // namespace esphome

