import esphome.codegen as cg
from esphome import automation
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
)

DEPENDENCIES = ['network']
AUTO_LOAD = ['climate']
MULTI_CONF = False

homekit_ns = cg.esphome_ns.namespace('homekit')

HomekitComponent = homekit_ns.class_('HomekitComponent', cg.PollingComponent)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(HomekitComponent),
    cv.Optional("setup_code", default="111-11-111"): cv.string,
    cv.Optional("name", default="Homekit Bridge"): cv.string,
    cv.Optional("model", default="bridge_d1"): cv.string,
    cv.Optional("manufacturer", default="diy"): cv.string,
    cv.Optional("serial_num", default="20240524"): cv.string,
    cv.Optional("fw_rev", default="1.0.0"): cv.string,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    cg.add_library("HomeKit-ESP8266", "8a8e1a065005e9252d728b24f96f6d0b29993f67", "https://magic.idreamshen.com/proxy/https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266.git")
    var = cg.new_Pvariable(config[CONF_ID])
    cg.add(var.set_setup_code(config["setup_code"]))
    cg.add(var.set_name(config["name"]))
    cg.add(var.set_model(config["model"]))
    cg.add(var.set_manufacturer(config["manufacturer"]))
    cg.add(var.set_serial_num(config["serial_num"]))
    cg.add(var.set_fw_rev(config["fw_rev"]))
    yield cg.register_component(var, config)
    
