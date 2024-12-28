#define echoPin A3
#define trigPin A4
#define greenLED A0
#define orangeLED A5
#define redLED A1
#define switchPin A2
#define buzzerPin A6
#define SHCP D4
#define STCP D10
#define DS D5

long duration, distance;
byte segments[] = {
  B01111111,  // 0
  B00001011,  // 1
  B10110111,  // 2
  B10011111,  // 3
  B11001011,  // 4
  B11011101,  // 5
  B11111101,  // 6
  B00001111,  // 7
  B11111111,  // 8
  B11011111,  // 9
};


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);

  pinMode(SHCP, OUTPUT);
  pinMode(STCP, OUTPUT);
  pinMode(DS, OUTPUT);
}

void loop() {
  int switchState = digitalRead(switchPin);  // Lire l'état de l'interrupteur

  if (switchState == HIGH) {  // Si l'interrupteur est activé (appuyé)
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.034 / 2) / 10;


    Serial.println(distance);

    // Contrôle des diodes en fonction de la distance
    if (distance > 12.0) {                              // Si la distance est supérieure à 1.2 m (120 cm)
      digitalWrite(greenLED, HIGH);                     // Allumer la LED verte
      digitalWrite(orangeLED, LOW);                     // Éteindre la LED orange
      digitalWrite(redLED, LOW);                        // Éteindre la LED rouge
    } else if (distance >= 07.5 && distance <= 12.0) {  // Si la distance est entre 75 cm et 120 cm
      digitalWrite(greenLED, LOW);                      // Éteindre la LED verte
      digitalWrite(orangeLED, HIGH);                    // Allumer la LED orange
      digitalWrite(redLED, LOW);                        // Éteindre la LED rouge
      tone(buzzerPin, 440);                             // Activer le buzzer avec une fréquence de 440 Hz
      delay(200);                                       // Attendre 200 ms
      noTone(buzzerPin);                                // Éteindre le buzzer
      digitalWrite(buzzerPin, LOW);
      Serial.print("Temps d'arrêt: ");
      Serial.print(map(distance, 5, 50, 500, 1000));  // Correspondance de la distance avec le temps d'arrêt
      Serial.println(" ms");
      delay(map(distance, 5, 50, 500, 1000));        // Attendre un délai variable en fonction de la distance
    } else if (distance > 3.0 && distance <= 7.5) {  // Si la distance est inférieure à 75 cm
      digitalWrite(greenLED, LOW);                   // Éteindre la LED verte
      digitalWrite(orangeLED, HIGH);                 // Éteindre la LED orange
      digitalWrite(redLED, LOW);                     // Allumer la LED rouge
      tone(buzzerPin, 440);                          // Activer le buzzer avec une fréquence de 440 Hz
      delay(200);                                    // Attendre 200 ms
      noTone(buzzerPin);                             // Éteindre le buzzer
      digitalWrite(buzzerPin, LOW);
      Serial.print("Temps d'arrêt: ");
      Serial.print(map(distance, -30, 5, 0, 500));  // Correspondance de la distance avec le temps d'arrêt
      Serial.println(" ms");
      delay(map(distance, -30, 5, 0, 500));  // Attendre un délai variable en fonction de la distance
    } else {
      digitalWrite(greenLED, LOW);   // Éteindre la LED verte
      digitalWrite(orangeLED, LOW);  // Éteindre la LED orange
      digitalWrite(redLED, HIGH);    // Allumer la LED rouge
      tone(buzzerPin, 440);          // Activer le buzzer avec une fréquence de 440 Hz
      delay(200);                    // Attendre 200 ms
    }
  } else {
    // Si l'interrupteur est désactivé, éteindre toutes les LED et le buzzer
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(orangeLED, LOW);
    digitalWrite(redLED, LOW);
    noTone(buzzerPin);  // Éteindre le buzzer
    digitalWrite(buzzerPin, LOW);
  }
  int digit1 = distance / 10;
  int digit2 = distance % 10;

  digitalWrite(STCP, LOW);

  shiftOut(DS, SHCP, MSBFIRST, segments[digit2]);
  shiftOut(DS, SHCP, MSBFIRST, segments[digit1]);

  digitalWrite(STCP, HIGH);
  delay(50);
}