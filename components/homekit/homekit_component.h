#pragma once

#include <homekit/homekit.h>

#include "esphome.h"
#include "esphome/core/component.h"
// #include "homekit_switch.h"

namespace esphome {
namespace homekit {

  class HomekitComponent : public PollingComponent {
    public:
      HomekitComponent() : PollingComponent(2000) {
        
      }

      void set_setup_code(const char* setup_code) { this->setup_code_ = setup_code; };
      void set_name(const char* name) { this->name_ = name; };
      void set_model(const char* model) { this->model_ = model; };
      void set_manufacturer(const char* manufacturer) { this->manufacturer_ = manufacturer; };
      void set_serial_num(const char* serial_num) { this->serial_num_ = serial_num; };
      void set_fw_rev(const char* fw_rev) { this->fw_rev_ = fw_rev; };

      void setup() override;
      void dump_config() override;
      void update() override;
      void loop() override;
      float get_setup_priority() const { return setup_priority::AFTER_WIFI; };

    protected:
      const char* setup_code_;
      std::string setup_id_;
      const char* name_;
      const char* model_;
      const char* manufacturer_;
      const char* serial_num_;
      const char* fw_rev_;
      // bool is_all_switches_inited_{false};
      // std::vector<HomekitSwitch*> homekit_switches_;
      bool is_all_climates_inited_{false};
      std::vector<HomekitClimate*> homekit_climates_;

      bool is_homekit_srv_started_{false};
      homekit_server_config_t homekit_srv_config_;
      std::vector<homekit_accessory_t*> accessories_;

    private:
  };

}
}
