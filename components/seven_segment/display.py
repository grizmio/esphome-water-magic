import esphome.codegen as cg
from esphome.components import display
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_LAMBDA
from esphome.pins import gpio_output_pin_schema

seven_segment_ns = cg.esphome_ns.namespace("seven_segment")
SEVENSEGMENTComponent = seven_segment_ns.class_(
    "SEVENSEGMENTComponent", cg.PollingComponent
)
SEVENSEGMENTComponentRef = SEVENSEGMENTComponent.operator("ref")

CONF_A_PIN = "a_pin"
CONF_B_PIN = "b_pin"
CONF_C_PIN = "c_pin"
CONF_D_PIN = "d_pin"
CONF_E_PIN = "e_pin"
CONF_F_PIN = "f_pin"
CONF_G_PIN = "g_pin"
CONF_DP_PIN = "dp_pin"
CONF_G1_PIN = "g1_pin"
CONF_G2_PIN = "g2_pin"
CONF_G3_PIN = "g3_pin"
CONF_G4_PIN = "g4_pin"

CONFIG_SCHEMA = display.BASIC_DISPLAY_SCHEMA.extend(
    {
        cv.GenerateID(): cv.declare_id(SEVENSEGMENTComponent),
        cv.Required(CONF_A_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_B_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_C_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_D_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_E_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_F_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_G_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_DP_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_G1_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_G2_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_G3_PIN): cv.ensure_schema(gpio_output_pin_schema),
        cv.Required(CONF_G4_PIN): cv.ensure_schema(gpio_output_pin_schema),
    }
).extend(cv.polling_component_schema("25ms"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await display.register_display(var, config)

    # Setup each pin
    pin_a = await cg.gpio_pin_expression(config[CONF_A_PIN])
    cg.add(var.set_a_pin(pin_a))

    pin_b = await cg.gpio_pin_expression(config[CONF_B_PIN])
    cg.add(var.set_b_pin(pin_b))

    pin_c = await cg.gpio_pin_expression(config[CONF_C_PIN])
    cg.add(var.set_c_pin(pin_c))

    pin_d = await cg.gpio_pin_expression(config[CONF_D_PIN])
    cg.add(var.set_d_pin(pin_d))

    pin_e = await cg.gpio_pin_expression(config[CONF_E_PIN])
    cg.add(var.set_e_pin(pin_e))

    pin_f = await cg.gpio_pin_expression(config[CONF_F_PIN])
    cg.add(var.set_f_pin(pin_f))

    pin_g = await cg.gpio_pin_expression(config[CONF_G_PIN])
    cg.add(var.set_g_pin(pin_g))

    pin_dp = await cg.gpio_pin_expression(config[CONF_DP_PIN])
    cg.add(var.set_dp_pin(pin_dp))

    pin_g1 = await cg.gpio_pin_expression(config[CONF_G1_PIN])
    cg.add(var.set_g1_pin(pin_g1))

    pin_g2 = await cg.gpio_pin_expression(config[CONF_G2_PIN])
    cg.add(var.set_g2_pin(pin_g2))

    pin_g3 = await cg.gpio_pin_expression(config[CONF_G3_PIN])
    cg.add(var.set_g3_pin(pin_g3))

    pin_g4 = await cg.gpio_pin_expression(config[CONF_G4_PIN])
    cg.add(var.set_g4_pin(pin_g4))

    # cg.add(var.set_num_chips(config[CONF_NUM_CHIPS]))

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(SEVENSEGMENTComponentRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))
