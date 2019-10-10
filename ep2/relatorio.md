# Relatorio

**Sepa que o do_nice não foi recompilado e a mudança que eu fiz la nem esta acontecendo de vdd**

Seria bom deixar essa tabela no Relatorio

# Arquivos modificados

Arquivo | Mudança
:--- | :----
`/usr/src/kernel/proc.h` | Nesse arquivo definimos o novo macro para a fila `BATCH`
`/usr/src/kernel/proc.c` | Aqui foi onde mundamos o escalonador. Basicamente fizemos um tratamento totalmente diferente para processos que estão com a prioridade definida como `BATCH`
`/usr/src/kernel/system/do_nice.c`| Uma excessão foi adicionada para o caso em que a *system task* é invocada para mudar a prioridade de um processo para `BATCH_Q`
`/usr/src/servers/pm/table.c` | Adicionamos novas entradas no vetor, que representam as novas chamadas de sistama `do_batch` e `do_unbatch`
`/usr/src/servers/pm/proto.h` | Prototipamos as novas chamadas
`/usr/src/servers/pm/misc.c` | Fizemos aqui a definição das chamadas, ou seja, o codigo/maquinario das duas funções fica nesse arquivo
`/usr/src/include/minix/callnr.h` | Definimos as macros `BATCH` e `UNBATCH`, a posição das novas chamadas no vetor
`/usr/src/lib/posix/_batch.c` | Arquivo que vai realizar a *system call* relacionada á `BATCH`. Essa função que é chamada quando um programa de usuario invoca `batch(pid)`
`/usr/src/lib/posix/_unbatch.c` | Arquivo que vai realizar a *system call* relacionada á `UNBATCH`. Essa função que é chamada quando um programa de usuario invoca `unatch(pid)`
