🌡️ Automação de Ar-Condicionado com ESP32 e Thinger.io
Um sistema inteligente para controle de ar-condicionado, utilizando ESP32, sensores e integração com a plataforma Thinger.io. Automatize seu ambiente e monitore tudo em tempo real!

🛠️ Visão Geral
Este projeto automatiza o controle de um ar-condicionado com base em:

Temperatura ambiente: Liga ou desliga o sistema automaticamente.
Detecção de movimento: Desliga o ar-condicionado quando não há movimento.
Controle manual: Permite que o usuário ligue/desligue o ar-condicionado pelo painel do Thinger.io.
Inclui funcionalidades visuais com LEDs indicadores e integra-se ao Wokwi, um simulador para facilitar o teste.

📑 Funcionalidades
🔄 Controle Automático:

Liga o ar-condicionado quando a temperatura ultrapassa o limite configurado e há movimento.
Desliga quando a temperatura está abaixo do limite ou não há movimento.
🎛️ Controle Manual:

O usuário pode controlar o sistema manualmente pelo Thinger.io, desabilitando o modo automático.
🖥️ Monitoramento Remoto:

Veja a temperatura em tempo real no painel do Thinger.io.
Configure os limites de temperatura e os modos de operação.
💡 Indicadores Visuais:

LED indica o estado do ar-condicionado (ligado/desligado).
LED de movimento pisca quando há presença detectada.
🔧 Hardware Utilizado

Componente	Descrição
🎛️ ESP32	Microcontrolador com Wi-Fi integrado

🌡️ DHT22	Sensor de temperatura e umidade

🚶 PIR	Sensor de movimento

💡 2 LEDs	Indicadores de estado

🔌 2 Resistores 220Ω	Limitação de corrente para LEDs

🔗 Protoboard & Jumpers	Para montagem e conexões do circuito

# 📊 Esquema do Circuito

## Conexões:
Componente	GPIO do ESP32	Descrição

DHT22	GPIO 15	Sensor de temperatura

PIR	GPIO 13	Sensor de movimento

LED do Ar-Condicionado	GPIO 14	Indica estado do ar-condicionado

LED de Movimento	GPIO 37	Pisca ao detectar movimento

#💻 Configuração do Ambiente

1. 🛠️ Configurar no Wokwi

Acesse Wokwi.
Use o arquivo JSON abaixo para configurar o ambiente no simulador:
<details> <summary>Clique para expandir</summary>
json
Copiar código
{
  "version": 1,
  "author": "Jonas_GS",
  "editor": "wokwi",
  "parts": [
    { "type": "board-esp32-s3-devkitc-1", "id": "esp", "top": 0, "left": 0, "attrs": {} },
    { "type": "wokwi-led", "id": "led1", "top": 130.8, "left": 272.6, "attrs": { "color": "red" } },
    { "type": "wokwi-dht22", "id": "dht1", "top": -153.3, "left": -91.8, "attrs": {} },
    { "type": "wokwi-pir-motion-sensor", "id": "pir1", "top": -149.6, "left": -208.98, "attrs": {} },
    { "type": "wokwi-resistor", "id": "r1", "top": 205.55, "left": 182.4, "attrs": { "value": "200" } },
    { "type": "wokwi-resistor", "id": "r2", "top": 32.75, "left": 192, "attrs": { "value": "200" } },
    { "type": "wokwi-led", "id": "led2", "top": -70.8, "left": 215, "attrs": { "color": "red" } }
  ],
  "connections": [
    [ "esp:TX", "$serialMonitor:RX", "", [] ],
    [ "esp:RX", "$serialMonitor:TX", "", [] ],
    [ "dht1:SDA", "esp:15", "green", [ "v0" ] ],
    [ "pir1:OUT", "esp:13", "green", [ "v0" ] ],
    [ "led1:C", "esp:GND.3", "green", [ "v0" ] ],
    [ "led2:C", "esp:GND.4", "green", [ "v0" ] ]
  ]
}
</details>

#2. 🌐 Configurar no Thinger.io
Acesse Thinger.io.
Crie um dispositivo com as credenciais:
USERNAME: Jonas_GS
DEVICE_ID: GS_IA
DEVICE_CREDENTIAL: 123456.
Configure os seguintes recursos:
Temperature: Monitora a temperatura ambiente.
AC_State: Indica se o ar-condicionado está ligado/desligado.
ManualControl: Permite alternar entre controle manual e automático.
SetTemperature: Configura o limite de temperatura.

#🔥 Demonstração

## 🌡️ Controle de Temperatura
Ajuste o limite de temperatura no Thinger.io.
Acompanhe o estado do ar-condicionado com base na leitura do sensor.

## 🚶 Controle de Presença
Simule movimento no Wokwi para ver o LED de presença piscando.

## 🛠️ Como Contribuir
Faça um fork do repositório.
Clone o projeto para o seu computador:
bash
Copiar código
git clone https://github.com/SeuUsername/NomeDoProjeto.git
Adicione melhorias e faça um pull request.
