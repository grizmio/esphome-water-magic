#pragma once

#include <Arduino.h>
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace water_level_sensor {

class WaterLevelSensor : public sensor::Sensor, public PollingComponent {
 public:
  WaterLevelSensor(const char* name);

  void update() override;

  void setup() override;
  void dump_config() override;

  float get_setup_priority() const override;
  void set_echo_pin(int pin);
  void set_trigger_pin(int pin);


  int echo_pin = -1;
  int trigger_pin = -1;
  const char* name;
  uint32_t time_period_ = 5 * 1000;
  
};

};  // namespace water_level_sensor
};  // namespace esphome
// edwin