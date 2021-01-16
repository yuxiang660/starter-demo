PY := /lan/cva/hsv-apfw/yuxiangw/common/bin/python3
PIP := /lan/cva/hsv-apfw/yuxiangw/common/bin/pip3

# Unit test targets in ./tests folder, such as: internal.command, tester.setup
TARGETS := $(shell find ./tests -name "*_tests.py" | sed --expression='s|/|.|g;s|\.\.tests\.||g;s|_tests.py||g')


tests:
	@$(PY) -m unittest discover -p '*_tests.py' -v

$(TARGETS):
	@$(PY) -m tests.$@_tests -v

build:
	@$(PY) setup.py build

install_deps:
	@$(PIP) install -r requirements.txt

install:
	@$(PIP) install .

uninstall:
	@$(PIP) install ap2_devops

clean:
	@rm -rf dist build MANIFEST output

.PHONY: $(TARGETS) tests clean install install_deps build
