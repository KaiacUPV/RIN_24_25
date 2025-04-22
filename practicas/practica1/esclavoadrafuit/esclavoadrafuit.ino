#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define UART_TX 17  // UART TX pin (al maestro)
#define UART_RX 16  // UART RX pin (no usado aquí)

// Pines I2C
#define SDA_PIN 4  // GPIO 4 para SDA
#define SCL_PIN 5  // GPIO 5 para SCL

void setup() {
  Serial.begin(115200);  // Consola serial para debug
  Serial1.begin(9600, SERIAL_8N1, UART_RX, UART_TX);  // UART hacia maestro
  Serial.println("Esclavo listo y esperando datos...");

  // Iniciar la comunicación I2C con pines personalizados
  Wire.begin(SDA_PIN, SCL_PIN);  // SDA en GPIO 4, SCL en GPIO 5

  // Iniciar la pantalla OLED con I2C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, OLED_RESET)) {
    Serial.println(F("No se encontró la pantalla OLED"));
    while (true);  // Detener si no se encuentra la pantalla
  }

  display.display();  // Mostrar pantalla en blanco
  delay(2000);  // Tiempo para que se muestre el logo al inicio
}

void loop() {
  if (Serial1.available()) {  // Verificar si hay datos recibidos por UART
    String mensaje = Serial1.readStringUntil('\n');  // Leer hasta el salto de línea

    // Mostrar el mensaje recibido por el monitor serial
    Serial.print("Recibido del maestro: ");
    Serial.println(mensaje);

    // Limpiar la pantalla OLED
    display.clearDisplay();

    // Establecer el tamaño de texto más grande
    display.setTextSize(2);      // Cambiar a tamaño de texto 2 (más grande)
    display.setTextColor(SSD1306_WHITE);  // Color del texto
    display.setCursor(0, 0);     // Posición inicial del texto

    // Extraer la temperatura y la humedad del mensaje
    String temp = mensaje.substring(2, mensaje.indexOf(','));
    String hum = mensaje.substring(mensaje.indexOf('H') + 2);

    display.print("T: ");
    display.print(temp);  // Mostrar la temperatura

    display.setCursor(0, 30);  // Mover el cursor hacia abajo para la siguiente línea
    display.print("H: ");
    display.print(hum);  // Mostrar la humedad
    display.print(" %");

    display.display();  // Actualizar la pantalla
  }
}
