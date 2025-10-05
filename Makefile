# === Compiler & Flags ===
CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall -Wextra -pthread -Wno-unused-parameter -fsanitize=address -g
LDFLAGS = -Llib -ldaas -lpthread -lbluetooth

# === Paths ===
SRC_DIR = src
MAIN_DIR = main
BIN_DIR = bin
LOG_DIR = logs

# === Common sources ===
SRC_COMMON = $(SRC_DIR)/daas_chat.cpp

# === Targets ===
TARGET_A = $(BIN_DIR)/nodo_a
TARGET_B = $(BIN_DIR)/nodo_b

# === Default build ===
all: prepare $(TARGET_A) $(TARGET_B)

# === Build rules ===
$(TARGET_A): $(MAIN_DIR)/main_a.cpp $(SRC_DIR)/nodo_a.cpp $(SRC_COMMON)
	@echo "[BUILD] $@"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_B): $(MAIN_DIR)/main_b.cpp $(SRC_DIR)/nodo_b.cpp $(SRC_COMMON)
	@echo "[BUILD] $@"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# === Prepare directories ===
prepare:
	@mkdir -p $(BIN_DIR) $(LOG_DIR)

# === Clean ===
clean:
	@echo "[CLEAN]"
	rm -rf $(BIN_DIR) $(LOG_DIR)

# === Debug build ===
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

# === Info helper ===
print-%:
	@echo '$*=$($*)'

# === Run targets with logging ===
run_a: $(TARGET_A)
	@echo "[RUN] nodo_a -> $(LOG_DIR)/nodo_a.log"
	@$(TARGET_A) 2>&1 | tee $(LOG_DIR)/nodo_a.log

run_b: $(TARGET_B)
	@echo "[RUN] nodo_b -> $(LOG_DIR)/nodo_b.log"
	@$(TARGET_B) 2>&1 | tee $(LOG_DIR)/nodo_b.log
