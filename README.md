# L6470

Raspbery pi STM L6470 stepper motor driver based on X-CUBE-SPN2_firmware.

Prerequists:
1. pigpio library http://abyz.me.uk/rpi/pigpio/

It can be used with X-NUCLEO-IHM02A1 board. It is possible to synchronously drive two-axis stepper motors with one X-NUCLEO-IHM02A1
board only. However, up to four X-NUCLEO-IHM02A1 expansion boards can be stacked on a single STM32 Nucleo board, so it is possible to drive up to eight stepper motors.
