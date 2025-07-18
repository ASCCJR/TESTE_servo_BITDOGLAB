# 🦾 Controle de Servo Motor com PWM Direto no Raspberry Pi Pico

![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry%20Pi%20Pico-purple.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-BitDogLab-purple.svg)
![Técnica](https://img.shields.io/badge/Técnica-PWM-yellow.svg)
![Atuador](https://img.shields.io/badge/Atuador-Servo%20Motor-orange.svg)


Este repositório demonstra uma técnica para controlar um servo motor padrão (ex: SG90) utilizando uma porta I2C de uma placa Raspberry Pi Pico (neste caso, a BitDogLab).

A solução não utiliza o protocolo I2C para comunicação. Em vez disso, o projeto **reconfigura um dos pinos de dados da porta I2C (SDA ou SCL) para atuar como uma saída PWM nativa do Pico**, gerando o sinal de controle que o servo motor entende. O código foi resumido em um único arquivo `main.c` 

## ✨ Funcionalidades

* **Controle de Servo Motor:** Controla o ângulo de um servo motor padrão (0-180 graus).
* **Geração de PWM Direta:** Utiliza a biblioteca `hardware/pwm` do Pico SDK para gerar um sinal preciso de 50Hz.
* **Reaproveitamento de Pinos:** Demonstra como um pino GPIO, mesmo que parte de um conector específico como o I2C, pode ser reconfigurado para outras funções.
* **Função de Mapeamento:** Inclui uma função helper para converter facilmente ângulos em graus para o valor de `duty cycle` do PWM.
* **Aplicação de Exemplo:** O código de teste varre o servo continuamente de 0 a 180 graus e de volta.

## 🛠️ Hardware e Software Necessários

### Hardware
* Placa com Raspberry Pi Pico (neste projeto, foi usada a **BitDogLab**).
* **Placa Adaptadora Passiva:** Uma placa que conecta o pino de sinal do servo a um dos pinos de dados da porta I2C.
* Servo Motor (SG90).

### Software
* [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk)
* Ambiente de compilação C/C++ (GCC para ARM, CMake).
* Um programa para monitor serial.

## 🚀 Como Usar

### Conexão do Hardware

**Conecte o Adaptador:** Use um cabo para conectar a porta **I2C 1 (Esquerda)** da sua BitDogLab à entrada I2C da sua placa adaptadora.

**Conecte o Servo:** Conecte o cabo do servo motor ao conector de 3 pinos no adaptador.

### Compilar

O projeto consiste em um único arquivo `main.c`. Certifique-se de que seu `CMakeLists.txt` está configurado para compilá-lo e linkar a biblioteca `hardware_pwm`.

### Executar o Teste
* Com tudo conectado e alimentado, o servo motor deverá começar a se mover suavemente de 0 a 180 graus, e de volta, continuamente. As mensagens de log no monitor serial indicarão o ângulo atual.

## Visualizar a Saída

```bash
--- Teste de Servo Motor com PWM Direto do Pico ---
Controlando o servo no pino GPIO2
PWM inicializado. Movendo o servo...
Movendo para o angulo: 0
Movendo para o angulo: 5
Movendo para o angulo: 10
Movendo para o angulo: 15
Movendo para o angulo: 20
Movendo para o angulo: 25
Movendo para o angulo: 30
```

### 📂 Estrutura dos Arquivos

* main.c: Contém toda a lógica do programa, incluindo a configuração do PWM, a função de conversão de ângulo e o loop de controle.
* CMakeLists.txt: O arquivo de build do projeto.
