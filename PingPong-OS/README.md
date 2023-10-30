# Passos para a execução do Trabalho
## Replit
1. Abra o local da pasta do trabalho.
   - No Replit: `cd PingPong-OS`
2. Rode o seguinte comando no sistema, de acordo com o compilador de linguagem C:
   - No Replit: `gcc -o ppos ppos-core-aux.c pingpong-scheduler-srtf.c libppos_static.a -O0`
3. Em seguida, rode o executável:
   - `./ppos`
4. Ou, utilizando o makefile use: `make pingpong`
   - Em seguida `./pingpong`

## Ambiente Linux Padrão

1. Utilize um sistema operacional Linux.
2. Abra o prompt de comando.
3. Abra o local da pasta do trabalho.
5. Rode o seguinte comando no sistema, de acordo com o compilador de linguagem C:
   - No GCC: `gcc -o ppos ppos-core-aux.c pingpong-scheduler-srtf.c libppos_static.a`
6. Em seguida, rode o executável:
   - `./ppos`