#include <Arduino.h>

#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include "water_level.h"

namespace esphome {
namespace water_magic {
static const char *TAG = "water_magic";
TaskHandle_t taskCoreXHandle;

class Meador {
public:
  int echo_pin;
  int trigger_pin;
  double distance;
  bool setuped = false;

  void setup() {
    ESP_LOGD(TAG, "setup() begin");
    if (this->echo_pin == -1 || this->trigger_pin == -1) {
      ESP_LOGD(TAG, "pines no inicializados, no es posible medir");
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
// fin clase Meador

Meador meador;

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

WaterMagic::WaterMagic() {}

void WaterMagic::update() {
  if (!meador.setuped) {
    return;
  }
  // Agregar mutex
  this->publish_state((float)(this->to_liters(meador.distance)));
}

double WaterMagic::to_liters(double distance) {
  // distance_adjustment es para ajustar la distancia medida por posibles
  // diferencias entre la distancia medida y la distancia real del estanque
  return (distance + this->distance_adjustment) * this->magic_factor_converter;
}

void WaterMagic::setup() {
  ESP_LOGD(TAG, "Setting up ':-)' ");
  ESP_LOGD(TAG, "trigpin: %i", this->trigger_pin);
  ESP_LOGD(TAG, "echoPin: %i", this->echo_pin);
  meador.echo_pin = this->echo_pin;
  meador.trigger_pin = this->trigger_pin;
  meador.setup();

  // Crear tarea para el Core 0
  xTaskCreatePinnedToCore(taskCoreX,    // Función de la tarea
                          "TareaCoreX", // Nombre de la tarea
                          10000, // Tamaño de la pila (en bytes, Arduino lo
                                 // maneja diferente a ESP-IDF)
                          (void *)&meador,  // Parámetros de la tarea
                          10,               // Prioridad de la tarea
                          &taskCoreXHandle, // Handle de la tarea (opcional)
                          1 // Core en el que se ejecutará (0 para APP_CPU)
  );
}

void WaterMagic::dump_config() {
  LOG_SENSOR(TAG, "Water Level Sensor", this);
  LOG_UPDATE_INTERVAL(this);
}

float WaterMagic::get_setup_priority() const {
  // https://api-docs.esphome.io/namespaceesphome_1_1setup__priority
  return setup_priority::IO;
}

void WaterMagic::set_echo_pin(int pin) {
  this->echo_pin = pin;
  // TODO: Agregar mutex
  meador.echo_pin = pin;
}

void WaterMagic::set_trigger_pin(int pin) {
  this->trigger_pin = pin;
  // TODO: Agregar mutex
  meador.trigger_pin = pin;
}

void WaterMagic::set_distance_adjustment(double distance_adjustment) {
  this->distance_adjustment = distance_adjustment;
}

}; // namespace water_magic
}; // namespace esphome
