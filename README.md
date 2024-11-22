# 🌡️ Automação de Ar-Condicionado com ESP32 e Thinger.io
Controle inteligente de ar-condicionado utilizando ESP32, sensores e integração com a plataforma Thinger.io. Automatize seu ambiente, economize energia e acompanhe tudo em tempo real!

# 📜 Sumário
Introdução

Objetivo do Projeto

Características do Sistema

Componentes Utilizados

Esquema do Circuito

Código-Fonte

Configuração no Wokwi

Configuração no Thinger.io

Passo a Passo para Testar

Fluxo de Operação

Imagens e Demonstrações

# 📖 Introdução
Este projeto utiliza um ESP32 para criar um sistema de automação de ar-condicionado que responde a condições ambientais (temperatura e movimento). A integração com a plataforma Thinger.io permite monitoramento remoto e controle manual via um painel na nuvem.

## O sistema foi projetado para:

Facilitar a automação em ambientes domésticos ou comerciais.
Melhorar o conforto e a eficiência energética.
Demonstrar a viabilidade de soluções IoT usando ferramentas acessíveis.


# 🎯 Objetivo do Projeto
Desenvolver uma solução IoT para automação de ar-condicionado.
Utilizar sensores para monitoramento e tomada de decisões automáticas.
Fornecer controle remoto com uma interface simples e intuitiva via Thinger.io.
Permitir simulação completa no Wokwi para testes antes da implementação real.

# 🔍 Características do Sistema
Controle Inteligente de Temperatura:

Liga o ar-condicionado quando a temperatura excede um limite configurado.
Desliga o ar-condicionado quando a temperatura estiver abaixo do limite.
Controle Manual:

O usuário pode ligar/desligar manualmente via Thinger.io, ignorando o controle automático.
Monitoramento em Tempo Real:

Monitore a temperatura ambiente e o estado do sistema.
Indicadores Visuais:

LEDs para indicar o estado do ar-condicionado (ligado/desligado) e a presença detectada.
Simulação no Wokwi:

Todo o sistema pode ser testado no navegador antes da implementação.

# 🛠️ Componentes Utilizados

## Hardware

### Componente	Descrição

ESP32	Microcontrolador com Wi-Fi integrado

DHT22	Sensor de temperatura e umidade

PIR	Sensor de movimento

LEDs (x2)	Indicadores de estado

Resistores 220Ω	Limitação de corrente para LEDs

Protoboard & Jumpers	Conexões para prototipação

### Software

Wokwi: Simulador para ESP32 e periféricos.
Thinger.io: Plataforma IoT para controle remoto e monitoramento.

# 📊 Esquema do Circuito

O diagrama abaixo mostra as conexões entre o ESP32 e os componentes:


## Tabela de Conexões
Componente	ESP32 GPIO	Descrição

DHT22	GPIO 15	Sensor de temperatura

PIR	GPIO 13	Sensor de movimento

LED (Ar-Condicionado)	GPIO 14	Indica estado do ar-condicionado

LED (Presença)	GPIO 37	Indica detecção de movimento

# 💻 Código-Fonte

## O código completo para o sistema está disponível no arquivo codigo.ino. Abaixo está um trecho principal para referência:
cpp
Copiar código

#include <ThingerESP32.h>
#include "DHT.h"

// Credenciais do Thinger.io
#define USERNAME "Jonas_GS"
#define DEVICE_ID "GS_IOT"
#define DEVICE_CREDENTIAL "123456"

// Credenciais Wi-Fi
#define SSID "Wokwi-GUEST"
#define SSID_PASSWORD ""

// Configuração do DHT22
#define DHTPIN 15      // GPIO conectado ao DHT22
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Configuração do sensor PIR
#define PIRPIN 13      // GPIO conectado ao PIR Sensor

// LEDs
#define LED_AC 14      // GPIO para LED do ar-condicionado
#define LED_PRES 37    // GPIO para LED do sensor de presença

// Cliente Thinger.io
ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

// Estados iniciais
bool acOn = false;                 // Estado do ar-condicionado
bool manualControl = false;        // Controle manual do ar-condicionado
bool presenceEnabled = true;       // Estado do sensor de presença
bool stableMotionDetected = false; // Estado estável do sensor PIR
float temperatureThreshold = 24.0; // Limite padrão para ativar o ar-condicionado

// Intervalos e temporizadores
const unsigned long updateInterval = 5000;       // Tempo entre atualizações (5 segundos)
const unsigned long presenceBlinkInterval = 500; // Tempo de piscar do LED do sensor de presença
const unsigned long pirDebounceTime = 2000;      // Tempo para validar presença (2 segundos)
unsigned long lastUpdate = 0;
unsigned long lastPresenceBlink = 0;
unsigned long pirLastMotionTime = 0; // Última detecção de movimento
bool presenceBlinkState = false;

// Valor de histerese
const float hysteresis = 1.0;

void setup() {
  // Inicialização Serial
  Serial.begin(115200);

  // Configuração de pinos
  pinMode(PIRPIN, INPUT);
  pinMode(LED_AC, OUTPUT);
  pinMode(LED_PRES, OUTPUT);
  digitalWrite(LED_AC, LOW);
  digitalWrite(LED_PRES, LOW);

  // Inicialização do DHT22
  dht.begin();

  // Configuração de rede Wi-Fi
  thing.add_wifi(SSID, SSID_PASSWORD);

  // Recursos do Thinger.io

  // Envia a temperatura ao Thinger.io
  thing["Temperature"] >> [](pson &out) {
    float temperature = dht.readTemperature();
    if (!isnan(temperature)) {
      out = temperature;
    }
  };

  // Envia o estado do ar-condicionado
  thing["AC_State"] >> [](pson &out) {
    out = acOn ? "Ligado" : "Desligado";
  };

  // Controle manual do ar-condicionado
  thing["ManualControl"] << [](pson &in) {
    if (!in.is_empty()) {
      manualControl = (bool)in;
      acOn = manualControl;
      presenceEnabled = !manualControl; // Desabilita o sensor de presença no modo manual
      if (!presenceEnabled) {
        digitalWrite(LED_PRES, LOW);
      }
      updateACLED();

      // Debug: Exibe o valor recebido
      Serial.print("ManualControl recebido: ");
      Serial.println(manualControl ? "Ligado" : "Desligado");
    }
  };

  // Ajusta o limite de temperatura
  thing["SetTemperature"] << [](pson &in) {
    if (!in.is_empty() && (float)in > 0) {
      temperatureThreshold = (float)in;

      // Debug: Exibe o novo limite configurado
      Serial.print("Novo limite de temperatura: ");
      Serial.println(temperatureThreshold);
    }
  };
}

// Atualiza o LED do ar-condicionado
void updateACLED() {
  digitalWrite(LED_AC, acOn ? HIGH : LOW);
}

// Pisca o LED do sensor de presença se o sensor estiver ativo
void blinkPresenceLED() {
  if (!presenceEnabled) {
    digitalWrite(LED_PRES, LOW); // Garante que o LED está desligado
    return;
  }
  if (millis() - lastPresenceBlink >= presenceBlinkInterval) {
    lastPresenceBlink = millis();
    presenceBlinkState = !presenceBlinkState;
    digitalWrite(LED_PRES, presenceBlinkState ? HIGH : LOW);
  }
}

// Debounce para o sensor PIR
void handlePIR() {
  int motionDetected = digitalRead(PIRPIN);
  if (motionDetected == HIGH) {
    pirLastMotionTime = millis(); // Atualiza o tempo da última detecção
  }
  stableMotionDetected = (millis() - pirLastMotionTime <= pirDebounceTime);
}

void loop() {
  thing.handle(); // Processa mensagens do Thinger.io

  unsigned long currentMillis = millis();

  // Piscar o LED do sensor de presença
  if (presenceEnabled) {
    blinkPresenceLED();
  }

  // Leitura do sensor PIR
  handlePIR();

  // Lógica de controle do ar-condicionado
  if (currentMillis - lastUpdate >= updateInterval) {
    lastUpdate = currentMillis;

    // Ignora a lógica automática se o controle manual estiver ativo
    if (manualControl) {
      return;
    }

    // Leitura do sensor de temperatura
    float temperature = dht.readTemperature();
    if (isnan(temperature)) {
      Serial.println("Erro ao ler a temperatura!");
      return;
    }

    // Controle automático baseado no PIR e temperatura
    if (presenceEnabled && !stableMotionDetected) {
      acOn = false;
      Serial.println("Sem presença detectada. Ar-condicionado desligado.");
    } else {
      if (temperature >= temperatureThreshold + hysteresis) {
        acOn = true;
        Serial.println("Temperatura acima do limite. Ar-condicionado ligado.");
      } else if (temperature <= temperatureThreshold - hysteresis) {
        acOn = false;
        Serial.println("Temperatura abaixo do limite. Ar-condicionado desligado.");
      }
    }
    // Atualiza o LED do ar-condicionado
    updateACLED();
  }
}




# 🌐 Configuração no Wokwi

### Acesse o Wokwi.
Crie um novo projeto e selecione o ESP32.
Substitua o conteúdo do editor com o seguinte JSON:
<details> <summary>📄 JSON para Configuração do Hardware</summary>

 json
Copiar código

{
  "version": 1,
  "author": "Anonymous maker",
  "editor": "wokwi",
  "parts": [
    { "type": "board-esp32-s3-devkitc-1", "id": "esp", "top": 0, "left": 0, "attrs": {} },
    {
      "type": "wokwi-led",
      "id": "led1",
      "top": 130.8,
      "left": 272.6,
      "attrs": { "color": "red" }
    },
    { "type": "wokwi-dht22", "id": "dht1", "top": -153.3, "left": -91.8, "attrs": {} },
    {
      "type": "wokwi-pir-motion-sensor",
      "id": "pir1",
      "top": -149.6,
      "left": -208.98,
      "attrs": {}
    },
    {
      "type": "wokwi-resistor",
      "id": "r1",
      "top": 205.55,
      "left": 182.4,
      "attrs": { "value": "200" }
    },
    {
      "type": "wokwi-resistor",
      "id": "r2",
      "top": 32.75,
      "left": 192,
      "attrs": { "value": "200" }
    },
    { "type": "wokwi-led", "id": "led2", "top": -70.8, "left": 215, "attrs": { "color": "red" } }
  ],
  "connections": [
    [ "esp:TX", "$serialMonitor:RX", "", [] ],
    [ "esp:RX", "$serialMonitor:TX", "", [] ],
    [ "dht1:SDA", "esp:15", "green", [ "v0" ] ],
    [ "dht1:GND", "esp:GND.2", "black", [ "v19.2", "h144" ] ],
    [ "pir1:VCC", "esp:3V3.2", "red", [ "v0" ] ],
    [ "pir1:OUT", "esp:13", "green", [ "v0" ] ],
    [ "pir1:GND", "esp:GND.1", "black", [ "v0" ] ],
    [ "dht1:VCC", "esp:3V3.1", "red", [ "v0" ] ],
    [ "r1:1", "esp:14", "green", [ "v0" ] ],
    [ "led1:A", "r1:2", "green", [ "v0" ] ],
    [ "led1:C", "esp:GND.3", "green", [ "v0" ] ],
    [ "r2:1", "esp:37", "green", [ "v0" ] ],
    [ "r2:2", "led2:A", "green", [ "v0" ] ],
    [ "led2:C", "esp:GND.4", "green", [ "v0" ] ]
  ],
  "dependencies": {}
}
</details>

# 🌐 Configuração no Thinger.io
Acesse Thinger.io.
Crie um dispositivo com:
Username: Jonas_GS
Device ID: GS_IA
Device Credential: 123456.
Configure os seguintes recursos:
Temperature: Para monitorar a temperatura.
ManualControl: Para alternar entre controle manual e automático.

# 🚀 Passo a Passo para Testar

 Simulação no Wokwi
Configure o hardware e o código.
Inicie a simulação e abra o Serial Monitor para verificar as mensagens.
 
 Testes no Thinger.io
Acesse o painel e use os recursos:
Temperature: Leia a temperatura.
ManualControl: Ligue ou desligue manualmente o ar-condicionado.
SetTemperature: Ajuste o limite de temperatura.

# 🔄 Fluxo de Operação
Inicialização:

O ESP32 conecta ao Wi-Fi e autentica no Thinger.io.
Operação Automática:

Liga o ar-condicionado quando:
Temperatura > Limite.
Movimento detectado.
Desliga o ar-condicionado quando:
Temperatura < Limite.
Sem movimento detectado.
Operação Manual:

O usuário controla manualmente via painel do Thinger.io.

# 📸 Imagens e Demonstrações
(![Captura de tela 2024-11-22 013411](https://github.com/user-attachments/assets/5ef081c5-0ed3-4943-b206-00055281b1a6)
)

(![Captura de tela 2024-11-22 015002](https://github.com/user-attachments/assets/12d2e7fb-ee04-49f7-bc7f-edeb1defd93a)
)

# 🎥 Simulação no Wokwi

(![Captura de tela 2024-11-22 015856](https://github.com/user-attachments/assets/ef983fe1-37e2-406a-bf41-fec36bb1cad6)
)

# 🌐 Painel do Thinger.io
(![Captura de tela 2024-11-22 015955](https://github.com/user-attachments/assets/683fdb84-1afe-4c1c-803f-96d102ae0299)
)

(![Captura de tela 2024-11-22 015930](https://github.com/user-attachments/assets/ea0ed8b2-5543-4889-a5f5-48822e920439)
)

