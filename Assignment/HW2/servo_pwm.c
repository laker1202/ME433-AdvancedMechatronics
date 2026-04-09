#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"

#define SERVO_PIN 15

void set_servo_angle(unsigned int gpio, float angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    float pulse_ms = 1.0f + (angle / 180.0f);   // 1.0 ms to 2.0 ms
    unsigned int level = (unsigned int)(pulse_ms * 1000.0f);

    pwm_set_gpio_level(gpio, level);
}

int main() {
    stdio_init_all();

    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);

    unsigned int slice_num = pwm_gpio_to_slice_num(SERVO_PIN);

    pwm_set_clkdiv(slice_num, 125.0f);
    pwm_set_wrap(slice_num, 20000);
    pwm_set_enabled(slice_num, true);

    while (1) {
        for (float angle = 0; angle <= 180; angle += 5) {
            set_servo_angle(SERVO_PIN, angle);
            sleep_ms(100);
        }

        for (float angle = 180; angle >= 0; angle -= 5) {
            set_servo_angle(SERVO_PIN, angle);
            sleep_ms(100);
        }
    }

    return 0;
}