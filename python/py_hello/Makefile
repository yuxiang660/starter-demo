PY = /lan/cva/hsv-apfw/yuxiangw/common/bin/python3.8
TARGET = main.py

.PHONY: run debug

run: $(TARGET)
	$(PY) $(TARGET)

debug: $(TARGET)
	$(PY) -m pdb $(TARGET)

# for gdb debug, use gdb to attach
# 1. get pid: ps -u yuxiangw | grep python3
# 2. attach: gdb -p <PID>
# 3. use py commands in gdb console: "py-bt", "py-print a"

