#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 16
#define PWM_WRAP 20000
#define SERVO_MIN_US 500
#define SERVO_MAX_US 2500
#define STEP_DELAY_MS 15

static uint slice_num;

static void servo_init(uint gpio_pin) {
    gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(gpio_pin);

    pwm_config config = pwm_get_default_config();

    // 125 MHz / 125 = 1 MHz, so 1 PWM count = 1 microsecond
    pwm_config_set_clkdiv(&config, 125.0f);
    pwm_config_set_wrap(&config, PWM_WRAP - 1);

    pwm_init(slice_num, &config, true);
}

static void set_servo_angle(float angle_deg) {
    if (angle_deg < 0.0f) angle_deg = 0.0f;
    if (angle_deg > 180.0f) angle_deg = 180.0f;

    float pulse_us = SERVO_MIN_US +
        (angle_deg / 180.0f) * (SERVO_MAX_US - SERVO_MIN_US);

    pwm_set_gpio_level(SERVO_PIN, (uint16_t)pulse_us);
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    servo_init(SERVO_PIN);

    while (true) {
        for (int angle = 0; angle <= 180; angle += 2) {
            set_servo_angle((float)angle);
            printf("Angle: %d\n", angle);
            sleep_ms(STEP_DELAY_MS);
        }

        for (int angle = 180; angle >= 0; angle -= 2) {
            set_servo_angle((float)angle);
            printf("Angle: %d\n", angle);
            sleep_ms(STEP_DELAY_MS);
        }
    }
}
