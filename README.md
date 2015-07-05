![JAGA logo](https://github.com/JorgeGzm/JAGA/blob/master/docs/logo.jpg)

# JAGA 

English:

This project has the proposal developing libraries universals to  microcontrolers with architecture 8, 16 and 32bits.

-------------------------------------------------------------------------------------------------------------------

Português:

O projeto JAGA foi idealizado por Jorge Guzman e Rafael Lopes e tem como objetivos criar bibliotecas universais usando a linguagem C. A construção de cada lib deverá garantir uma boa performance, modularidade, portabilidade do código para microcontroladores com arquiteturas de 8, 16 e 32 bits e de fácil integração com um RTOS.

O desenvolvimento inicial das libs será focado nos periféricos mais usados no ensino de sistemas embarcados e em shields arduinos que podem ser facilmente adquiridos.

A arquitetura do software é baseada na comunicação de 3 camadas: Application, Middleware e HAL (Hardware Application Layer). A comunicação entre as camadas se dá por meio da abstração de dados, orientação a eventos e as seguintes regras abaixo:

Camada de Aplicação:
- Evitar chamadas horizontais na camada de aplicação
- Somente chamar funções de funcionalidade da camada de Middleware.

Camada de Middleware:
- Não pode haver, de maneira alguma, chamadas horizontais;
- Só pode #incluir .h das bibliotecas da camada de HAL

Camada HAL (Driver):
- Só existe para permitir portabilidade de plataforma e organizar o acesso aos periféricos.
- Fornecerá funções abstraídas para configurar e acessar os registradores do microcontrolador específico.
- É a única camada cujas bibliotecas podem fazer chamadas horizontais.

Bibliotecas Independentes(Util):
- Bibliotecas que não dependem de nenhuma outra podem ser independentes, então podem ser chamadas de qualquer camada. ex: vars.h, crc.h, printf.h;

O maior intuito deste projeto é criar, ensinar e compartilhar conhecimento para o desenvolvimento de um bom firmware.

Referências:
http://www.embarcados.com.br/arquitetura-de-software-em-sistemas-embarcados/
https://selivre.wordpress.com/2011/10/15/praticas-para-a-qualidade-design-da-firmware/
