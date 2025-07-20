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

  int echo_pin = -1;
  int trigger_pin = -1;
  double distance_adjustment = 0;
  const double magic_factor_converter = 3141.592653589793238;
  const uint32_t time_period_ = 5 * 1000;
};

}; // namespace water_magic
}; // namespace esphome

