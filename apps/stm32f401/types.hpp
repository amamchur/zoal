#ifndef ZOAL_STM32F401_TYPES_HPP
#define ZOAL_STM32F401_TYPES_HPP

enum class app_cmd {
    none,
    adc,
    i2c,
    timer,
    ticks,
    help,
    pwm,
    pwm_on,
    pwm_off,
    w25q_read,
    w25q_write,
    w25q_erase_chip,
    w25q_erase_sector
};

#endif
