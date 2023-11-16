#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
unsigned long interval = 6000; // Intervalo en milisegundos, inicializado en 6 segundos
unsigned long duracion = 500;
unsigned long MillisPrevios = 0;
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
bool dispositivoEncendido = false;
unsigned long tiempoInicioEncendido = 0;

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
  

  if (Serial.available() >= 4) {
    char HsActual1 = Serial.read();
    char HsActual2 = Serial.read();
    char MinActual1 = Serial.read();
    char MinActual2 = Serial.read();

    if (isDigit(HsActual1) && isDigit(HsActual2) && isDigit(MinActual1) && isDigit(MinActual2)) {
      horas = (HsActual1 - '0') * 10 + (HsActual2 - '0');
      minutos = (MinActual1 - '0') * 10 + (MinActual2 - '0');

      lcd.setCursor(3, 1);
      lcd.print(horas);
      lcd.print(":");
      lcd.print(minutos);
    }
  }
  unsigned long tiempoActual = millis();
  unsigned long tiempoObjetivo = tiempoActual + duracion;
  
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

    // Actualizar la hora y los minutos después de incrementar
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
  if (dispositivoEncendido) {
    digitalWrite(11, LOW);
    dispositivoEncendido = false;
    lcd.createChar(1, vacio);
    lcd.setCursor(10, 1);
    lcd.write((byte)1);
  }
  botones = analogRead(A0);

  if (botones >= 640 && botones <= 652) { // Boton select
    while (millis() < tiempoObjetivo) {
    	lcd.createChar(0, corazon);
    	lcd.setCursor(10, 1);
    	lcd.write((byte)0);
    	digitalWrite(11, HIGH);
    	dispositivoEncendido = true;
    }
  }

  if (botones >= 97 && botones <= 105) { // Boton UP
    interval = interval + 6000; // Aumenta el intervalo en 6 segundos
  }

  if (botones >= 253 && botones <= 270) { // Boton DOWN
    if (interval > 6000) {
      interval = interval - 6000; // Disminuye el intervalo en 6 segundos
    }
  }

  if (tiempoActual - tiempoInicioEncendido >= interval) {
    tiempoInicioEncendido = tiempoActual;
    while (millis() < tiempoObjetivo) {
    	lcd.createChar(0, corazon);
    	lcd.setCursor(10, 1);
    	lcd.write((byte)0);
    	digitalWrite(11, HIGH);
    	dispositivoEncendido = true;
    }
  }
}
