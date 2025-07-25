#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace water_distance {

class WaterDistanceComponent : public sensor::Sensor, public PollingComponent {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;
  void update() override;

  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }

  void set_echo_pin(int pin);
  void set_trigger_pin(int pin);
  void set_distance_adjustment(double distance_adjustment);
  void set_distance_to_liters_factor(double distance_to_liters_factor);
  void set_time_period_measurement(int time_period_measurement);
  double to_liters(double distance);

  int echo_pin = -1;
  int trigger_pin = -1;
  double distance_adjustment = 0;
  double distance_to_liters_factor = 0;
  int time_period_measurement = 5 * 1000;

 protected:

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
};

}  // namespace water_distance
}  // namespace esphome

