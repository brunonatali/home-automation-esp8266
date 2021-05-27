# Automação residencial

Esse projeto tem como intuito principal automatizar o sistema de iluminação de um ou mais ambientes utilizando o módulo Wi-Fi ESP-12E baseado no microcontrolador [ESP8266](https://www.espressif.com/en/products/socs/esp8266).   

A ideia desse projeto surgiu a muito tempo mais recentemente com a disponibilização do microcontrolador programável da Espressif, tornou-se possível trazer ele a vida.   

O módulo escolhido (ESP-12E) é o móduulo que une a versatilidade da programação, quantidade satisfatórias de IOs e custo atraente, mas mesmo assim você verá que existem alguas limitações de hardware que serão pontuadas.   

Para o software utilizo a plataforma do Arduino para facilitar o código principal, mas as libs foram desenvolvidas em c++.  

**Table of Contents**
* [Serve para mim](#its-for-me)
* [Antes de começar](#first)
* [Material necessário](#hardware)
* [Mão na massa](#lets-do-it)
* [Software](#software)
    * [Coonfigurações](#config)
    * [Classes](#class-modules)
        * [Bounce](#bounce-effect)
        * [Flash](#flash)
        * [Saídas](#lighter-output)
        * [Touch](#touch-button)
    * [Página web](#web-page)
        * [Embarcando](#embedding-page)


## Serve para mim
Bom, se você chegou até aqui e está se perguntando se esse projeto serve para você, tenha em mente prncipalmente duas coisas:
- Suporte para até 6 botões touch
- Suporte para até 5 saídas de iluminação (dimerizável ou não)

Se isso for suficiente para você, seja bem vindo, e aproveite a navegação dessa criação.

## Antes de começar
Esse projeto não é apenas de software, claro que muita coisa aqui é relativa ao software, mas para manter um padrão de projeto e minimizar o tempo para finalização, faça a lição de casa e leia um pouco mais sobre o módulo, entenda como ele tem que ser operado (resistores de pull-up/down, alimentação ...).   

Seguem algumas sugestões de leitura:
- [IOs & pinouts](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/)
- [Module datasheet](https://www.espressif.com/sites/default/files/documentation/0a-esp8266ex_datasheet_en.pdf)
- [Entenda um pouco mais da sua capacidade](https://www.espressif.com/sites/default/files/documentation/esp8266_hardware_design_guidelines_en.pdf)
- [Chamadas do sistema operacional][https://www.espressif.com/sites/default/files/documentation/2c-esp8266_non_os_sdk_api_reference_en.pdf]

## Material necessário
- ESP-12E
- 6 botões touch em modo A (2 no mínimo)
- Fonte 3,3V
- Um punhado de fios (para ligar as coisas)

## Mão na massa
Antes de começar todo aquele bla-bla-bla da arquitetura do software, vamos montar o projeto e colocar ele pra funcionar.   

Para essa parte você vai precisar de:
- Conversor TTL-USB
- Ferro de solda
- Solda (para o ferro de solda =P)
- Visual Studio Code (fortemente recomendo a utilização de uma IDE diferente da do próprio Arduino) 

Vamos começar abrindo o arquivo chamado "eeprom_map_n_pin_layout.xlsx", eu detalho um pouco melhor a aba [EEPROM](#eeprom) desse arquivo mais para frente, mas neste momento iremos utilizar apenas a aba GPIO.   
Você verá 3 colunas nessa aba, GPIO, In, Out, Note, Use for, Pin. Cada uma delas será descrita numa outra ocasião, portanto vamos ao que interessa: 
- Cada botão touch está marcado em azul
- As cores na coluna Pin referem-se as saídas
- btn pwm é usado para dimmerizar a intensidade da luz dos botões 

Tudo o que tem que ser feito aqui, é ligar GPIO -> Pin conforme a tabela.   

Depois de tudo conectado, ligue ele com a GPIO 0 conectada ao GND e desconecte logo em seguida. Isso fará com que o ESP entre em modo de gravação.   

No VS Code, abra o projeto, na parte inferior configure a porta com do seu conversor TTL-USB e pressione ctrl + alt + u para fazer o upload do código para o módulo.  

Desconecte da alientação e reconecte. Você deve ver as luzes dos botões acenderem e uma delas (a última) irá apagar, indicando o final do processo de boot.   

Para garantir que está tudo funcionando, conecte-se ao Wi-Fi BN+MAC (ex. BN20:30:50:40:60:90) utilize a senha "semsenha" e acesse através do browser: http://192.168.4.1, você deverá ver uma imagem como na tela a seguir:

![main-app](/teste/img/main-git.png)

Aqui demonstrado com o menú (ícone central aberto)

## Software
O software foi baseado utilizando-se a plataforma do Arduino, dessa forma fica bem mais simples o entendimento e a modificação.   
O fluxo principal é:
- Lê as configurações da [Flash](#flash)
- Iniciliza todas as saídas [configuradas](#config)
- Inicializa os botões conforme configurações recebidas da Flash
- Inicializa o WiFi conforme modo de operação e configurações recebidas da Flash
- inicializa o servidor HTTP em / na porta 80

Maiores detalhes podem ser encontrados no arquivo main/Main.h

### Coonfigurações
As configuração de adaptação ao hardware podem ser encontradas em main/Main.h   

Algumas outras confgurações interessantes podem ser encontradas no arquivo main/FlashMan.h, como por exemplo a senha default para o wifi.

Desconsiderando as configurações que precedem "System used" na descição da variável, todas podem ser customizadas de acordo com a necessidade do seu hardware.
### Classes
Nesta sessão serão indicados os arquivos onde poderá ser obtido a ajuda, onde tudo está documentado
#### Bounce
main/BounceEffect.h
#### Flash
main/FlashMan.h
#### Saídas
main/Lighter.h
#### Touch
main/TouchButtonModule.h

### Página web
[Em desenvolvimento]   
Foi decidido que o sistema de backend e frontend desse projeto seria o mais enxuto possível, justamente para minimizar as latências e aumentar a experiência do usuário, desta forma a página principal contem o HTML, CSS e o JavaScript, e é enviada de uma única só vez.   

O backend possui apenas uma única rota (/), isso foi decidido assim para fazer os testes de interação de maneira mais rápida. Então ao clicar para salvar as configurações, como na figura a seguir, todas elas serão identificadas por queryes específicas.   
![config-page](/teste/img/config-modal-git.png)

A documentação da página pode ser encontrada em web/page.html. Vale ressaltar que que todas as variáveis foram cridas visando primordialmente o baixo consumo da flash, então elas tem nicknames relativos aos nomes ideais para boas práticas de programação.   

#### Embarcando
Para embarca a página web no código, você precisara remover:
- Comentários
- Tabulações (4 espaços) e caracteres de nova linha (\n)
- Escapar escapes (\ -> \\)
- Remover o final do código onde foi marcado

Após tudo removido, basta copiar o conteúdo e colar dentro do valor da variável 'localWebPage' atribuida durante a contrução da classe MCommunication em MCommunication.cpp   

Obs. Eu sei que da trabalho, um script para execução automática desse procedimento está sendo desenvolvido.