# GS_IA

Projeto: Automação de Ar-Condicionado com ESP32 e Thinger.io
Este projeto automatiza o controle de um ar-condicionado com base em temperatura ambiente e detecção de movimento. Ele foi desenvolvido para funcionar com o microcontrolador ESP32, utilizando sensores para monitoramento do ambiente e LEDs para indicar estados do sistema. Todo o controle e monitoramento podem ser feitos através da plataforma Thinger.io, que permite ajustar configurações e visualizar dados em tempo real.

Objetivos
Automação Inteligente: Automatizar o ar-condicionado para economizar energia e melhorar o conforto, ligando-o apenas quando necessário.
Monitoramento em Tempo Real: Exibir a temperatura e o estado do sistema em um painel remoto (Thinger.io).
Controle Manual: Permitir que o usuário tenha controle total sobre o sistema, independentemente das condições ambientais.
Simulação Completa no Wokwi: Oferecer uma maneira prática de testar o projeto antes da implementação real.
Recursos do Projeto

Controle Automático:

O ar-condicionado é ligado quando a temperatura ultrapassa o limite configurado e há presença detectada.
O ar-condicionado é desligado automaticamente quando não há movimento por um período ou a temperatura está abaixo do limite.
Controle Manual:

O usuário pode ativar ou desativar o sistema manualmente pelo painel do Thinger.io, desabilitando o controle automático.
Feedback Visual:

Um LED indica o estado do ar-condicionado (ligado/desligado).
Outro LED pisca para indicar detecção de movimento.
Personalização via Thinger.io:


Configuração do limite de temperatura.

Alternância entre controle manual e automático.

Monitoramento do estado do sistema em tempo real.

Componentes Utilizados

Hardware

Componente	Descrição

ESP32	Microcontrolador Wi-Fi

DHT22	Sensor de temperatura e umidade

PIR	Sensor de movimento

2 LEDs	Indicação de estados

2 Resistores (220Ω)	Para limitar a corrente dos LEDs

Protoboard e Jumpers	Para montagem dos circuitos
Software

Wokwi: Simulador para o ESP32 e periféricos.

Thinger.io: Plataforma IoT para controle remoto e monitoramento.

Bibliotecas:

ThingerESP32: Para integração com o Thinger.io.

DHT: Para leitura do sensor DHT22.

Diagrama do Circuito

O circuito foi configurado no Wokwi e segue o seguinte esquema:

plaintext
Copiar código
ESP32                     DHT22
(VCC) ------------------> (VCC)
(GND) ------------------> (GND)
(GPIO 15) -------------> (DATA)

ESP32                     PIR
(VCC) ------------------> (VCC)
(GND) ------------------> (GND)
(GPIO 13) -------------> (OUT)

ESP32                     LED AC
(GPIO 14) -------------> (Resistor -> Ânodo LED)
(GND) ------------------> (Cátodo LED)

ESP32                     LED Presença
(GPIO 37) -------------> (Resistor -> Ânodo LED)
(GND) ------------------> (Cátodo LED)

Configuração no Wokwi

Arquivo JSON
Utilize o seguinte JSON para configurar o ambiente no Wokwi:

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

Código no Wokwi

Insira o código no editor do Wokwi, como descrito anteriormente.

Configuração no Thinger.io

Acesse o painel do Thinger.io: https://console.thinger.io/.
Crie um dispositivo com as credenciais:

USERNAME: Jonas_GS

DEVICE_ID: GS_IA

DEVICE_CREDENTIAL: 123456.

Configure os recursos:

Temperature: Mostra a temperatura lida pelo DHT22.

AC_State: Indica o estado do ar-condicionado (ligado/desligado).

ManualControl: Permite alternar entre controle manual e automático.

SetTemperature: Ajusta o limite de temperatura.

Como Funciona

Estado Automático:

Se o PIR detectar movimento e a temperatura estiver acima do limite configurado, o ar-condicionado liga.
Se não houver movimento por 2 segundos ou a temperatura estiver abaixo do limite, ele desliga.
Controle Manual:

Pelo painel do Thinger.io, o usuário pode ativar ou desativar o ar-condicionado manualmente, ignorando as condições automáticas.
Visualização no Thinger.io:

Monitorar a temperatura e o estado do ar-condicionado em tempo real.
Configurar os limites de temperatura e alternar modos.
