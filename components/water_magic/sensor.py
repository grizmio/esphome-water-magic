import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    ICON_RULER,
    DEVICE_CLASS_WATER,
    CONF_ID,
    STATE_CLASS_MEASUREMENT,
    UNIT_LITERS,
)

CODEOWNERS = ["@grizmio"]

TRIGGER_PIN = "trigger_pin"
ECHO_PIN = "echo_pin"
DISTANCE_ADJUSTMENT = "distance_adjustment"
DISTANCE_TO_LITERS_FACTOR = "distance_to_liters_factor"

water_magic_ns = cg.esphome_ns.namespace("water_magic")
WaterMagic = water_magic_ns.class_("WaterMagic", cg.PollingComponent)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        WaterMagic,
        unit_of_measurement=UNIT_LITERS,
        icon=ICON_RULER,
        accuracy_decimals=1,
        device_class=DEVICE_CLASS_WATER,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(WaterMagic),
            cv.Optional(TRIGGER_PIN, default=13): cv.int_,
            cv.Optional(ECHO_PIN, default=15): cv.int_,
            cv.Optional(DISTANCE_ADJUSTMENT, default=0): cv.float_,
            cv.Optional(DISTANCE_TO_LITERS_FACTOR, default=3141.592653589793238): cv.float_,
        }
    )
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    cg.add_platformio_option("framework", "arduino")
    cg.add_build_flag("-DUSE_ARDUINO")

    cg.add(var.set_trigger_pin(config[TRIGGER_PIN]))
    cg.add(var.set_echo_pin(config[ECHO_PIN]))
    cg.add(var.set_distance_adjustment(config[DISTANCE_ADJUSTMENT]))
    cg.add(var.set_distance_to_liters_factor(config[DISTANCE_TO_LITERS_FACTOR]))
