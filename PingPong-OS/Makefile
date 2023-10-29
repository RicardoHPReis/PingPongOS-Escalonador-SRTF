# Nome do executável
TARGET = pingpong

# Compilador
CC = gcc

# Opções de compilação
CFLAGS = -O0

# Bibliotecas e arquivos fonte
LIBS = libppos_static.a
SRCS = ppos-core-aux.c pingpong-scheduler-srtf.c

# Objetos gerados a partir dos arquivos de origem
OBJS = $(SRCS:.c=.o)

# Regra padrão para criar o alvo (executável)
all: $(TARGET)

# Regra para criar o executável a partir dos objetos e bibliotecas
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

# Regra para criar os objetos a partir dos arquivos de origem
.c.o:
	$(CC) $(CFLAGS) -c $<

# Limpar arquivos intermediários e o executável
clean:
	rm -f $(OBJS) $(TARGET)

# Regra .PHONY para evitar conflitos com arquivos com nomes iguais
.PHONY: all clean