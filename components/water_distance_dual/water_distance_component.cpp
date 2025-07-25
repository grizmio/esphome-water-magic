#include "water_distance_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace water_distance {

static const char *const TAG = "water_distance";

void WaterDistanceComponent::update() {
  ESP_LOGD(TAG, "update()");
  if (this->temperature_sensor_ != nullptr)
    this->temperature_sensor_->publish_state(12);
  if (this->humidity_sensor_ != nullptr)
    this->humidity_sensor_->publish_state(34);
  this->status_clear_warning();
}
void WaterDistanceComponent::setup() {
  ESP_LOGCONFIG(TAG, "Running setup");
}
void WaterDistanceComponent::dump_config() {
  ESP_LOGD(TAG, "WaterDistance:");
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
}
float WaterDistanceComponent::get_setup_priority() const {
  return setup_priority::IO;
}

} // namespace water_distance
} // namespace esphome
