import spidev
import time

import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BOARD)
GPIO.setup(24, GPIO.OUT)

# Enable SPI
spi = spidev.SpiDev(0, 0)
# spi.open(bus, cs)
spi.max_speed_hz = 20000000
# spi.max_speed_hz = 100000
spi.mode = 0

# # Toggle CS
# spi.xfer2([0x00])

# time.sleep(0.5)

# # Revert to SPI bus mode
# spi.xfer2([0xFF])

# time.sleep(0.5)

# Write
# CMD_WRITE = 0x02
# tx = [CMD_WRITE, 0x00, 0x00, 0x00, 0x03, 0x02, 0x01]
# spi.xfer2(tx)

# time.sleep(0.5)

# Read
CMD_READ = 0x03
tx = [CMD_READ, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]

rx = spi.xfer2(tx)
print(rx)

GPIO.setup(24, GPIO.IN)
