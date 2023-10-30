const int botaoPin = 2;     // Pino do botão
const int ledPin = 13;      // Pino do LED embutido no Arduino
const int led1 = 12;     // Pino do LED piscando

unsigned long tempoInicial = 0;   // Variável para armazenar o tempo inicial
unsigned long tempoEspera = 10000; // Tempo de espera em milissegundos (10 segundos)

unsigned long tempoPressionado = 0;  // Tempo que o botão foi pressionado
unsigned long tempoLiberado = 0;     // Tempo que o botão foi liberado

boolean modoProtegido = false;       // Variável para indicar se estamos no modo protegido
boolean botaoAnterior = LOW;        // Armazena o estado anterior do botão
boolean botaoEstavel = LOW;         // Estado estável do botão

void setup() {
  pinMode(botaoPin, INPUT);    // Configura o pino do botão como entrada
  pinMode(ledPin, OUTPUT);     // Configura o pino do LED embutido como saída
  digitalWrite(ledPin, LOW);   // Desliga o LED inicialmente
  pinMode(led1, OUTPUT);       // Configura o pino do LED1 piscante como saída
  
  // Inicializa a comunicação com o monitor serial
  Serial.begin(9600);
}

void ledpisca(int pin,int duration) {
  digitalWrite(pin, HIGH);  // Liga o LED
  delay(duration);  // Aguarda a duração especificada
  digitalWrite(pin, LOW);   // Desliga o LED
  delay(duration);  // Aguarda a duração especificada novamente
}

void loop() {
  // Lê o estado atual do botão
  boolean botaoAtual = digitalRead(botaoPin);

  // Verifica o debounce do botão
  if (botaoAtual != botaoAnterior) {
    // Se o estado do botão mudou
    delay(50); // Aguarda um curto período de tempo para evitar flutuações
    botaoAtual = digitalRead(botaoPin); // Lê o estado novamente
  }

  // Se o estado é estável, prosseguimos
  if (botaoAtual != botaoEstavel) {
    // Se o botão mudou para um estado estável
    botaoEstavel = botaoAtual;

    if (botaoEstavel == HIGH && !modoProtegido) {
      tempoInicial = millis();
      modoProtegido = true;
      digitalWrite(ledPin, HIGH);
    } else if (botaoEstavel == LOW) {
      tempoLiberado = millis();
      tempoPressionado = tempoLiberado - tempoInicial;
    }
  }

  // Verifica se o tempo de espera acabou
  if (modoProtegido && millis() - tempoInicial >= tempoEspera) {
    modoProtegido = false;
    digitalWrite(ledPin, LOW);
         
    Serial.print("Botao pressionado por ");
    Serial.print(tempoPressionado);
    Serial.println(" milissegundos.");
  }

  // Armazena o estado atual do botão para o próximo ciclo
  botaoAnterior = botaoAtual;

  // Outras tarefas podem ser executadas aqui enquanto aguarda o tempo passar
  ledpisca(led1, 1000);  // Piscar o primeiro LED a cada segundo
}
