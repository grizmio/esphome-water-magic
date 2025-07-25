#include <Arduino.h>

#include "meador.hpp"
#include "water_distance_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace water_distance {

static const char *const TAG = "water_distance";
TaskHandle_t taskCoreXHandle;

Meador *meador;

void taskCoreX(void *pvParameters) {
  // delay(2 * 1000);
  vTaskDelay(pdMS_TO_TICKS(2 * 1000));
  // ESP_LOGD(TAG, "", esp_cpu_get_cpuid());
  Meador *meador = (Meador *)pvParameters;

  for (;;) {
    // ESP_LOGD(TAG,
    //          "Tarea corriendo en el Core X (1). Millis: %lu | echo_pin %i | "
    //          "trigger_pin %i",
    //          millis(), meador->echo_pin, meador->trigger_pin);

    meador->set_distance();
    // ESP_LOGD(TAG, "Tarea corriendo en el Core X (1). Super distancia: %lf",
    //          meador->distance);
    vTaskDelay(pdMS_TO_TICKS(2 * 1000));
  }
};

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
  meador = new Meador(TAG);
  meador->echo_pin = this->echo_pin;
  meador->trigger_pin = this->trigger_pin;
  meador->setup();

  // Crear tarea para el Core 0
  xTaskCreatePinnedToCore(taskCoreX,    // Función de la tarea
                          "TareaCoreX", // Nombre de la tarea
                          10000, // Tamaño de la pila (en bytes, Arduino lo
                                 // maneja diferente a ESP-IDF)
                          (void *)meador,  // Parámetros de la tarea
                          10,               // Prioridad de la tarea
                          &taskCoreXHandle, // Handle de la tarea (opcional)
                          1 // Core en el que se ejecutará (0 para APP_CPU)
  );
}
void WaterDistanceComponent::dump_config() {
  ESP_LOGD(TAG, "WaterDistance:");
  LOG_SENSOR(TAG, "Temperature", this->temperature_sensor_);
  LOG_SENSOR(TAG, "Humidity", this->humidity_sensor_);
}
float WaterDistanceComponent::get_setup_priority() const {
  return setup_priority::IO;
}

double WaterDistanceComponent::to_liters(double distance) {
  // distance_adjustment es para ajustar la distancia medida por posibles
  // diferencias entre la distancia medida y la distancia real del estanque
  return (distance + this->distance_adjustment) * this->distance_to_liters_factor;
}
void WaterDistanceComponent::set_echo_pin(int pin) {
  this->echo_pin = pin;
  // TODO: Agregar mutex
  meador->echo_pin = pin;
}

void WaterDistanceComponent::set_trigger_pin(int pin) {
  this->trigger_pin = pin;
  // TODO: Agregar mutex
  meador->trigger_pin = pin;
}

void WaterDistanceComponent::set_distance_adjustment(double distance_adjustment) {
  this->distance_adjustment = distance_adjustment;
}

void WaterDistanceComponent::set_distance_to_liters_factor(double distance_to_liters_factor) {
  this->distance_to_liters_factor = distance_to_liters_factor;
}

void WaterDistanceComponent::set_time_period_measurement(int time_period_measurement) {
  this->time_period_measurement = time_period_measurement;
}

} // namespace water_distance
} // namespace esphome
