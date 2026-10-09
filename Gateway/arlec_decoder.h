#pragma once

#include "esphome.h"
#include "esphome/components/remote_base/remote_base.h"
#include "esphome/components/remote_receiver/remote_receiver.h"
#include "esphome/components/cc1101/cc1101.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace arlec {

class ArlecDecoder;
inline ArlecDecoder *global_arlec_decoder = nullptr;

class ArlecDecoder : public Component, public remote_base::RemoteReceiverListener {
 public:
  remote_receiver::RemoteReceiverComponent *receiver{nullptr};
  uint32_t *remote_id_var{nullptr};
  text_sensor::TextSensor *remote_id_sensor{nullptr};
  text_sensor::TextSensor *learn_status_sensor{nullptr};
  binary_sensor::BinarySensor *learning_binary_sensor{nullptr};
  cc1101::CC1101Component *cc1101_comp{nullptr};

  bool learning_active{false};
  uint32_t learn_start_time{0};

  void setup() override {
    global_arlec_decoder = this;
    if (this->receiver != nullptr) {
      this->receiver->register_listener(this);
      ESP_LOGI("arlec_learn", "Registered listener with RF receiver.");
    } else {
      ESP_LOGE("arlec_learn", "RF receiver is null!");
    }
    if (this->remote_id_sensor != nullptr && this->remote_id_var != nullptr) {
      char buf[16];
      snprintf(buf, sizeof(buf), "0x%05" PRIX32, *this->remote_id_var);
      this->remote_id_sensor->publish_state(buf);
      ESP_LOGI("arlec_learn", "Current Arlec Remote ID: %s", buf);
    }
    if (this->learn_status_sensor != nullptr) {
      this->learn_status_sensor->publish_state("Idle");
    }
    if (this->learning_binary_sensor != nullptr) {
      this->learning_binary_sensor->publish_state(false);
    }
    ESP_LOGI("arlec_learn", "ArlecDecoder initialized successfully.");
  }

  void start_learning() {
    this->learning_active = true;
    this->learn_start_time = millis();
    ESP_LOGI("arlec_learn", ">>> Starting 30-second Learning Window <<<");
    if (this->learn_status_sensor != nullptr) {
      this->learn_status_sensor->publish_state("Listening (press any button on remote)...");
    }
    if (this->learning_binary_sensor != nullptr) {
      this->learning_binary_sensor->publish_state(true);
    }
    if (this->cc1101_comp != nullptr) {
      this->cc1101_comp->begin_rx();
      ESP_LOGI("arlec_learn", "CC1101 switched to RX mode.");
    } else {
      ESP_LOGE("arlec_learn", "CC1101 component is null!");
    }
  }

  void stop_learning(bool success = false, uint32_t learned_id = 0) {
    this->learning_active = false;
    if (this->learning_binary_sensor != nullptr) {
      this->learning_binary_sensor->publish_state(false);
    }
    if (this->cc1101_comp != nullptr) {
      this->cc1101_comp->set_idle();
      ESP_LOGI("arlec_learn", "CC1101 returned to IDLE mode.");
    }
    if (success) {
      char buf[48];
      snprintf(buf, sizeof(buf), "Learned 0x%05" PRIX32 " successfully!", learned_id);
      if (this->learn_status_sensor != nullptr) {
        this->learn_status_sensor->publish_state(buf);
      }
      char id_buf[16];
      snprintf(id_buf, sizeof(id_buf), "0x%05" PRIX32, learned_id);
      if (this->remote_id_sensor != nullptr) {
        this->remote_id_sensor->publish_state(id_buf);
      }
      ESP_LOGI("arlec_learn", "Successfully saved and published new Remote ID: 0x%05" PRIX32, learned_id);
    } else {
      if (this->learn_status_sensor != nullptr) {
        this->learn_status_sensor->publish_state("Timed out (no signal)");
      }
      ESP_LOGW("arlec_learn", "Learning stopped without signal.");
    }
  }

  void loop() override {
    if (this->learning_active) {
      if (millis() - this->learn_start_time > 30000) {
        ESP_LOGW("arlec_learn", "Learning window timed out (30s elapsed).");
        this->stop_learning(false);
      }
    }
  }

  bool on_receive(remote_base::RemoteReceiveData data) override {
    if (!this->learning_active) return false;

    const auto &raw = data.get_raw_data();
    if (raw.size() < 20) return false;

    ESP_LOGD("arlec_learn", "Received RF pulse stream: %d transitions", (int)raw.size());

    if (raw.size() < 66) {
      return false;
    }

    ESP_LOGI("arlec_learn", "Analyzing candidate signal with %d transitions...", (int)raw.size());

    // Iterate through raw timings looking for 33 valid bits
    for (size_t start_idx = 0; start_idx + 66 <= raw.size(); start_idx += 2) {
      uint64_t code = 0;
      bool valid = true;

      for (size_t b = 0; b < 33; b++) {
        size_t idx = start_idx + b * 2;
        int32_t mark = raw[idx];
        int32_t space = (raw[idx + 1] < 0) ? -raw[idx + 1] : raw[idx + 1];

        if (mark >= 500 && mark <= 1100 && space >= 200 && space <= 650) {
          // Logic 1
          code = (code << 1) | 1;
        } else if (mark >= 120 && mark <= 500 && space >= 650 && space <= 1250) {
          // Logic 0
          code = (code << 1) | 0;
        } else {
          valid = false;
          break;
        }
      }

      if (!valid) continue;

      // Extract fields from 33-bit code:
      uint32_t prefix = (code >> 13) & 0xFFFFF;
      uint8_t sw_id = (code >> 10) & 0x7;
      uint8_t cmd = (code >> 9) & 0x1;
      uint8_t fixed_nibble = (code >> 5) & 0xF;
      uint8_t tail_sw = (code >> 2) & 0x7;
      uint8_t tail_cmd = (code >> 1) & 0x1;
      uint8_t stop_bit = code & 0x1;

      uint8_t expected_tail_sw = (((sw_id >> 2) & 1) << 2) | (sw_id & 1);

      ESP_LOGI("arlec_learn", "Candidate 33-bit code: 0x%" PRIX64 " (Prefix: 0x%05" PRIX32 ", Sw: %d, Cmd: %d, Nibble: 0x%X, TailSw: %d, TailCmd: %d, Stop: %d)",
               code, prefix, sw_id, cmd, fixed_nibble, tail_sw, tail_cmd, stop_bit);

      if (fixed_nibble == 0x5 && tail_cmd == cmd && tail_sw == expected_tail_sw && stop_bit == 0) {
        ESP_LOGI("arlec_learn", ">>> MATCHED VALID ARLEC RC210 REMOTE! <<<");
        ESP_LOGI("arlec_learn", "Remote ID: 0x%05" PRIX32 " (Decimal: %" PRIu32 ")", prefix, prefix);

        if (this->remote_id_var != nullptr) {
          *this->remote_id_var = prefix;
        }
        this->stop_learning(true, prefix);
        return true;
      }
    }

    return false;
  }
};

}  // namespace arlec
}  // namespace esphome
