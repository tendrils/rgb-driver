#include <light.h>
#include <light_core.h>
#include <light/display_panel.h>
#include <light/mod_display_gui.h>

#include <pico/time.h>
#include <pico.h>

#include <hardware/pwm.h>
#include <hardware/gpio.h>

#define PIN_RGB_R               4 
#define PIN_RGB_G               5
#define PIN_RGB_B               6
#define PIN_UV                  7

#define MASK_PINS               ((1 << PIN_RGB_R) | (1 << PIN_RGB_G) | (1 << PIN_RGB_B) | (1 << PIN_UV))

#define UPDATE_PERIOD_MS        10
#define PWM_COUNTER_WRAP        100

#define APP_NAME                "LED Driver"

light_app_context_t this_app = {
        .name = APP_NAME,
        .deps_count = 3,
        .depends_on = {
                LIGHT_MODULE_NAME_DISPLAY_PANEL,
                LIGHT_MODULE_NAME_DISPLAY_GUI,
                LIGHT_MODULE_NAME_LIGHT_CORE
        }
        
};

int main(int argc, char *argv[])
{
    light_init();

    pwm_config conf = pwm_get_default_config();
    int slice_r = pwm_gpio_to_slice_num(PIN_RGB_R);
    int slice_g = pwm_gpio_to_slice_num(PIN_RGB_G);
    int slice_b = pwm_gpio_to_slice_num(PIN_RGB_B);
    int slice_uv = pwm_gpio_to_slice_num(PIN_UV);

    gpio_set_dir(PIN_RGB_R, GPIO_OUT);
    gpio_set_function(PIN_RGB_R, GPIO_FUNC_PWM);
    
    gpio_set_dir(PIN_RGB_G, GPIO_OUT);
    gpio_set_function(PIN_RGB_G, GPIO_FUNC_PWM);
    
    gpio_set_dir(PIN_RGB_B, GPIO_OUT);
    gpio_set_function(PIN_RGB_B, GPIO_FUNC_PWM);
    
    gpio_set_dir(PIN_UV, GPIO_OUT);
    gpio_set_function(PIN_UV, GPIO_FUNC_PWM);

    pwm_config_set_clkdiv(&conf, 100.0f);
    pwm_config_set_wrap(&conf, PWM_COUNTER_WRAP);

    // configure slices
    pwm_init(PIN_RGB_R, &conf, false);
    pwm_init(PIN_RGB_G, &conf, false);
    pwm_init(PIN_RGB_B, &conf, false);
    pwm_init(PIN_UV, &conf, false);

    //start slices running simultaneously
    pwm_set_mask_enabled(MASK_PINS);

    for(int i = 0; i < UINT16_MAX; i++) {
        pwm_set_gpio_level(PIN_RGB_R, i);

        sleep_ms(UPDATE_PERIOD_MS);
    }
}

static void pwm_wrap_handler()
{
    
}
