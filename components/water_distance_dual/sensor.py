import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_HUMIDITY,
    CONF_ID,
    CONF_DISTANCE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CENTIMETER,
    UNIT_PERCENT,
)

CODEOWNERS = ["@grizmio"]

TRIGGER_PIN = "trigger_pin"
ECHO_PIN = "echo_pin"
DISTANCE_ADJUSTMENT = "distance_adjustment"
DISTANCE_TO_LITERS_FACTOR = "distance_to_liters_factor"

water_distance_ns = cg.esphome_ns.namespace("water_distance")
WaterDistanceComponent = water_distance_ns.class_("WaterDistanceComponent", cg.PollingComponent)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(WaterDistanceComponent),
            cv.Optional(TRIGGER_PIN, default=13): cv.int_,
            cv.Optional(ECHO_PIN, default=15): cv.int_,
            cv.Optional(DISTANCE_ADJUSTMENT, default=0): cv.float_,
            cv.Optional(DISTANCE_TO_LITERS_FACTOR, default=3141.592653589793238): cv.float_,
            cv.Optional(CONF_DISTANCE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CENTIMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_DISTANCE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_HUMIDITY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("60s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add_platformio_option("framework", "arduino")
    cg.add_build_flag("-DUSE_ARDUINO")

    cg.add(var.set_trigger_pin(config[TRIGGER_PIN]))
    cg.add(var.set_echo_pin(config[ECHO_PIN]))
    cg.add(var.set_distance_adjustment(config[DISTANCE_ADJUSTMENT]))
    cg.add(var.set_distance_to_liters_factor(config[DISTANCE_TO_LITERS_FACTOR]))

    if CONF_DISTANCE in config:
        sens = await sensor.new_sensor(config[CONF_DISTANCE])
        cg.add(var.set_distance_sensor(sens))

    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))

