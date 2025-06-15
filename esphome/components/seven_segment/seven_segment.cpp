#include "seven_segment.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace seven_segment {

static const char *const TAG = "seven_segment";

static const uint8_t SEVENSEG_UNKNOWN_CHAR = 0b00000000;

const uint8_t SEVENSEG_ASCII_TO_RAW[95] PROGMEM = {
    0b00000000,             // ' ', ord 0x20
    0b10110000,             // '!', ord 0x21
    0b00100010,             // '"', ord 0x22
    SEVENSEG_UNKNOWN_CHAR,  // '#', ord 0x23
    SEVENSEG_UNKNOWN_CHAR,  // '$', ord 0x24
    0b01001001,             // '%', ord 0x25
    SEVENSEG_UNKNOWN_CHAR,  // '&', ord 0x26
    0b00000010,             // ''', ord 0x27
    0b01001110,             // '(', ord 0x28
    0b01111000,             // ')', ord 0x29
    0b01000000,             // '*', ord 0x2A
    SEVENSEG_UNKNOWN_CHAR,  // '+', ord 0x2B
    0b00010000,             // ',', ord 0x2C
    0b00000001,             // '-', ord 0x2D
    0b10000000,             // '.', ord 0x2E
    SEVENSEG_UNKNOWN_CHAR,  // '/', ord 0x2F
    0b01111110,             // '0', ord 0x30
    0b00110000,             // '1', ord 0x31
    0b01101101,             // '2', ord 0x32
    0b01111001,             // '3', ord 0x33
    0b00110011,             // '4', ord 0x34
    0b01011011,             // '5', ord 0x35
    0b01011111,             // '6', ord 0x36
    0b01110000,             // '7', ord 0x37
    0b01111111,             // '8', ord 0x38
    0b01111011,             // '9', ord 0x39
    0b01001000,             // ':', ord 0x3A
    0b01011000,             // ';', ord 0x3B
    SEVENSEG_UNKNOWN_CHAR,  // '<', ord 0x3C
    0b00001001,             // '=', ord 0x3D
    SEVENSEG_UNKNOWN_CHAR,  // '>', ord 0x3E
    0b01100101,             // '?', ord 0x3F
    0b01101111,             // '@', ord 0x40
    0b01110111,             // 'A', ord 0x41
    0b00011111,             // 'B', ord 0x42
    0b01001110,             // 'C', ord 0x43
    0b00111101,             // 'D', ord 0x44
    0b01001111,             // 'E', ord 0x45
    0b01000111,             // 'F', ord 0x46
    0b01011110,             // 'G', ord 0x47
    0b00110111,             // 'H', ord 0x48
    0b00110000,             // 'I', ord 0x49
    0b00111100,             // 'J', ord 0x4A
    SEVENSEG_UNKNOWN_CHAR,  // 'K', ord 0x4B
    0b00001110,             // 'L', ord 0x4C
    SEVENSEG_UNKNOWN_CHAR,  // 'M', ord 0x4D
    0b00010101,             // 'N', ord 0x4E
    0b01111110,             // 'O', ord 0x4F
    0b01100111,             // 'P', ord 0x50
    0b11111110,             // 'Q', ord 0x51
    0b00000101,             // 'R', ord 0x52
    0b01011011,             // 'S', ord 0x53
    0b00000111,             // 'T', ord 0x54
    0b00111110,             // 'U', ord 0x55
    0b00111110,             // 'V', ord 0x56
    0b00111111,             // 'W', ord 0x57
    SEVENSEG_UNKNOWN_CHAR,  // 'X', ord 0x58
    0b00100111,             // 'Y', ord 0x59
    0b01101101,             // 'Z', ord 0x5A
    0b01001110,             // '[', ord 0x5B
    SEVENSEG_UNKNOWN_CHAR,  // '\', ord 0x5C
    0b01111000,             // ']', ord 0x5D
    SEVENSEG_UNKNOWN_CHAR,  // '^', ord 0x5E
    0b00001000,             // '_', ord 0x5F
    0b00100000,             // '`', ord 0x60
    0b01110111,             // 'a', ord 0x61
    0b00011111,             // 'b', ord 0x62
    0b00001101,             // 'c', ord 0x63
    0b00111101,             // 'd', ord 0x64
    0b01001111,             // 'e', ord 0x65
    0b01000111,             // 'f', ord 0x66
    0b01011110,             // 'g', ord 0x67
    0b00010111,             // 'h', ord 0x68
    0b00010000,             // 'i', ord 0x69
    0b00111100,             // 'j', ord 0x6A
    SEVENSEG_UNKNOWN_CHAR,  // 'k', ord 0x6B
    0b00001110,             // 'l', ord 0x6C
    SEVENSEG_UNKNOWN_CHAR,  // 'm', ord 0x6D
    0b00010101,             // 'n', ord 0x6E
    0b00011101,             // 'o', ord 0x6F
    0b01100111,             // 'p', ord 0x70
    SEVENSEG_UNKNOWN_CHAR,  // 'q', ord 0x71
    0b00000101,             // 'r', ord 0x72
    0b01011011,             // 's', ord 0x73
    0b00000111,             // 't', ord 0x74
    0b00011100,             // 'u', ord 0x75
    0b00011100,             // 'v', ord 0x76
    SEVENSEG_UNKNOWN_CHAR,  // 'w', ord 0x77
    SEVENSEG_UNKNOWN_CHAR,  // 'x', ord 0x78
    0b00100111,             // 'y', ord 0x79
    SEVENSEG_UNKNOWN_CHAR,  // 'z', ord 0x7A
    0b00110001,             // '{', ord 0x7B
    0b00000110,             // '|', ord 0x7C
    0b00000111,             // '}', ord 0x7D
    0b01100011,             // '~', ord 0x7E (degree symbol)
};

float SEVENSEGMENTComponent::get_setup_priority() const { return setup_priority::PROCESSOR; }
void SEVENSEGMENTComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up 7 Segment...");
  // ckeck all pins are defined
  if (!this->a_pin_ || !this->b_pin_ || !this->c_pin_ || !this->d_pin_ || !this->e_pin_ || !this->f_pin_ ||
      !this->g_pin_ || !this->dp_pin_) {
    ESP_LOGE(TAG, "Not all pins are defined.");
    return;
  }
  this->a_pin_->set_state(false);
  this->b_pin_->set_state(false);
  this->c_pin_->set_state(false);
  this->d_pin_->set_state(false);
  this->e_pin_->set_state(false);
  this->f_pin_->set_state(false);
  this->g_pin_->set_state(false);
  this->dp_pin_->set_state(false);
  this->g1_pin_->set_state(false);
  this->g2_pin_->set_state(false);
  this->g3_pin_->set_state(false);
  this->g4_pin_->set_state(false);

  // log all pin numbers and states
  ESP_LOGCONFIG(TAG, "A Pin: %u", this->a_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "B Pin: %u", this->b_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "C Pin: %u", this->c_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "D Pin: %u", this->d_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "E Pin: %u", this->e_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "F Pin: %u", this->f_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "G Pin: %u", this->g_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "DP Pin: %u", this->dp_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "G1 Pin: %u", this->g1_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "G2 Pin: %u", this->g2_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "G3 Pin: %u", this->g3_pin_->get_component_state());
  ESP_LOGCONFIG(TAG, "G4 Pin: %u", this->g4_pin_->get_component_state());
}

void SEVENSEGMENTComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "SEVENSEG:");
  // ESP_LOGCONFIG(TAG, "  Number of Digits: %u", this->num_chips_);
  LOG_UPDATE_INTERVAL(this);
}

void SEVENSEGMENTComponent::display() {
  for (uint8_t i = 0; i < 8; i++) {
    // Set the register to the digit position
  }
}

void SEVENSEGMENTComponent::update() { this->display(); }

void SEVENSEGMENTComponent::clear_display_() {
  this->a_pin_->set_state(false);
  this->b_pin_->set_state(false);
  this->c_pin_->set_state(false);
  this->d_pin_->set_state(false);
  this->e_pin_->set_state(false);
  this->f_pin_->set_state(false);
  this->g_pin_->set_state(false);
  this->dp_pin_->set_state(false);
}

void SEVENSEGMENTComponent::set_digit_(uint8_t digit, uint8_t ch, bool dot) {
  this->clear_display_();
  this->g1_pin_->set_state(digit == 0);
  this->g2_pin_->set_state(digit == 1);
  this->g3_pin_->set_state(digit == 2);
  this->g4_pin_->set_state(digit == 3);

  uint8_t segments = SEVENSEG_ASCII_TO_RAW[ch];
  this->a_pin_->set_state((segments & 0b10000000) || dot);
  this->a_pin_->set_state(segments & 0b01000000);
  this->a_pin_->set_state(segments & 0b00100000);
  this->a_pin_->set_state(segments & 0b00010000);
  this->a_pin_->set_state(segments & 0b00001000);
  this->a_pin_->set_state(segments & 0b00000100);
  this->a_pin_->set_state(segments & 0b00000010);
  this->a_pin_->set_state(segments & 0b00000001);
  delay(5);
};

uint8_t SEVENSEGMENTComponent::print(uint8_t start_pos, const char *str) {
  // print implementation
  return 0;
}

uint8_t SEVENSEGMENTComponent::print(const char *str) { return this->print(0, str); }
uint8_t SEVENSEGMENTComponent::printf(uint8_t pos, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}
uint8_t SEVENSEGMENTComponent::printf(const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  char buffer[64];
  int ret = vsnprintf(buffer, sizeof(buffer), format, arg);
  va_end(arg);
  if (ret > 0)
    return this->print(buffer);
  return 0;
}
void SEVENSEGMENTComponent::set_writer(seven_segment_writer_t &&writer) { this->writer_ = writer; }

// void SEVENSEGMENTComponent::set_num_chips(uint8_t num_chips) { this->num_chips_ = num_chips; }

void SEVENSEGMENTComponent::set_a_pin(gpio::GPIOBinaryOutput *a_pin) { this->a_pin_ = a_pin; }
void SEVENSEGMENTComponent::set_b_pin(gpio::GPIOBinaryOutput *b_pin) { this->b_pin_ = b_pin; }
void SEVENSEGMENTComponent::set_c_pin(gpio::GPIOBinaryOutput *c_pin) { this->c_pin_ = c_pin; }
void SEVENSEGMENTComponent::set_d_pin(gpio::GPIOBinaryOutput *d_pin) { this->d_pin_ = d_pin; }
void SEVENSEGMENTComponent::set_e_pin(gpio::GPIOBinaryOutput *e_pin) { this->e_pin_ = e_pin; }
void SEVENSEGMENTComponent::set_f_pin(gpio::GPIOBinaryOutput *f_pin) { this->f_pin_ = f_pin; }
void SEVENSEGMENTComponent::set_g_pin(gpio::GPIOBinaryOutput *g_pin) { this->g_pin_ = g_pin; }
void SEVENSEGMENTComponent::set_dp_pin(gpio::GPIOBinaryOutput *dp_pin) { this->dp_pin_ = dp_pin; }
void SEVENSEGMENTComponent::set_g1_pin(gpio::GPIOBinaryOutput *g1_pin) { this->g1_pin_ = g1_pin; }
void SEVENSEGMENTComponent::set_g2_pin(gpio::GPIOBinaryOutput *g2_pin) { this->g2_pin_ = g2_pin; }
void SEVENSEGMENTComponent::set_g3_pin(gpio::GPIOBinaryOutput *g3_pin) { this->g3_pin_ = g3_pin; }
void SEVENSEGMENTComponent::set_g4_pin(gpio::GPIOBinaryOutput *g4_pin) { this->g4_pin_ = g4_pin; }

uint8_t SEVENSEGMENTComponent::strftime(uint8_t pos, const char *format, ESPTime time) {
  char buffer[64];
  size_t ret = time.strftime(buffer, sizeof(buffer), format);
  if (ret > 0)
    return this->print(pos, buffer);
  return 0;
}
uint8_t SEVENSEGMENTComponent::strftime(const char *format, ESPTime time) { return this->strftime(0, format, time); }

/* old lambda function
std::string text = id(home_assistant_text).state;

          // Pad with spaces if the text is shorter than 4 characters
          while (text.length() < 4) {
            text = " " + text;
          }

          auto clear_display = [&]() {
            id(G1).set_state(0);
            id(G2).set_state(0);
            id(G3).set_state(0);
            id(G4).set_state(0);
            id(DP).set_state(0);
            id(A).set_state(0);
            id(B).set_state(0);
            id(C).set_state(0);
            id(D).set_state(0);
            id(E).set_state(0);
            id(F).set_state(0);
            id(G).set_state(0);
          };

          auto set_digit = [&](int digit, uint8_t ch, bool dot) {
            clear_display();
            id(G1).set_state(digit == 0);
            id(G2).set_state(digit == 1);
            id(G3).set_state(digit == 2);
            id(G4).set_state(digit == 3);

            uint8_t segments = char_map[ch];
            id(DP).set_state((segments & 0b10000000) || dot);
            id(A).set_state(segments & 0b01000000);
            id(B).set_state(segments & 0b00100000);
            id(C).set_state(segments & 0b00010000);
            id(D).set_state(segments & 0b00001000);
            id(E).set_state(segments & 0b00000100);
            id(F).set_state(segments & 0b00000010);
            id(G).set_state(segments & 0b00000001);
            delay(5);
          };

          int l = text.length();
          bool dot = false;
          uint8_t tmp = 0;

          // Iterate over each digit position (up to 4)
          for (int i = 0; i < 4; ++i) {

              // Check if the character is a dot
              if (text[i] == '.') {
                  // Display the dot on its own
                  set_digit(i, 0, true);
              } else {
                  // Character is not a dot, check if the next character is a dot
                  if ((i + 1 < l) && (text[i + 1] == '.')) {
                      // The next character is a dot
                      dot = true;
                      // Erase the dot, so it won't be processed again
                      text.erase(i + 1, 1);
                      // Update the length after erasure
                      l = text.length();
                  } else {
                      dot = false;
                  }

                  // Adjust the character to the segment map
                  if (text[i] < 32) {
                      tmp = 0;
                  } else {
                      tmp = text[i] - 32;
                  }

                  // Set the digit with or without the dot
                  set_digit(i, tmp, dot);
              }
          }

*/

}  // namespace seven_segment
}  // namespace esphome
