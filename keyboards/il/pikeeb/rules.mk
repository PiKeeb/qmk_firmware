# Enable PiKeeb-specific features
AUDIO_ENABLE = yes          		# Enable audio output
OLED_ENABLE = yes           		# Enable OLED display
POINTING_DEVICE_ENABLE = yes		# Enable pointing device
RGBLIGHT_ENABLE = yes				# ENABLE RGB lights

# OLED Driver
OLED_DRIVER = SSD1306

# Audio driver
AUDIO_DRIVER = pwm_hardware

# Pointing device driver
POINTING_DEVICE_DRIVER = analog_joystick

# Disable USB check on startup
NO_USB_STARTUP_CHECK = yes

# ADC Driver for voltage measurement
SRC += analog.c

# Enable Link Time Optimization
LTO_ENABLE = yes