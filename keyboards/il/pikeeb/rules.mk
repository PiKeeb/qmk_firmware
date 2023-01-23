# Enable PiKeeb-specific features
AUDIO_ENABLE = yes          		# Enable audio output
OLED_ENABLE = yes           		# Enable OLED display
POINTING_DEVICE_ENABLE = yes		# Enable pointing device
RGBLIGHT_ENABLE = yes				# ENABLE RGB lights

# OLED Driver
OLED_DRIVER = SSD1306

# Audio driver
AUDIO_DRIVER = pwm_hardware

# Disable USB check on startup
NO_USB_STARTUP_CHECK = yes

# Pointing device driver
POINTING_DEVICE_DRIVER = analog_joystick

# ADC Driver for voltage measurement
SRC += analog.c

# Custom matrix for handling the JoyMouse DB direct pins
CUSTOM_MATRIX = lite
SRC += matrix.c

# Enable Link Time Optimization
LTO_ENABLE = yes