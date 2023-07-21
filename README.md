# EcoX

## Sobre

<p align ="justify"> O projeto EcoX consiste em projetar e desenvolver um veículo autônomo
para transporte de cargas em linhas fechadas de indústrias. Sua
principal função será transportar uma carga de até 5kg de um ponto A até um ponto B
dentro de uma indústria.

O projeto é desenvolvido por alunos de graduação em Engenharia Aeroespacial, Engenharia Automotiva, Engenhraria de Energia, Engenharia Eletrônica e Engenharia de Software da Universidade de Brasília, do Campus do Gama (FGA), para a disciplina de Projeto Integrador 2.

### Integrantes

| Membro             | Engenharia | Sistemas         | 
| ------------------ | ---------------- | ---------- |
| [João Paulo Trindade]() | Aeroespacial | Mecânicos |
| [Moriyoshi Tsuchiya](https://github.com/moriyoshi-st) | Aeroespacial | Mecânicos |
| [Tales Brandão]() | Aeroespacial | Mecânicos |
| [Victor Araújo](https://github.com/VictorSantos1999) | Automotiva | Mecânicos |
| [Daniel de Melo](https://github.com/danielcardosofga) | Eletrônica | Eletrônicos |
| [João Vítor Fonseca](https://github.com/joaojjvitor) | Eletrônica | Eletrônicos |
| [João Vitor Silva](https://github.com/jvses) | Eletrônica | Eletrônicos |
| [Ricardo Miguel](https://github.com/RicardoMiguel21) | Energia | Eletrônicos |
| [Erick Melo](https://github.com/ErickMVdO) | Software | Interface |
| [João Pedro Moura](https://github.com/Joao-Pedro-Moura) | Software | Interface |
| [Marcos Felipe](https://github.com/Marofelipe) | Software | Interface |
| [Mateus Maia](https://github.com/mateusmaiamaia) | Software | Interface |
| [Matheus Afonso]() | Software | Interface |
| [Paulo Batista](https://github.com/higton) | Software | Interface |

## Como usar

### Frontend

Para rodar o frontend, é necessário ter o [Node.js](https://nodejs.org/en/) instalado. Após isso, basta rodar os seguintes comandos:

```bash
cd carnalize
npm install
npm start
```

### Backend - Servidor de comunicação

Para rodar o servidor de comunicação, é necessário usar o Python. Após isso, basta rodar os seguintes comandos:

```bash
cd socket_version_veiculo_autonomo
cd system-test-server
python3 server.py
```

### Simulação - Gazebo

Para rodar a simulação, é necessário ter o [ROS](https://www.ros.org/) instalado. Após isso, basta rodar os seguintes comandos:

```bash
cd ros2_simulation
cd my_ws
colcon build --symlink-install
source install/setup.bash
ros2 launch articubot_one launch_sim.launch.py world:=./src/articubot_one/worlds/test2.world
```

Para rodar a parte de detecção da placa de sinalização, abra um novo terminal e rode o seguinte comando:

```bash
ros2 run ball_tracker detect_stop_sign --ros-args -p tuning_mode:=true -r image_in:=camera/image_raw
```

Para rodar a parte de detecção da linha, abra um novo terminal e rode o seguinte comando:

```bash
ros2 run ball_tracker detect_path --ros-args -p tuning_mode:=true -r image_in:=camera/image_raw
```

Agora para fazer o carro andar, basta rodar o seguinte comando em um novo terminal:

```bash
ros2 run ball_tracker follow_path --ros-args -r cmd_vel:=cmd_vel_tracker
ros2 service call /start_follower std_srvs/srv/Empty "{}"
```

Para usar no aplicativo, basta rodar o seguinte comando em um novo terminal:

```bash
ros2 run rosbridge_server rosbridge_websocket
```

Para ver o video em tempo real do carro no aplicativo, basta rodar o seguinte comando em um novo terminal:

```bash
ros2 run web_video_server web_video_server
```
