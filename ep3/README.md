# EP3 - Melhorando MM
### Daniela Gonzalez Favero -
### Felipe Castro de Noronha - 10737032

---

Nesse EP modificamos os **memory manager** do MINIX 3. Adicionamos a chamada de sistema `memalloc()` que muda a politica de alocação de memoria entre _first-fit_ e _worst-fit_. Além disso, implementamos o utilitario `memorymap` que exibe o _memory map_.

# 1 Definindo a _syscall_

Como o mecanismo que faz alocação de memoria fica no **process manager**, foi neste servidor que fizemos as modificações para definir a nova _syscall_.

Assim como no EP2, definimos uma nova entrada no arquivo `/usr/src/servers/pm/table.c`, a entrada para `memalloc` fica na posição 66. Adicionamos tambem o prototipo da nova chamada em `/usr/src/servers/pm/proto.h`. Em seguida, no arquivo `/usr/src/servers/pm/alloc.c` foi implementada a função `do_memalloc`, tal função faz a checagem se o processo que invocou a  é _root_, mas sua principal ação sera explicada na proxima sessão.

Para definir uma função que possa ser utilizada como uma biblioteca disponivel para o usuario, modificamos os arquivos /`usr/src/include/minix/callnr.h` e `/usr/include/minix/callnr.h`, definindo MEMALLOC para o valor 66. Na pasta `/usr/src/lib/posix/` adicionamos o arquivo `_memalloc.c`, que sera responsavel por fornecer a interface entre a função de usuario e a syscall.

Ao final disso, bastou realizar alguns ajustes em makefiles e compilar as novas funções da biblioteca, assim como as biblitoecas em si.

# 2 Implementação da nova politica

Primeiramente, para deixar as coisas mais claras, definimos duas novas macros em `user/src/include/stdlib.h`, obtendo `WORST_FIT == 1` e `FIRST_FIT == 0`. Assim, podemos fazer a chamada `memalloc(WORST_FIT)` para mudarmos a politica de alocação de memoria para _worst-fit_.

O arquivo modificado para implentar essa nova politica foi o `/usr/src/servers/pm/alloc.c`. Nele definimos a variavel privada e global `alloc_mode`, ela representa qual a politica de alocação que queremos.

Nesse mesmo arquivo, temos a função `do_memalloc()`, que é responsavel por aplicar a _syscall_. Primeiro, essa função faz a checagem se o _effective uid_ do processo que a invocou é o _root uid_, ou seja, 0. Depois disso ela pega da mensagem o modo que foi passado como argumento, e caso ele seja algum dos modos predefinidos, _seta_ `alloc_mode` de acordo.

Finalmente, a função que realiza a alocação de memoria em si, `alloc_mem`, foi modificada. Fizemos um condicional para diferenciar a politica que deve ser aplicada. O procedimento tomado para aplicar a politica de _worst-fit_ consiste em percorrer toda a lista de _holes_ em busca do buraco com maior _h_len_ (tamanho). Depois disso é checado se o tamanho desse buraco suporta o numero de _clicks_ pedido a função. Em caso positivo, o buraco é diminuido e o começo de seu endereço base é retornado. Caso negativo, tenta-se _swappar_ algum processo da memoria para criar mais espaço.

# 3 Utilitario
