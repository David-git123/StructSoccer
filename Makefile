# ===================================================
#   Struct Soccer - Makefile
#   Compila e executa o jogo com "make run"
# ===================================================

# Compilador e flags
CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -O2

# Bibliotecas Raylib (para Linux)
LIBS    = -lraylib -lm -lpthread -ldl -lX11 -lGL

# Diretórios e arquivos
SRC_FILES   = main.c
BUILD_DIR   = build
OUT_FILE    = $(BUILD_DIR)/structsoccer

# Criação da pasta build, se não existir
$(shell mkdir -p $(BUILD_DIR))

# Compilação
$(OUT_FILE): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $(OUT_FILE) $(SRC_FILES) $(LIBS)

# Limpeza
clean:
	rm -rf $(BUILD_DIR)

# Execução
run: $(OUT_FILE)
	./$(OUT_FILE)

.PHONY: clean run
