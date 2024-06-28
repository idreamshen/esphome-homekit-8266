#include "homekit_component.h"

#include <algorithm>
#include <cctype>

#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include <arduino_homekit_server.h>

extern "C" homekit_accessory_t* new_switch_accessory(unsigned int id, bool is_primary, 
  homekit_characteristic_t* cha_name, homekit_characteristic_t* cha_switch_on);
extern "C" homekit_accessory_t* new_climate_accessory(unsigned int id,
    bool is_primary, 
    homekit_characteristic_t* cha_name,
    homekit_characteristic_t* cha_ac_active, 
    homekit_characteristic_t* cha_current_temp,
    homekit_characteristic_t* cha_current_heater_cooler_state,
    homekit_characteristic_t* cha_target_heater_cooler_state,
    homekit_characteristic_t* cha_cooling_threshold_temp,
    homekit_characteristic_t* cha_heating_threshold_temp,
    homekit_characteristic_t* cha_rotation_speed
    );

namespace esphome {
namespace homekit {

  static const char *const TAG = "homekit"; 

  void HomekitComponent::setup() {
    ESP_LOGD(TAG, "HomekitComponent::setup");

    std::string mac = esphome::get_mac_address();
    if (mac.length() < 4) {
      this->setup_id_ = std::string("XY24");
    } else {
      std::string setup_id = mac.substr(mac.length() - 4);
      std::transform(setup_id.begin(), setup_id.end(), setup_id.begin(), ::toupper);
      this->setup_id_ = setup_id;
    }
    

    // force reset
    // homekit_server_reset();
  }

  void HomekitComponent::dump_config() {
    ESP_LOGCONFIG(TAG, "Homekit:");
    ESP_LOGCONFIG(TAG, "  Setup Code: %s", this->setup_code_);
    ESP_LOGCONFIG(TAG, "  Setup Id: %s", this->setup_id_.c_str());
  };

  void HomekitComponent::update() {
    ESP_LOGD(TAG, "HomekitComponent::update");

    // if (!this->is_all_switches_inited_) {
    //   if (App.get_switches().size() > 0) {
    //     this->is_all_switches_inited_ = true;

    //     for (auto *obj : App.get_switches()) {
    //       ESP_LOGD(TAG, "found switch");

    //       HomekitSwitch* homekit_switch = new HomekitSwitch(obj);
    //       this->homekit_switches_.push_back(homekit_switch);
    //       bool is_primary = this->accessories_.size() == 0;
    //       homekit_accessory_t *accessory = new_switch_accessory(this->accessories_.size() + 1, is_primary,
    //         homekit_switch->get_cha_name(),
    //         homekit_switch->get_cha_switch_on());
    //       this->accessories_.push_back(accessory);
    //     }
    //   }
    // }

    if (!this->is_all_climates_inited_) {
      if (App.get_climates().size() > 0) {
        this->is_all_climates_inited_ = true;
        for (auto *obj : App.get_climates()) {
          HomekitClimate* homekit_climate = new HomekitClimate(obj);
          this->homekit_climates_.push_back(homekit_climate);
          bool is_primary = this->accessories_.size() == 0;
          homekit_accessory_t *accessory = new_climate_accessory(this->accessories_.size() + 1, is_primary, 
            homekit_climate->get_cha_name(), 
            homekit_climate->get_cha_ac_active(),
            homekit_climate->get_cha_current_temp(),
            homekit_climate->get_cha_current_heater_cooler_state(),
            homekit_climate->get_cha_target_heater_cooler_state(),
            homekit_climate->get_cha_cooling_threshold_temp(),
            homekit_climate->get_cha_heating_threshold_temp(),
            homekit_climate->get_cha_rotation_speed()
            );
          this->accessories_.push_back(accessory);
        }
      }
    }

    if (!this->is_homekit_srv_started_) {
      if (this->is_all_climates_inited_) {
        this->accessories_.push_back(NULL);

        this->homekit_srv_config_ = {
          .accessories = this->accessories_.data(),
          .password = (char*)this->setup_code_,
          .setupId = (char*)this->setup_id_.c_str()
        };

        arduino_homekit_setup(&this->homekit_srv_config_);
        this->is_homekit_srv_started_ = true;
      }
    }
    
    
  };

  void HomekitComponent::loop() {
    arduino_homekit_loop();
  }

}
}