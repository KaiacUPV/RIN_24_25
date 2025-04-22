#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

Adafruit_HTU21DF htu = Adafruit_HTU21DF();

#define UART_TX 17  // UART TX pin (al esclavo)
#define UART_RX 16  // RX no usado aquí

// Pantalla OLED conectada en SDA=11, SCL=12
#define OLED_SDA 11
#define OLED_SCL 12
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
TwoWire wireOLED = TwoWire(1);  // Nueva instancia de I2C para la pantalla
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &wireOLED, OLED_RESET);

void setup() {
  Serial.begin(115200);  // Debug USB
  Serial1.begin(9600, SERIAL_8N1, UART_RX, UART_TX); // UART hacia esclavo

  Wire.begin(4, 5);  // SDA, SCL para el sensor HTU21D

  if (!htu.begin()) {
    Serial.println("Sensor HTU21D no encontrado");
    while (1);
  }

  Serial.println("Sensor iniciado");

  // Iniciar pantalla OLED usando otra instancia de I2C (SDA=11, SCL=12)
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

  // Mostrar en consola
  Serial.printf("Temp:%.2f °C, Humedad: %.2f %%\n", temp, hum);

  // Enviar al esclavo por UART
  Serial1.printf("T:%.2f C, H:%.2f%\n", temp, hum);

  // Mostrar en pantalla OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("T: %.2f C", temp);
  display.setCursor(0, 30);
  display.printf("H: %.2f% %", hum);
  display.display();

  display.setTextSize(1);  // Tamaño grande
  display.setCursor(0, 32);  // Posición en esquina inferior derecha
  display.print("M");

  delay(1000);
}