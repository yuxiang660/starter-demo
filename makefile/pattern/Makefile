TESTS := cnt32 mockD hdsb
TESTS_COMPILE := $(addsuffix _compile, $(TESTS))
TESTS_RUN := $(addsuffix _run, $(TESTS))

target:
	@echo "compiling $@"

$(TESTS):
	@echo "running $@ config - config.$@.json"

$(TESTS_COMPILE):
	@echo "running $@ config - config.$(patsubst %_compile,%,$@).json"

$(TESTS_RUN):
	@echo "running $@ config - config.$(patsubst %_run,%,$@).json"

.PHONY: $(TESTS) $(TESTS_COMPILE) $(TESTS_RUN)
