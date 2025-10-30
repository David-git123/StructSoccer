# ===================================================
#   Struct Soccer - Makefile
#   Compila e executa o jogo com "make run"
# ===================================================

# Compilador e flags
CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -O2 -Iinclude

# Bibliotecas Raylib (Linux)
LIBS    = -lraylib -lm -lpthread -ldl -lX11 -lGL
# (Se quiser macOS:
# LIBS  = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo)

# Diretórios
SRC_DIR   = src
BUILD_DIR = build
OBJ_DIR   = $(BUILD_DIR)/obj

# Arquivos (todos os .c dentro de src/, inclusive subpastas)
SRC_FILES = $(shell find $(SRC_DIR) -name "*.c")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
OUT_FILE  = $(BUILD_DIR)/structsoccer

# Alvo padrão
all: $(OUT_FILE)

# Link final
$(OUT_FILE): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Regra de compilação de cada .c -> .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -rf $(BUILD_DIR)

# Execução
run: $(OUT_FILE)
	./$(OUT_FILE)

.PHONY: all clean run
