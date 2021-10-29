#include <SPI.h>

#define HSPI_SCLK   14
#define HSPI_MISO   12
#define HSPI_MOSI   13
#define HSPI_SS     15

#define BENCH_START unsigned long _t_start = micros();
#define BENCH_END Serial.println(micros() - _t_start);
#define TEST_N_BYTES 252

static const long spiClk = 20000000; // 20 MHz

SPIClass * hspi = new SPIClass();

void setup() {
  hspi->begin();
  hspi->setFrequency(spiClk);
  pinMode(HSPI_SS, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // Define and initialize test buffers
  byte send_buf[TEST_N_BYTES];
  byte recv_buf[TEST_N_BYTES];
  for (size_t i = 0; i < TEST_N_BYTES; i++) {
    send_buf[i] = (byte) i;
    recv_buf[i] = 0x00;
  }

  BENCH_START

  write_sram(send_buf, TEST_N_BYTES);
  read_sram(recv_buf, TEST_N_BYTES);

  BENCH_END

  // recv_buf should == send_buf
  for (size_t i = 0; i < TEST_N_BYTES; i++) {
    if (send_buf[i] != recv_buf[i]) {
      for (;;) {
        Serial.println("++++++++++++++++++++++++++++");
        Serial.println("ERROR: send_buf != recv_buf");
        delay(1000);
      }
    }
  }
//    for (size_t i = 0; i < TEST_N_BYTES; i++) {
//      Serial.print(recv_buf[i]);
//      Serial.print("\t");
//    }
//    Serial.println();
}

void write_sram(byte *data, size_t len) {
  byte command_write = 0x02;

  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(HSPI_SS, LOW);
  hspi->transfer(command_write);
  hspi->transfer(0x00); // Addr high
  hspi->transfer(0x00); // Addr mid
  hspi->transfer(0x00); // Addr low
  for (size_t i = 0; i < len; i++) {
    // write data
    hspi->transfer(data[i]);
  }
  digitalWrite(HSPI_SS, HIGH);
  hspi->endTransaction();
}

byte read_sram(byte *buf, size_t len) {
  byte command_read = 0x03;
  byte data;

  hspi->beginTransaction(SPISettings(spiClk, MSBFIRST, SPI_MODE0));
  digitalWrite(HSPI_SS, LOW);
  hspi->transfer(command_read);
  hspi->transfer(0x00); // Addr high
  hspi->transfer(0x00); // Addr mid
  hspi->transfer(0x00); // Addr low
  for (size_t i = 0; i < len; i++) {
    // read data
    buf[i] = hspi->transfer(0x00);
  }
  digitalWrite(HSPI_SS, HIGH);
  hspi->endTransaction();
  return data;
}
