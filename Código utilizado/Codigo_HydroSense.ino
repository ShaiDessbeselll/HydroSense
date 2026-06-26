// ----------------------
// PINOS
// ----------------------
#define TRIG 10
#define ECHO 11

#define LED_VERMELHO 2
#define BUZZER 8

// ----------------------
// VARIÁVEIS
// ----------------------
unsigned long tempoInicio = 0;
bool garrafaPresente = false;
bool lembreteAtivado = false;

// Distância máxima para considerar que a garrafa está presente (cm)
const int DISTANCIA_GARRAFA = 10;

// Tempo para o lembrete (30 minutos)
const unsigned long TEMPO_LEMBRETE = 300000UL;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  float distancia = medirDistancia();

  Serial.print("Distancia: ");
  Serial.println(distancia);

  // Garrafa detectada
  if (distancia > 0 && distancia <= DISTANCIA_GARRAFA) {

    // Primeira vez que detectou
    if (!garrafaPresente) {
      garrafaPresente = true;
      tempoInicio = millis();

      digitalWrite(LED_VERMELHO, LOW);
      noTone(BUZZER);

      Serial.println("Garrafa colocada.");
    }

    // Verifica se passaram 30 minutos
    if (!lembreteAtivado &&
        millis() - tempoInicio >= TEMPO_LEMBRETE) {

      lembreteAtivado = true;

      digitalWrite(LED_VERMELHO, HIGH);
      tone(BUZZER, 1000);

      Serial.println("Lembrete ativado!");
    }
  }
  else {

    // Garrafa removida
    if (garrafaPresente) {
      garrafaPresente = false;
      lembreteAtivado = false;

      digitalWrite(LED_VERMELHO, LOW);
      noTone(BUZZER);

      Serial.println("Garrafa removida.");
    }
  }

  delay(500);
}

float medirDistancia() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG, LOW);

  long duracao = pulseIn(ECHO, HIGH);

  float distancia = duracao * 0.034 / 2;

  return distancia;
}
