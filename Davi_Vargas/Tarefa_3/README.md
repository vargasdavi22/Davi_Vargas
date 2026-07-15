## Descrição das Tarefas (Estrutura Real do Código)

O firmware do robô foi estruturado em 4 tarefas periódicas no FreeRTOS, escalonadas estaticamente de acordo com a política **Rate Monotonic (RM)**. A comunicação entre as tarefas ocorre por meio de variáveis globais compartilhadas de alta velocidade:

---

## 1. `ir_line_task` (Leitura de Linha)

* **Funcionamento:** Lê o estado físico dos 5 sensores infravermelhos seguidores de linha conectados aos GPIOs. Como o nível lógico ativo é `0` (linha preta detectada), a tarefa decodifica o padrão dos sensores e atualiza diretamente a variável global `line_error` com valores que variam de `-2` (totalmente deslocado para a esquerda) a `2` (totalmente deslocado para a direita), passando por `0` (centralizado).
* **Watchdog:** Incrementa a variável `ir_heartbeat` a cada ciclo para provar ao sistema que está rodando.
* **Justificativa de tempo real:** Rodando a cada 20 ms, o robô atualiza a leitura de pista a cada 1 cm percorrido (na velocidade de 0.50 m/s), tempo de reação fundamental para não passar direto nas curvas.

---

### 2. `wheel_task` (Controle dos Motores e PID em Cascata)

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

### 3. `Threeeyes_task` (Sensor Ultrassônico)

* **Funcionamento:** Varre sequencialmente os 3 sensores ultrassônicos dianteiros para calcular a distância física até o obstáculo mais próximo.
  * Se a menor distância lida for menor ou igual a 10 cm (`STOP_DISTANCE_CM`), ela ativa a variável global `obstacle_detected = 1` imediatamente.
  * **Filtro de Histerese:** Para liberar o robô para voltar a andar, o obstáculo precisa ser afastado a pelo menos 15 cm (`RELEASE_DISTANCE_CM`) e o código exige **5 leituras limpas consecutivas** acima dessa distância, filtrando leituras instáveis.
* **Watchdog:** Incrementa a variável `threeeyes_heartbeat` a cada loop.
* **Justificativa de tempo real:** O tempo de 60 ms garante que as ondas sonoras emitidas pelo sensor tenham tempo de se propagar no ar e se dissipar, evitando que ecos antigos gerem medições fantasmas.

---

### 4. `Monitor_Task` (O Watchdog de Software e Hardware)

* **Funcionamento:** É o cérebro supervisor do robô e atua em duas camadas:
  1. **Watchdog de Hardware (TWDT):** Registra a si mesma no Watchdog nativo do silício do ESP32-S3 via `esp_task_wdt_add(NULL)` e limpa o contador com `esp_task_wdt_reset()`. Se a própria tarefa do monitor travar, o microcontrolador reinicia por completo.
  2. **Watchdog de Software:** Avalia se os contadores das outras três tarefas (`wheel_heartbeat`, `threeeyes_heartbeat` e `ir_heartbeat`) pararam de incrementar. Se qualquer tarefa congelar por 3 verificações

### Validação Analítica de Escalonabilidade (Teste de Utilização)

A partir das medições reais de pior tempo de execução (WCET) coletadas no osciloscópio, foram obtidos os seguintes tempos de computação ($C_i$):
* **C_ir_line** = $1,82\ \mu\text{s}$ ($0,00182\text{ ms}$)
* **C_wheel** = $13,2\ \mu\text{s}$ ($0,0132\text{ ms}$)
* **C_treeeyes** = $4,24\text{ ms}$
* **C_monitor** = $3,32\ \mu\text{s}$ ($0,00332\text{ ms}$)

Aplicando o teorema de utilização de Liu e Layland para $n = 4$ tarefas:

$$U = \sum_{i=1}^{4} \frac{C_i}{T_i} \approx \frac{0,00182}{20} + \frac{0,0132}{50} + \frac{4,24}{60} + \frac{0,00332}{200}$$

$$U \approx 0,0091\% + 0,0264\% + 7,066\% + 0,00166\% \approx 7,10\%$$

O limite superior de escalonabilidade garantido para o algoritmo Rate Monotonic com 4 tarefas é dado por:

$$U_{\text{lim}} = 4(2^{1/4} - 1) \approx 75,6\%$$

Como a utilização real do sistema é de apenas **$7,10\%$**, o que é significativamente menor que o limite matemático de **$75,6\%$**, fica cientificamente provado que **o sistema é perfeitamente escalonável**. Todas as tarefas têm garantia matemática de que cumprirão seus deadlines, restando ainda $92,9\%$ de tempo de CPU ocioso no processador ESP32-S3.

## Medições Reais de WCET no Osciloscópio

Abaixo estão detalhados os piores tempos de execução (WCET) de cada tarefa, medidos diretamente através da instrumentação de pinos GPIO de teste e analisados em um osciloscópio digital Tektronix TBS 1062:

| Tarefa | Período ($T_i$) | WCET Real Medido ($C_i$) | Utilização da CPU ($C_i / T_i$) |
| :--- | :---: | :---: | :---: |
| `ir_line`| 20 ms | **1,82 µs** (0,00182 ms) | 0,0091% |
| `wheel_ctrl` | 50 ms | **13,20 µs** (0,01320 ms) | 0,0264% |
| `threeeyes`| 60 ms | **4,24 ms** | 7,0666% |
| `monitor` | 200 ms | **3,32 µs** (0,00332 ms) | 0,0016% |

---

### Capturas do Osciloscópio (Sinais de WCET)

#### A. Leitura de Linha (`ir_line`)
* **Tempo medido:** 1,82 µs 
<p align="center">
<img width="300" height="400" alt="IMG_6983" src="https://github.com/user-attachments/assets/2b480996-9a63-4fbf-a780-cd28d424861a" />

</p>

#### B. Controle e PID (`wheel_ctrl`)
* **Tempo medido:** 13,20 µs 
<p align="center">
  <img width="300" height="400" alt="IMG_6985" src="https://github.com/user-attachments/assets/4867a463-ff98-4ebf-a2ed-9d424b376e5a" />


</p>

#### C. Sensor Ultrassônico (`threeeyes`)
* **Tempo medido:** 4,24 ms 
<p align="center">
  <img width="300" height="400" alt="IMG_6984" src="https://github.com/user-attachments/assets/fb40fcfb-7c0e-49ba-8dd4-3c7808b8e458" />
</p>

#### D. Monitor de Sistema (`Monitor_Task`)
* **Tempo medido:** 3,32 µs 
<p align="center">
  <img width="300" height="400" alt="IMG_6986" src="https://github.com/user-attachments/assets/a984bd58-ef91-4412-8abd-ba428dd7f60b" />

</p>
  4. ir_line_taskseguidas (totalizando 600 ms), o monitor ativa a variável global `fault_mode = 1` para colocar o robô em rotação segura de emergência.
* **Justificativa de tempo real:** Sendo uma tarefa de supervisão de nível macro, rodar a cada 200 ms consome pouquíssima CPU e é rápido o bastante para impedir colisões se alguma tarefa de controle travar.
