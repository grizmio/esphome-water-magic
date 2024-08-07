#pragma once

#include "esphome/core/component.h"
#include "esphome/core/time.h"
#include "esphome/components/gpio/output/gpio_binary_output.h"

namespace esphome {
namespace seven_segment {

class SEVENSEGMENTComponent;

using seven_segment_writer_t = std::function<void(SEVENSEGMENTComponent &)>;

class SEVENSEGMENTComponent : public PollingComponent {
 public:
  void set_writer(seven_segment_writer_t &&writer);

  void setup() override;

  void dump_config() override;

  void update() override;

  float get_setup_priority() const override;

  void display();

  // void set_num_chips(uint8_t num_chips);
  void set_a_pin(gpio::GPIOBinaryOutput *a_pin);
  void set_b_pin(gpio::GPIOBinaryOutput *b_pin);
  void set_c_pin(gpio::GPIOBinaryOutput *c_pin);
  void set_d_pin(gpio::GPIOBinaryOutput *d_pin);
  void set_e_pin(gpio::GPIOBinaryOutput *e_pin);
  void set_f_pin(gpio::GPIOBinaryOutput *f_pin);
  void set_g_pin(gpio::GPIOBinaryOutput *g_pin);
  void set_dp_pin(gpio::GPIOBinaryOutput *dp_pin);
  void set_g1_pin(gpio::GPIOBinaryOutput *g1_pin);
  void set_g2_pin(gpio::GPIOBinaryOutput *g2_pin);
  void set_g3_pin(gpio::GPIOBinaryOutput *g3_pin);
  void set_g4_pin(gpio::GPIOBinaryOutput *g4_pin);

  /// Evaluate the printf-format and print the result at the given position.
  uint8_t printf(uint8_t pos, const char *format, ...) __attribute__((format(printf, 3, 4)));
  /// Evaluate the printf-format and print the result at position 0.
  uint8_t printf(const char *format, ...) __attribute__((format(printf, 2, 3)));

  /// Print `str` at the given position.
  uint8_t print(uint8_t pos, const char *str);
  /// Print `str` at position 0.
  uint8_t print(const char *str);

  /// Evaluate the strftime-format and print the result at the given position.
  uint8_t strftime(uint8_t pos, const char *format, ESPTime time) __attribute__((format(strftime, 3, 0)));

  /// Evaluate the strftime-format and print the result at position 0.
  uint8_t strftime(const char *format, ESPTime time) __attribute__((format(strftime, 2, 0)));

 protected:
  void clear_display_();

  void set_digit_(uint8_t digit, uint8_t value, bool dp);

  gpio::GPIOBinaryOutput *a_pin_{nullptr};
  gpio::GPIOBinaryOutput *b_pin_{nullptr};
  gpio::GPIOBinaryOutput *c_pin_{nullptr};
  gpio::GPIOBinaryOutput *d_pin_{nullptr};
  gpio::GPIOBinaryOutput *e_pin_{nullptr};
  gpio::GPIOBinaryOutput *f_pin_{nullptr};
  gpio::GPIOBinaryOutput *g_pin_{nullptr};
  gpio::GPIOBinaryOutput *dp_pin_{nullptr};
  gpio::GPIOBinaryOutput *g1_pin_{nullptr};
  gpio::GPIOBinaryOutput *g2_pin_{nullptr};
  gpio::GPIOBinaryOutput *g3_pin_{nullptr};
  gpio::GPIOBinaryOutput *g4_pin_{nullptr};

  // uint8_t num_chips_{1};
  optional<seven_segment_writer_t> writer_{};
};

}  // namespace seven_segment
}  // namespace esphome