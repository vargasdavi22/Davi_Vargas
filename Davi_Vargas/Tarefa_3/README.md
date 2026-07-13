## 4. Descrição das Tarefas (Estrutura Real do Código)

O firmware do robô foi estruturado em 4 tarefas periódicas no FreeRTOS, escalonadas estaticamente de acordo com a política **Rate Monotonic (RM)**. A comunicação entre as tarefas ocorre por meio de variáveis globais compartilhadas de alta velocidade:

---

## 1. `IR_Task` (Leitura de Linha)

* **Funcionamento:** Lê o estado físico dos 5 sensores infravermelhos seguidores de linha conectados aos GPIOs. Como o nível lógico ativo é `0` (linha preta detectada), a tarefa decodifica o padrão dos sensores e atualiza diretamente a variável global `line_error` com valores que variam de `-2` (totalmente deslocado para a esquerda) a `2` (totalmente deslocado para a direita), passando por `0` (centralizado).
* **Watchdog:** Incrementa a variável `ir_heartbeat` a cada ciclo para provar ao sistema que está rodando.
* **Justificativa de tempo real:** Rodando a cada 20 ms, o robô atualiza a leitura de pista a cada 1 cm percorrido (na velocidade de 0.50 m/s), tempo de reação fundamental para não passar direto nas curvas.

---

### 2. `wheel_ctrl` (Controle dos Motores e PID em Cascata)

* **Funcionamento:** É o coração do robô. A cada ciclo de 50 ms, ela realiza o seguinte fluxo:
  1. Lê os pulsos dos encoders físicos e calcula a velocidade linear real de cada roda em metros por segundo ($m/s$).
  2. Executa um **PID Angular** que transforma o erro de linha (`line_error`) em uma correção de velocidade de rotação.
  3. Aplica as equações de cinemática inversa para gerar a velocidade de referência individual de cada roda.
  4. Roda dois **PIDs individuais de velocidade** (um para cada roda) para corrigir o Duty Cycle enviado aos drivers de motor (`MCPWM`).
  * **Tratamento de Emergência:** * Se `obstacle_detected` estiver ativo, ela aplica uma rampa de frenagem controlada até parar o robô.
    * Se `fault_mode` for acionado pelo Watchdog, ela aborta o controle e injeta potência fixa em apenas um motor, fazendo o robô girar indefinidamente sobre o próprio eixo.
* **Watchdog:** Incrementa a variável `wheel_heartbeat` a cada loop.
* **Justificativa de tempo real:** O período de 50 ms acompanha a resposta física e a inércia mecânica das rodas e motores, permitindo uma leitura estável dos encoders sem oscilação por ruído.

---

### 3. `Threeeyes` (Sensor Ultrassônico)

* **Funcionamento:** Varre sequencialmente os 3 sensores ultrassônicos dianteiros para calcular a distância física até o obstáculo mais próximo.
  * Se a menor distância lida for menor ou igual a 10 cm (`STOP_DISTANCE_CM`), ela ativa a variável global `obstacle_detected = 1` imediatamente.
  * **Filtro de Histerese:** Para liberar o robô para voltar a andar, o obstáculo precisa ser afastado a pelo menos 15 cm (`RELEASE_DISTANCE_CM`) e o código exige **5 leituras limpas consecutivas** acima dessa distância, filtrando leituras instáveis.
* **Watchdog:** Incrementa a variável `threeeyes_heartbeat` a cada loop.
* **Justificativa de tempo real:** O tempo de 60 ms garante que as ondas sonoras emitidas pelo sensor tenham tempo de se propagar no ar e se dissipar, evitando que ecos antigos gerem medições fantasmas.

---

### 4. `Monitor_Task` (O Watchdog de Software e Hardware)

* **Funcionamento:** É o cérebro supervisor do robô e atua em duas camadas:
  1. **Watchdog de Hardware (TWDT):** Registra a si mesma no Watchdog nativo do silício do ESP32-S3 via `esp_task_wdt_add(NULL)` e limpa o contador com `esp_task_wdt_reset()`. Se a própria tarefa do monitor travar, o microcontrolador reinicia por completo.
  2. **Watchdog de Software:** Avalia se os contadores das outras três tarefas (`wheel_heartbeat`, `threeeyes_heartbeat` e `ir_heartbeat`) pararam de incrementar. Se qualquer tarefa congelar por 3 verificações seguidas (totalizando 600 ms), o monitor ativa a variável global `fault_mode = 1` para colocar o robô em rotação segura de emergência.
* **Justificativa de tempo real:** Sendo uma tarefa de supervisão de nível macro, rodar a cada 200 ms consome pouquíssima CPU e é rápido o bastante para impedir colisões se alguma tarefa de controle travar.
