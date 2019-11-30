# <center> EP4 – MAC422 - Sistemas Operacionais

<center> Daniela Gonzalez Favero    10277443 <br>
Felipe Castro de Noronha    10737032 </center>
<br>

Nesse exercicio programa, mechemos no sistema de arquivos (FS) do Minix para adicionar **arquivos temporarios**, isto é, arquivos que são removidos automaticamente quando o processo que os criou termina. Os passos da implementação e decisões de projeto são aprensentados nos topicos a seguir.

Todas as sessões modificadas em arquivos estão entre os sinalizadores `/* EP4 ####...####*/`.

# 1. Um novo macro, para um novo i-node

Sabemos que no Minix, arquivos são implementados usado a ideia de _i-nodes_. Logo, temos difentes rotulos de i-nodes sendo utilizados na estrutura interna do sistema, como, por exemplo, um rotulo especifico para um diretorio. Para o nosso arquivo temporario, vamos definir um novo tipo **I_TEMPORARY** e que será representado pela _flag_ I_ **0030000**.

O arquivo em que este _define_ foi criado é o `/usr/s/include/minix/const.h`.

# 2. Nova syscall open_tmp()

Para a criação dessa nova _syscall_ definimos uma nova entrada na tabela de chamadas do FS. Logo, modificamos o arquivo `/usr/src/servers/fs/table.c`, adicionando o rotulo `do_open_tmp` na posição 70. Após isso, adicionamos o prototipo da função aplicada pela syscall no arquivo `/usr/src/servers/fs/proto.h`. A implementação dessa função foi realizada no arquivo `/usr/src/servers/fs/open.c`.

Para criar a syscall propriamente dita (uma função _bontinnha_ que fica amostra para o usuario) fizemos o seguinte: criamos um _define_ que mapeia `OPEN_TMP` para o valor 70 (posição no vetor de funções do FS) nos arquivos `/usr/src/include/minix/callnr.h` e `/usr/include/minix/callnr.h`; Em seguida, implementamos o arquivo `/usr/src/lib/posix/_open_tmp.c`, que é responsavel por receber a syscall e efetua-la no FS, passando os argumentos recebidos, em uma mensagem. Após isso, bastou recompilar os servidores e bibliotecas e as chamadas estavam prontas.

# 3. Funcionamento das chamadas

**COLOCAR AQUI DETALHES SOBRE ESSA IMPLEMENTAÇÃO, CASO ELA SEJA FEITA**
**PROVALMENTE TEM QUE ALTERAR O /posix/_OPEN_TMP.C E O /fs/OPEN.C PARA QUE O ARQUVIO SEJA CRIADO**
