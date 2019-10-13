CC := g++
FLAGS := -std=c++11 -w
TEST_FLAGS := -lgtest_main -lgtest -lpthread
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/main.out: $(BUILD_DIR)/Storage.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/main.o
	@mkdir -p $(BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE) $^ -o $@

# $(BIN_DIR)/test: $(BUILD_DIR)/Storage.o $(BUILD_DIR)/test.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/Meeting.o
# 	@mkdir -p $(BIN_DIR)
# 	$(CC) $(FLAGS) $(INCLUDE) $^ -o $@ $(TEST_FLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)