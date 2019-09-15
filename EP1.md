# <center> EP1 – MAC422 - Sistemas Operacionais 
<center> Daniela Gonzalez Favero    10277443 <br>
Felipe Castro de Noronha    10737032 </center>
<br>

### 1. Implementando a parte externa da shell
A função ` main() ` tem um laço infinito que recebe o input do usuário. A função `parse_arg()` trata a entrada e repassa o comando e o arquivo para a função ` process()`, que por sua vez chama as funções que executarão as chamadas de sistema.<br>
Fizemos também algumas funções auxiliares como `valid_file()` para analisar o arquivo antes de passá-lo para qualquer comando, `remove_lf()` e `word_count()` para tratamento de texto (inserido na shell).

### 2. Implementando `protegepracaramba()` e `liberageral()`
Implementadas usando a chamada de sistema `chmod()` para mudar a permissão do arquivo passado.

### 3. Implementando `rode()` e `rodeveja()`
Implementadas usando as chamadas de sistema `fork()` para criar um processo filho da shell e `execve()` para executar o arquivo passado. A função `rodeveja()` utiliza também `waitpid()` para aguardar o processo terminar e `WEXITSTATUS()` para retornar a saída do programa após o processo ser encerrado.