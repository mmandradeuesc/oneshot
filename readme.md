# ONESHOT

## Tarefa 2 - Aula Síncrona (29/01)

### Integrante  
- **Marcel Mascarenhas Andrade**  

## 📌 Pré-requisitos  
Certifique-se de que seu ambiente está configurado corretamente antes de compilar e executar o programa:  

- **Pico SDK** instalado e configurado.  
- **VSCode** com todas as extensões necessárias.  
- **CMake** e **Make** instalados.  

## 📝 Funcionamento do Programa
O programa implementa um controle de LEDs acionado por um botão. A lógica de funcionamento é:

Acionamento inicial:

Quando o botão (pushbutton) for pressionado, os três LEDs serão ligados simultaneamente (nível alto).
Após um atraso de 3 segundos, o LED azul será desligado.
Após mais 3 segundos, o LED vermelho será desligado.
Finalmente, após mais 3 segundos, o LED verde será desligado.
Regras de operação:

O botão não pode iniciar uma nova sequência enquanto a atual estiver em execução.
A transição dos LEDs é controlada por callbacks do temporizador, seguindo o exemplo estudado na aula síncrona (turn_off_callback()).

## ⚙️ Configuração e Execução  

```sh
# Clone o repositório
git clone https://github.com/mmandradeuesc/oneshot.git
cd seu-repositorio

# Abra o projeto no VSCode
code .

# Compile o código
# (Certifique-se de que a extensão Pi Pico esteja configurada corretamente)