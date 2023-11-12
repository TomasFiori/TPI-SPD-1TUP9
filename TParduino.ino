#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);
unsigned long interval = 6000; // Intervalo en milisegundos, inicializado en 1 minuto
unsigned long MillisPrevios = 0;
unsigned long tiempoInicio = 0;
int botones;
byte corazon[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte vacio[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

int horas = 0;
int minutos = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("EL PERFUMASO ");
  lcd.setCursor(0, 1);
  lcd.print("Hs: ");
  pinMode(11, OUTPUT); // Prender el perfumaso.
  pinMode(A0, INPUT); // Leer la entrada del teclado.
  
}

void loop() {
  
    // Si hay datos disponibles en el puerto serie
    lcd.createChar(1, vacio);
    lcd.setCursor(10,1);
    lcd.write((byte)1);
    if (Serial.available() >= 4) {
        char HsActual1 = Serial.read(); // Leer el primer carácter de las horas
        char HsActual2 = Serial.read(); // Leer el segundo carácter de las horas
        char MinActual1 = Serial.read(); // Leer el primer carácter de los minutos
        char MinActual2 = Serial.read(); // Leer el segundo carácter de los minutos

        // Verificar si los caracteres son dígitos
        if (isDigit(HsActual1) && isDigit(HsActual2) && isDigit(MinActual1) && isDigit(MinActual2)) {
        horas = (HsActual1 - '0') * 10 + (HsActual2 - '0'); // Convierte los dígitos a un número
        minutos = (MinActual1 - '0') * 10 + (MinActual2 - '0');

        // Mostrar la hora y los minutos
        lcd.setCursor(3, 1);
        lcd.print(horas);
        lcd.print(":");
        if (millis())
        lcd.print(minutos);
        }
    }
    
    // Incrementar los minutos cada minuto basado en millis()
  unsigned long tiempoActual = millis();
  if (tiempoActual - MillisPrevios >= 60000) {
    MillisPrevios = tiempoActual;
    minutos++;
    if (minutos >= 60) {
      minutos = 0;
      horas++;
      if (horas >= 24) {
        horas = 0;
      }
    }

    // Actualizar la hora y los minutos
    lcd.setCursor(3, 1);
    if (horas < 10) {
      lcd.print("0");
    }
    lcd.print(horas);
    lcd.print(":");
    if (minutos < 10) {
      lcd.print("0");
    }
    lcd.print(minutos);
  }


  botones = analogRead(A0);

  if (botones >= 640 && botones <= 652) { // Boton select
    lcd.createChar(0, corazon);
    lcd.setCursor(10, 1);
    lcd.write((byte)0);
    
    digitalWrite(11, HIGH);
    retrasoPersonalizado(1000);
    digitalWrite(11, LOW);
  }

  if (botones >= 97 && botones <= 105) { // Boton UP
    interval = interval + 60000; // Aumenta el intervalo en 1 minuto
  }

  if (botones >= 253 && botones <= 270) { // Boton DOWN
    if (interval > 60000) {
      interval = interval - 60000; // Disminuye el intervalo en 1 minuto, con un mínimo de 1 minuto
    }
  }


  if (MillisPrevios >= interval) {
    MillisPrevios = millis();
    lcd.createChar(0, corazon);
    lcd.setCursor(10, 1);
    lcd.write((byte)0);

    digitalWrite(11, HIGH);
    retrasoPersonalizado(1000);
    digitalWrite(11, LOW);
    
  }
}
void retrasoPersonalizado(unsigned long duracion) {
  unsigned long tiempoActual = millis();
  unsigned long tiempoObjetivo = tiempoActual + duracion;

  while (millis() < tiempoObjetivo) {
    // Espera hasta que haya pasado el tiempo
  }
}