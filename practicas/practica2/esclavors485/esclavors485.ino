#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define UART_TX 17  // TX hacia HW-519
#define UART_RX 16  // RX desde HW-519

// Pines I2C de la pantalla OLED
#define SDA_PIN 4
#define SCL_PIN 5

void setup() {
  Serial.begin(115200);  // Consola serial para debug
  Serial1.begin(9600, SERIAL_8N1, UART_RX, UART_TX);  // UART con HW-519
  Serial.println("Esclavo listo y esperando datos...");

  Wire.begin(SDA_PIN, SCL_PIN);  // Iniciar I2C para la pantalla

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, OLED_RESET)) {
    Serial.println("No se encontró la pantalla OLED");
    while (true);
  }

  display.display();  // Mostrar logo por defecto
  delay(2000);
  display.clearDisplay();
  display.display();
}

void loop() {
  if (Serial1.available()) {
    String mensaje = Serial1.readStringUntil('\n');

    Serial.print("Recibido del maestro: ");
    Serial.println(mensaje);

    // Limpiar pantalla
    display.clearDisplay();

    // Extraer temperatura y humedad del mensaje
    String temp = mensaje.substring(2, mensaje.indexOf(','));
    String hum = mensaje.substring(mensaje.indexOf('H') + 2);

    // Mostrar en OLED
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.print("T: ");
    display.print(temp);

    display.setCursor(0, 30);
    display.print("H: ");
    display.print(hum);
    display.print(" %");

    // Letra "E" para indicar esclavo
    display.setTextSize(1);
    display.setCursor(110, 54);
    display.print("E");

    display.display();
  }
}
