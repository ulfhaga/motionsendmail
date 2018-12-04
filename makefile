CODE_DIR = src 
TEST_DIR = tests


.PHONY: project_code
project_code:
	$(MAKE) -C $(CODE_DIR)
	$(MAKE) -C $(TEST_DIR)

.PHONY: clean
clean:
	$(MAKE) -C $(CODE_DIR) clean
	$(MAKE) -C $(TEST_DIR) clean
	
.PHONY: indent
indent:
	$(MAKE) -C $(CODE_DIR) indent
	$(MAKE) -C $(TEST_DIR) indent

.PHONY: test
test: 
	$(MAKE) -C $(TEST_DIR) test

