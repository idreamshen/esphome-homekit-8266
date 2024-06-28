#pragma once

#include <homekit/homekit.h>
#include <homekit/characteristics.h>
#include "esphome/components/climate/climate.h"

extern "C" homekit_characteristic_t* new_cha_name(const char* name);
extern "C" homekit_characteristic_t* new_cha_on();
extern "C" homekit_characteristic_t* new_cha_ac_active();
extern "C" homekit_characteristic_t* new_cha_current_temp();
extern "C" homekit_characteristic_t* new_cha_current_heater_cooler_state();
extern "C" homekit_characteristic_t* new_cha_target_heater_cooler_state();
extern "C" homekit_characteristic_t* new_cha_cooling_threshold_temp();
extern "C" homekit_characteristic_t* new_cha_heating_threshold_temp();
extern "C" homekit_characteristic_t* new_cha_rotation_speed();

namespace esphome {
namespace homekit {

static uint8_t name_idx = 0;

class HomekitClimate {

    static void cha_ac_active_setter(homekit_characteristic_t *ch, homekit_value_t value) {
        bool on = value.bool_value;
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        ESP_LOGI("TAG", "Climate active: %s", on ? "ON" : "OFF");

        auto climate_call = instance->get_bind_climate()->make_call();

        if (!on) {
            climate_call.set_mode(climate::ClimateMode::CLIMATE_MODE_OFF);
        }

        climate_call.perform();

        instance->get_cha_ac_active()->value = value;
        homekit_characteristic_notify(instance->get_cha_ac_active(), instance->get_cha_ac_active()->value);
    }

    static homekit_value_t cha_ac_active_getter(const homekit_characteristic_t *ch) {
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        homekit_value_t result = HOMEKIT_UINT8_CPP(0);

        if (instance->get_bind_climate()->mode != climate::ClimateMode::CLIMATE_MODE_OFF) {
            result = HOMEKIT_UINT8_CPP(1);
        }

        return result;
    }

    static homekit_value_t cha_current_temp_getter(const homekit_characteristic_t *ch) {
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        homekit_value_t result = HOMEKIT_FLOAT_CPP(21);

        if (!std::isnan(instance->get_bind_climate()->current_temperature) && instance->get_bind_climate()->current_temperature != 0) {
            instance->get_cha_current_temp()->value = HOMEKIT_FLOAT_CPP(instance->get_bind_climate()->current_temperature);
        }

        ESP_LOGI("TAG", "Received read for Climate temp %f", result.float_value);

        return result;
    }

    static homekit_value_t cha_current_heater_cooler_state_getter(const homekit_characteristic_t *ch) {
        //INACTIVE=0, IDLE=1, HEATING=2, COOLING=3
        homekit_value_t result = HOMEKIT_UINT8_CPP(0);
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);

        switch (instance->get_bind_climate()->mode) {
            case climate::ClimateMode::CLIMATE_MODE_HEAT:
                result = HOMEKIT_UINT8_CPP(2);
                break;
            case climate::ClimateMode::CLIMATE_MODE_COOL:
                result = HOMEKIT_UINT8_CPP(3);
                break;
            case climate::ClimateMode::CLIMATE_MODE_OFF:
                result = HOMEKIT_UINT8_CPP(0);
                break;
            default:
                break;
        }

        ESP_LOGI("TAG", "Received read for Climate current state %d", result.uint8_value);

        return result;
    }

    static homekit_value_t cha_target_heater_cooler_state_getter(const homekit_characteristic_t *ch) {
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        homekit_value_t result = HOMEKIT_UINT8_CPP(2);

        switch (instance->get_bind_climate()->mode) {
            case climate::ClimateMode::CLIMATE_MODE_HEAT:
                result = HOMEKIT_UINT8_CPP(1);
                break;
            case climate::ClimateMode::CLIMATE_MODE_COOL:
                result = HOMEKIT_UINT8_CPP(2);
                break;
            default:
                break;
        }

        ESP_LOGI("TAG", "Received read for Climate target state %d", result.uint8_value);

        return result;
    }

    static void cha_target_heater_cooler_state_setter(homekit_characteristic_t *ch, homekit_value_t value) {
        uint8_t mode = value.uint8_value;
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        ESP_LOGI("TAG", "Received Write for Climate Target Heater Cooler State %d", mode);

        auto climate_call = instance->get_bind_climate()->make_call();

        if (mode == 1) {
            climate_call.set_mode(climate::ClimateMode::CLIMATE_MODE_HEAT);
        } else if (mode == 2) {
            climate_call.set_mode(climate::ClimateMode::CLIMATE_MODE_COOL);
        }

        climate_call.perform();

        instance->get_cha_target_heater_cooler_state()->value = value;
        homekit_characteristic_notify(instance->get_cha_target_heater_cooler_state(), 
            instance->get_cha_target_heater_cooler_state()->value);
    }

    static homekit_value_t cha_cooling_threshold_temp_getter(const homekit_characteristic_t *ch) {
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        homekit_value_t result = HOMEKIT_FLOAT_CPP(26);

        if (!std::isnan(instance->get_bind_climate()->target_temperature) && instance->get_bind_climate()->target_temperature != 0) {
            result = HOMEKIT_FLOAT_CPP(instance->get_bind_climate()->target_temperature);
        }

        ESP_LOGI("TAG", "Received read for Climate cooling threshold temp %f", result.float_value);

        return result;
    }

    static void cha_cooling_threshold_temp_setter(homekit_characteristic_t *ch, homekit_value_t value) {
        float temp = value.float_value;
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        ESP_LOGI("TAG", "Received Write for Climate cooling threshold temp %f", temp);

        auto climate_call = instance->get_bind_climate()->make_call();

        climate_call.set_target_temperature(temp);
        climate_call.perform();

        instance->get_cha_cooling_threshold_temp()->value = value;
        homekit_characteristic_notify(instance->get_cha_cooling_threshold_temp(), 
            instance->get_cha_cooling_threshold_temp()->value);
    }

    static homekit_value_t cha_heating_threshold_temp_getter(const homekit_characteristic_t *ch) {
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);

        if (std::isnan(instance->get_bind_climate()->target_temperature)) {
            instance->get_cha_heating_threshold_temp()->value = HOMEKIT_FLOAT_CPP(26);
        } else {
            instance->get_cha_heating_threshold_temp()->value = HOMEKIT_FLOAT_CPP(instance->get_bind_climate()->target_temperature);
        }

        ESP_LOGI("TAG", "Received read for Climate heating threshold temp %f", instance->get_cha_heating_threshold_temp()->value.float_value);

        return instance->get_cha_heating_threshold_temp()->value;
    }

    static void cha_heating_threshold_temp_setter(homekit_characteristic_t *ch, homekit_value_t value) {
        float temp = value.float_value;
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        ESP_LOGI("TAG", "Received Write for Climate heating threshold temp %f", temp);

        auto climate_call = instance->get_bind_climate()->make_call();

        climate_call.set_target_temperature(temp);
        climate_call.perform();

        instance->get_cha_heating_threshold_temp()->value = value;
        homekit_characteristic_notify(instance->get_cha_heating_threshold_temp(), 
            instance->get_cha_heating_threshold_temp()->value);
    }

    static homekit_value_t cha_rotation_speed_getter(const homekit_characteristic_t *ch) {
        homekit_value_t result = HOMEKIT_FLOAT_CPP(0);
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);

        if (instance->get_bind_climate()->fan_mode.has_value()) {
            switch (instance->get_bind_climate()->fan_mode.value()) {
                case climate::CLIMATE_FAN_AUTO:
                    result = HOMEKIT_FLOAT_CPP(100);
                    break;
                case climate::CLIMATE_FAN_LOW:
                    result = HOMEKIT_FLOAT_CPP(33);
                    break;
                case climate::CLIMATE_FAN_MEDIUM:
                    result = HOMEKIT_FLOAT_CPP(66);
                    instance->get_cha_rotation_speed()->value.float_value = 66;
                    break;
                case climate::CLIMATE_FAN_HIGH:
                    result = HOMEKIT_FLOAT_CPP(99);
                    break;
                default:
                    break;
            }
        }

        ESP_LOGI("TAG", "Received read for Climate speed %d", result.float_value);

        return result;
    }

    static void cha_rotation_speed_setter(homekit_characteristic_t *ch, homekit_value_t value) {
        uint8_t speed = value.float_value;
        HomekitClimate* instance = static_cast<HomekitClimate*>(ch->context);
        ESP_LOGI("TAG", "Received Write for Climate Speed %f", speed);

        auto climate_call = instance->get_bind_climate()->make_call();

        if (speed == 100) {
            climate_call.set_fan_mode(climate::CLIMATE_FAN_AUTO);
        } else if (speed >= 0 && speed <= 33) {
            climate_call.set_fan_mode(climate::CLIMATE_FAN_LOW);
        } else if (speed > 33 && speed <= 66) {
            climate_call.set_fan_mode(climate::CLIMATE_FAN_MEDIUM);
        } else if (speed > 66 && speed < 100) {
            climate_call.set_fan_mode(climate::CLIMATE_FAN_HIGH);
        }

        climate_call.perform();

        instance->get_cha_rotation_speed()->value = value;
        homekit_characteristic_notify(instance->get_cha_rotation_speed(), 
            instance->get_cha_rotation_speed()->value);
    }

    public:
    HomekitClimate(climate::Climate* bind_climate) {
        this->name_ = esphome::str_sprintf("climate%d", name_idx);
        name_idx = name_idx + 1;
        this->bind_climate_ = bind_climate;
        this->cha_name_ = new_cha_name(bind_climate->get_name().c_str());
        this->cha_ac_active_ = new_cha_ac_active();
        this->cha_ac_active_->value = HOMEKIT_UINT8_CPP(0);
        this->cha_ac_active_->getter_ex = cha_ac_active_getter;
        this->cha_ac_active_->setter_ex = cha_ac_active_setter;
        this->cha_ac_active_->context = this;
        this->cha_current_temp_ = new_cha_current_temp();
        this->cha_current_temp_->getter_ex = cha_current_temp_getter;
        this->cha_current_temp_->context = this;
        this->cha_current_heater_cooler_state_ = new_cha_current_heater_cooler_state();
        this->cha_current_heater_cooler_state_->getter_ex = cha_current_heater_cooler_state_getter;
        this->cha_current_heater_cooler_state_->context = this;
        this->cha_target_heater_cooler_state_ = new_cha_target_heater_cooler_state();
        this->cha_target_heater_cooler_state_->value = HOMEKIT_UINT8_CPP(2);
        this->cha_target_heater_cooler_state_->getter_ex = cha_target_heater_cooler_state_getter;
        this->cha_target_heater_cooler_state_->setter_ex = cha_target_heater_cooler_state_setter;
        this->cha_target_heater_cooler_state_->context = this;
        this->cha_cooling_threshold_temp_ = new_cha_cooling_threshold_temp();
        this->cha_cooling_threshold_temp_->getter_ex = cha_cooling_threshold_temp_getter;
        this->cha_cooling_threshold_temp_->setter_ex = cha_cooling_threshold_temp_setter;
        this->cha_cooling_threshold_temp_->context = this;
        this->cha_heating_threshold_temp_ = new_cha_heating_threshold_temp();
        this->cha_heating_threshold_temp_->getter_ex = cha_heating_threshold_temp_getter;
        this->cha_heating_threshold_temp_->setter_ex = cha_heating_threshold_temp_setter;
        this->cha_heating_threshold_temp_->context = this;
        this->cha_rotation_speed_ = new_cha_rotation_speed();
        this->cha_rotation_speed_->getter_ex = cha_rotation_speed_getter;
        this->cha_rotation_speed_->setter_ex = cha_rotation_speed_setter;
        this->cha_rotation_speed_->context = this;
    };

    climate::Climate* get_bind_climate() { return this->bind_climate_; };
    homekit_characteristic_t* get_cha_name() { return this->cha_name_; };
    homekit_characteristic_t* get_cha_ac_active() { return this->cha_ac_active_; };
    homekit_characteristic_t* get_cha_current_temp() { return this->cha_current_temp_; };
    homekit_characteristic_t* get_cha_current_heater_cooler_state() { return this->cha_current_heater_cooler_state_; };
    homekit_characteristic_t* get_cha_target_heater_cooler_state() { return this->cha_target_heater_cooler_state_; };
    homekit_characteristic_t* get_cha_cooling_threshold_temp() { return this->cha_cooling_threshold_temp_; };
    homekit_characteristic_t* get_cha_heating_threshold_temp() { return this->cha_heating_threshold_temp_; };
    homekit_characteristic_t* get_cha_rotation_speed() { return this->cha_rotation_speed_; };


    protected:
    climate::Climate* bind_climate_;
    std::string name_;
    homekit_characteristic_t* cha_name_;
    homekit_characteristic_t* cha_ac_active_;
    homekit_characteristic_t* cha_current_temp_;
    homekit_characteristic_t* cha_current_heater_cooler_state_;
    homekit_characteristic_t* cha_target_heater_cooler_state_;
    homekit_characteristic_t* cha_cooling_threshold_temp_;
    homekit_characteristic_t* cha_heating_threshold_temp_;
    homekit_characteristic_t* cha_rotation_speed_;

};

}
}