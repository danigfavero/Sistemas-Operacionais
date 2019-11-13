# EP3 - Melhorando MM
### Daniela Gonzalez Favero - 10277443
### Felipe Castro de Noronha - 10737032

---

Nesse EP modificamos os **memory manager** do MINIX 3. Adicionamos a chamada de sistema `memalloc()` que muda a política de alocação de memória entre _first-fit_ e _worst-fit_. Além disso, implementamos o utilitário `memorymap` que exibe o _memory map_.

# 1 Definindo as _syscalls_

Como o mecanismo que faz alocação de memória e a implementação do memory map ficam no **process manager**, foi neste servidor que fizemos as modificações para definir as duas novas _syscalls_.

Assim como no EP2, definimos duas novas entrada no arquivo `/usr/src/servers/pm/table.c`, a entrada para `memorymap` fica na posição 64 e para `memalloc` na 66. Adicionamos também os protótipos das nova chamadas em `/usr/src/servers/pm/proto.h`. Em seguida, no arquivo `/usr/src/servers/pm/alloc.c` foram implementadas as funções:
- `do_memalloc` , tal função faz a checagem se o processo que invocou a  é _root_, (sua principal ação será explicada na próxima sessão);
- `memory_map`, que imprime o mapa da memória.

Para definir funções que possam ser utilizadas como uma biblioteca disponível para o usuário, modificamos os arquivos `/usr/src/include/minix/callnr.h` e `/usr/include/minix/callnr.h`, definindo MEMORYMAP para o valor 64 e MEMALLOC para 66. Na pasta `/usr/src/lib/posix/` adicionamos tanto o arquivo `_memalloc.c`, quanto o `_memorymap.c`, que serão responsáveis por fornecer a interface entre a função de usuário e a syscall.

Ao final disso, bastou realizar alguns ajustes em makefiles e compilar as novas funções da biblioteca, assim como as bibliotecas em si.vim

# 2 Implementação da nova política

Primeiramente, para deixar as coisas mais claras, definimos duas novas macros em `/usr/src/include/stdlib.h`, obtendo `WORST_FIT == 1` e `FIRST_FIT == 0`. Assim, podemos fazer a chamada `memalloc(WORST_FIT)` para mudarmos a política de alocação de memória para _worst-fit_.

O arquivo modificado para implementar essa nova política foi o `/usr/src/servers/pm/alloc.c`. Nele definimos a variável privada e global `alloc_mode`, ela representa qual a política de alocação que queremos.

Nesse mesmo arquivo, temos a função `do_memalloc()`, que é responsável por aplicar a _syscall_. Primeiro, essa função faz a checagem se o _effective uid_ do processo que a invocou é o _root uid_, ou seja, 0. Depois disso ela pega da mensagem o modo que foi passado como argumento, e caso ele seja algum dos modos predefinidos, _seta_ `alloc_mode` de acordo.

Finalmente, a função que realiza a alocação de memória em si, `alloc_mem`, foi modificada. Fizemos um condicional para diferenciar a política que deve ser aplicada. O procedimento tomado para aplicar a politica de _worst-fit_ consiste em percorrer toda a lista de _holes_ em busca do buraco com maior _h_len_ (tamanho). Depois disso é checado se o tamanho desse buraco suporta o número de _clicks_ pedido à função. Em caso positivo, o buraco é diminuido e o começo de seu endereço base é retornado. Caso negativo, tenta-se _swappar_ algum processo da memória para criar mais espaço.

# 3 Utilitário

O utilitário _memorymap_ saída padrão (o terminal do QEMU) um “mapa” da memória. Este mapa consiste em uma tabela com cabeçalho “pid start end”, além de imprimir no final a quantidade total de memória livre. 
A implementação do utilitário se encontra em `/usr/src/servers/pm/alloc.c` e é bem simples. Primeiro checamos se a chamada de sistema para conseguir os processos foi bem sucedida. Depois, percorremos a lista de *structs pid_t* obtendo seus respectivos _pid_, _start_ e _end_. Imprimimos conforme a formatação pedida. Por fim, percorremos a lista de _memory holes_, incrementando dois contadores que guardam a memória livre (em _clicks_ e _bytes_) até então. Por fim imprimimos a *Available memory*.

# 4 Teste

Há um arquivo `teste.c` em `/home`. Compile-o e execute para ver a saída (que aparecerá apenas no terminal do QEMU) do memory map conforme modificamos a políticas de alocação de memória.