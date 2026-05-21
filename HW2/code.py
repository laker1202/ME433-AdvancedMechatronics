import time
import board
import pwmio

SERVO_PIN = board.GP15
PWM_FREQ = 50

servo = pwmio.PWMOut(SERVO_PIN, frequency=PWM_FREQ, duty_cycle=0)

def set_pulse_us(pulse_us):
    if pulse_us < 500:
        pulse_us = 500
    if pulse_us > 2500:
        pulse_us = 2500

    duty_cycle = int((pulse_us / 20000.0) * 65535)
    servo.duty_cycle = duty_cycle

def set_angle(angle_deg):
    if angle_deg < 0:
        angle_deg = 0
    if angle_deg > 180:
        angle_deg = 180

    pulse_us = 500 + (angle_deg / 180.0) * 2000
    set_pulse_us(pulse_us)

while True:
    for angle in range(0, 181, 2):
        set_angle(angle)
        time.sleep(0.02)

    for angle in range(180, -1, -2):
        set_angle(angle)
        time.sleep(0.02)
