# <center> EP1 – MAC422 - Sistemas Operacionais 
<center> Daniela Gonzalez Favero    10277443 <br>
Felipe Castro de Noronha    10737032 </center>
<br>

### 1. Implementando a parte externa da shell
A função ` main() ` tem um laço infinito que recebe o input do usuário. A função `parse_arg() ` trata a entrada e repassa o comando e o arquivo para a função ` process() `, que por sua vez chama as funções que executarão as chamadas de sistema.

### 2. Implementando `protegepracaramba()` e `liberageral()`
Implementadas usando a chamada de sistema `chmod()`.

### 3. Implementando `rode()` e `rodeveja()`
Implementadas usando as chamadas de sistema `execve()` e `fork()`. A função `rodeveja()` utiliza também `waitpid()`.