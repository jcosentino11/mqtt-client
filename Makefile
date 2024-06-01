
BUILD_DIR = build

.PHONY: all
all:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. -DFLAG_BUILD_TESTS=OFF
	@$(MAKE) -C $(BUILD_DIR)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: run
run: all
	$(BUILD_DIR)/mqtt

.PHONY: test
test:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. -DFLAG_BUILD_TESTS=ON
	@$(MAKE) -C $(BUILD_DIR)
	@cd build/test && ctest
