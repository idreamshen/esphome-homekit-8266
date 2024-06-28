#include <homekit/homekit.h>
#include <homekit/characteristics.h>

void my_accessory_identify(homekit_value_t _value) {
	printf("accessory identify\n");
}

homekit_characteristic_t* new_cha_name(const char* name) {
    homekit_characteristic_t* cha_name = NEW_HOMEKIT_CHARACTERISTIC(NAME, name);
    return cha_name;
}

homekit_characteristic_t* new_cha_on() {
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(ON, false);
    return c;
}

homekit_characteristic_t* new_cha_ac_active() {
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(ACTIVE, 0);
    return c;
}

homekit_characteristic_t* new_cha_current_temp() {
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(CURRENT_TEMPERATURE, 21,
        .min_value = (float[]) {12},
        .max_value = (float[]) {32}
    );
    return c;
}

homekit_characteristic_t* new_cha_current_heater_cooler_state() {
    //INACTIVE=0, IDLE=1, HEATING=2, COOLING=3
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(CURRENT_HEATER_COOLER_STATE, 0,
       .valid_values={.count=4, .values = (uint8_t[]) {0, 1, 2, 3}}
    );
    return c;
}

homekit_characteristic_t* new_cha_target_heater_cooler_state() {
    //0=AUTO, 1=HEAT, 2=COOL 
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(TARGET_HEATER_COOLER_STATE, 2,
        .valid_values={.count=2, .values=(uint8_t[]) {1,2}},
    );
    return c;
}

homekit_characteristic_t* new_cha_cooling_threshold_temp() {
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(COOLING_THRESHOLD_TEMPERATURE, 24, 
        .min_value  =(float[]) {12}, 
        .max_value  =(float[]) {32},
        .min_step   =(float[]) {1.0}
        );
    return c;
}

homekit_characteristic_t* new_cha_heating_threshold_temp() {
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(HEATING_THRESHOLD_TEMPERATURE, 26, 
        .min_value  =(float[]) {12}, 
        .max_value  =(float[]) {32},
        .min_step   =(float[]) {1.0}
        );
    return c;
}

homekit_characteristic_t* new_cha_rotation_speed() {
    homekit_characteristic_t* c = NEW_HOMEKIT_CHARACTERISTIC(ROTATION_SPEED, 0,
        .min_step   =(float[]) {1}
    );
    return c;
}

homekit_accessory_t* new_switch_accessory(unsigned int id, 
    bool is_primary,
    homekit_characteristic_t* cha_name,
    homekit_characteristic_t* cha_switch_on) {

    homekit_accessory_t *accessory = NEW_HOMEKIT_ACCESSORY(
        .id=id, 
        .category=homekit_accessory_category_switch, 
        .services=(homekit_service_t*[]) {
            NEW_HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
                NEW_HOMEKIT_CHARACTERISTIC(NAME, "Switch11"),
                NEW_HOMEKIT_CHARACTERISTIC(MANUFACTURER, "Arduino HomeKit"),
                NEW_HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "0123456"),
                NEW_HOMEKIT_CHARACTERISTIC(MODEL, "ESP8266/ESP32"),
                NEW_HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
                NEW_HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
                NULL
            }),
            NEW_HOMEKIT_SERVICE(SWITCH, .primary=is_primary, .characteristics=(homekit_characteristic_t*[]){
                cha_switch_on,
                cha_name,
                NULL
            }),
            NULL
        }
    );
    return accessory;
}

homekit_accessory_t* new_climate_accessory(unsigned int id,
    bool is_primary, 
    homekit_characteristic_t* cha_name,
    homekit_characteristic_t* cha_ac_active, 
    homekit_characteristic_t* cha_current_temp,
    homekit_characteristic_t* cha_current_heater_cooler_state,
    homekit_characteristic_t* cha_target_heater_cooler_state,
    homekit_characteristic_t* cha_cooling_threshold_temp,
    homekit_characteristic_t* cha_heating_threshold_temp,
    homekit_characteristic_t* cha_rotation_speed
    ) {

    homekit_accessory_t *accessory = NEW_HOMEKIT_ACCESSORY(
        .id=id, 
        .category=homekit_accessory_category_air_conditioner, 
        .services=(homekit_service_t*[]) {
            NEW_HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
                cha_name,
                NEW_HOMEKIT_CHARACTERISTIC(MANUFACTURER, "idreamshen"),
                NEW_HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "CL2024"),
                NEW_HOMEKIT_CHARACTERISTIC(MODEL, "ESP32"),
                NEW_HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
                NEW_HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
                NULL
            }),
            NEW_HOMEKIT_SERVICE(HEATER_COOLER, .primary=is_primary, .characteristics=(homekit_characteristic_t*[]){
                cha_ac_active,
                cha_current_temp,
                cha_current_heater_cooler_state,
                cha_target_heater_cooler_state,
                cha_cooling_threshold_temp,
                cha_heating_threshold_temp,
                cha_rotation_speed,
                NULL
            }),
            NULL
        }
    );
    return accessory;
}
