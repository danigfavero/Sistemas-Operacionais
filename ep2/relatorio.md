# Relatorio

oi, eu sou um relatorio.

Aqui tem as coisas que temos que fazer e as referencias que são uteis pra fazer as coisas

- [ ] Adicionar nova prioridade ao MINIX, criar macro BATCH_Q
    - [ ] Linha 87 de `kernel/system/proc.h`

- [ ] Adicionar a chamada de sistema batch(PID)
    - [ ] Achar uma posição nao usada em `servers/pm/table.c` e colocar o nome da função ali
    - [ ] Prototipar a função em `servers/pm/prot.h`
    - [ ] Escrever a função em `servers/pm/misc.c`
    - [ ] Usar a chamada `dequeue()` de `kernel/system/proc.h`
    - [ ] Vamos (talver) usar uma versão modificada de `enqueue()` de `kernel/system/proc.h`
    - [ ] A partir daqui é mais sobre ler a referencia sobre system call que é SuXeXo

- [ ] Adicionar a chamada de sistema unbatch(PID)
    - [ ] Achar uma posição nao usada em `servers/pm/table.c` e colocar o nome da função ali
    - [ ] Prototipar a função em `servers/pm/prot.h`
    - [ ] Escrever a função em `servers/pm/misc.c`
    - [ ] Usar a chamada `dequeue()` de `kernel/system/proc.h`
    - [ ] Usar a chamada `enqueue()` de `kernel/system/proc.h`
    - [ ] Ir para `/servers` e fazer `make image && make install`
    - [ ] A partir daqui é mais sobre ler a referencia sobre system call que é SuXeXo

- [ ] Alterar o algoritmo de escalonamento
    - [ ] Alterar `kernel/proc.c`

- [ ] Pensar nos testes e implementa-los

- [ ] Completar relatorio

Adicionar system call:
http://www.cis.syr.edu/~wedu/seed/Documentation/Minix3/How_to_add_system_call.pdf

A sessão 3 daqui é muito boa:
http://www.minix3.org/docs/scheduling/report.pdf

A sessão 2.6.10 tambem é muit boa
