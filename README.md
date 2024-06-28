# Esphome Homekit 8266

## 介绍
这是一个 esp8266 的 esphome homekit 组件，致力于将 homekit 与 esphome 结合。目前仍处于早期版本

## 支持的传感器类型
1. climate

## yaml 例子
```yaml
esphome:
  name: homekit-esp8266
  friendly_name: homekit-esp8266
  platformio_options:
    board_build.f_cpu: 160000000L
    build_flags: 
      - "-D HOMEKIT_LOG_LEVEL=0 -D ARDUINO_HOMEKIT_LOWROM"

external_components:
  - source:
      type: git
      url: https://github.com/idreamshen/esphome-homekit-8266
      refresh : 5min

esp8266:
  board: esp01_1m

logger:
  baud_rate: 0
  level: NONE

climate:
  - platform: xxxx
    name: "AirCondition 1"

homekit:
```