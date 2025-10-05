# === Compiler & Flags ===
CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall -Wextra -pthread -Wno-unused-parameter
LDFLAGS = -Llib -ldaas -lpthread -lbluetooth

# === Sources ===
SRC_COMMON = src/daas_chat.cpp
SRC_DIR = src
MAIN_DIR = main

# === Targets ===
BIN_DIR = bin
TARGET_A = $(BIN_DIR)/nodo_a
TARGET_B = $(BIN_DIR)/nodo_b

# === Default Rule ===
all: prepare $(TARGET_A) $(TARGET_B)

# === Build Rules ===
$(TARGET_A): $(MAIN_DIR)/main_a.cpp $(SRC_DIR)/nodo_a.cpp $(SRC_COMMON)
	@echo "[BUILD] $@"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TARGET_B): $(MAIN_DIR)/main_b.cpp $(SRC_DIR)/nodo_b.cpp $(SRC_COMMON)
	@echo "[BUILD] $@"
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# === Create folders if missing ===
prepare:
	@mkdir -p $(BIN_DIR)

# === Clean ===
clean:
	@echo "[CLEAN]"
	rm -f $(BIN_DIR)/nodo_a $(BIN_DIR)/nodo_b

# === Debug ===
debug: CXXFLAGS += -g -DDEBUG
debug: clean all

# === Info ===
print-%:
	@echo '$*=$($*)'
