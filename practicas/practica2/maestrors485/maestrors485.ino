#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

#define UART_TX 17  // TX hacia HW-519
#define UART_RX 16  // RX desde HW-519

// Pantalla OLED conectada en SDA=11, SCL=12
#define OLED_SDA 11
#define OLED_SCL 12
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
TwoWire wireOLED = TwoWire(1);  // Nueva instancia de I2C para OLED
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &wireOLED, OLED_RESET);

void setup() {
  Serial.begin(115200);  // Consola USB
  Serial1.begin(9600, SERIAL_8N1, UART_RX, UART_TX); // UART1 por 16 (RX) y 17 (TX)

  Wire.begin(4, 5);  // Sensor HTU21D en SDA=4, SCL=5

  if (!htu.begin()) {
    Serial.println("Sensor HTU21D no encontrado");
    while (1);
  }

  Serial.println("Sensor iniciado");

  // Iniciar pantalla OLED
  wireOLED.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Pantalla OLED no detectada");
    while (1);
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  float temp = htu.readTemperature();
  float hum = htu.readHumidity();

  // Mostrar en consola USB
  Serial.printf("Temp: %.2f °C, Hum: %.2f %%\n", temp, hum);

  // Enviar por RS485 usando UART1
  Serial1.printf("T:%.2f C,H:%.2f\n", temp, hum);

  // Mostrar en OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("T: %.2f C", temp);
  display.setCursor(0, 30);
  display.printf("H: %.2f %%", hum);

  // Mostrar letra "M" de maestro en pequeña
  display.setTextSize(1);
  display.setCursor(110, 54);  // Esquina inferior derecha
  display.print("M");

  display.display();

  delay(1000);  // Cada segundo
}
