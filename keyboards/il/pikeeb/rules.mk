# Enable PiKeeb-specific features
AUDIO_ENABLE = yes          		# Enable audio output
OLED_ENABLE = yes           		# Enable OLED display
POINTING_DEVICE_ENABLE = yes		# Enable pointing device
RGBLIGHT_ENABLE = yes				# Enable RGB lights
ENCODER_ENABLE = yes				# Enable Encoder support
ENCODER_MAP_ENABLE = yes			# Enable Encoder map
WPM_ENABLE = yes					# Enable Word Per Minute (WPM) Calculcation

# OLED Driver
OLED_DRIVER = SSD1306
SRC += lib/oled_render.c

# Audio driver
AUDIO_DRIVER = pwm_hardware

# Disable USB check on startup
NO_USB_STARTUP_CHECK = yes

# Pointing device driver
POINTING_DEVICE_DRIVER = analog_joystick

# ADC Driver for voltage measurement
SRC += analog.c
SRC += lib/measure.c

# Custom matrix for handling the JoyMouse DB direct pins
CUSTOM_MATRIX = lite
SRC += matrix.c

# Some Fun Stuff
SNAKE_ENABLE = yes
BONGOCAT_ENABLE = yes

# Enable Link Time Optimization
LTO_ENABLE = yes

ifeq ($(strip $(SNAKE_ENABLE)), yes)
    SRC += lib/snake.c
    OPT_DEFS += -DSNAKE_ENABLE
endif

ifeq ($(strip $(BONGOCAT_ENABLE)), yes)
    SRC += lib/bongocat.c
    OPT_DEFS += -DBONGOCAT_ENABLE
endif