// #pragma once

// #include <homekit/homekit.h>
// #include <homekit/characteristics.h>
// #include "esphome/components/switch/switch.h"

// extern "C" homekit_characteristic_t* new_cha_switch_on();
// extern "C" homekit_characteristic_t* new_cha_name(const char* name);


// namespace esphome {
// namespace homekit {

// class HomekitSwitch {
//     public:
//     HomekitSwitch(switch_::Switch* bind_switch) {
//         this->bind_switch_ = bind_switch;
//         this->cha_switch_on_ = new_cha_on();
//         this->cha_switch_on_->setter_ex = cha_switch_on_setter;
//         this->cha_switch_on_->context = this;
//         this->cha_name_ = new_cha_name(bind_climate->get_name().c_str());
//     };

//     static void cha_switch_on_setter(homekit_characteristic_t *ch, homekit_value_t value) {
//         bool on = value.bool_value;
//         HomekitSwitch* instance = static_cast<HomekitSwitch*>(ch->context);
//         // cha_switch_on.value.bool_value = on;	//sync the value
//         ESP_LOGD("TAG", "Switch: %s", on ? "ON" : "OFF");
//     }

//     homekit_characteristic_t* get_cha_switch_on() { return this->cha_switch_on_; };
//     homekit_characteristic_t* get_cha_name() { return this->cha_name_; };

//     protected:
//     switch_::Switch* bind_switch_;
//     homekit_characteristic_t* cha_switch_on_;
//     homekit_characteristic_t* cha_name_;

// };

// }
// }