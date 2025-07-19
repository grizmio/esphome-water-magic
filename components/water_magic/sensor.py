import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, sensor
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA,
    CONF_PLATFORM_VERSION,
    DEVICE_CLASS_WATER,
    ICON_RULER,
    STATE_CLASS_MEASUREMENT,
    UNIT_CENTIMETER,
)
from esphome.pins import gpio_output_pin_schema

IS_TARGET_PLATFORM = True
DEPENDENCIES = []

CODEOWNERS = ["@grizmio"]

TRIGGER_PIN = "trigger_pin"
ECHO_PIN = "echo_pin"

water_magic = cg.esphome_ns.namespace("water_magic")
WaterMagic = water_magic.class_("WaterMagic", cg.PollingComponent)
water_magicComponentRef = WaterMagic.operator("ref")
CONFIG_SCHEMA = (
    sensor.sensor_schema(
        WaterMagic,
        unit_of_measurement=UNIT_CENTIMETER,
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
        }
    )
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], "WaterMagic")
    await sensor.register_component(var, config)
    cg.add_platformio_option("framework", "arduino")
    cg.add_build_flag("-DUSE_ARDUINO")

    cg.add(var.set_trigger_pin(config[TRIGGER_PIN]))
    cg.add(var.set_echo_pin(config[ECHO_PIN]))
 
    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(water_magicComponentRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
