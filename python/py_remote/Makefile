SHELL := /bin/csh

.PHONY: run

run:
	@source setup.csh && /lan/cva/hsv-apfw/yuxiangw/common/bin/python3 ./main.py

debug:
	@source setup.csh && /lan/cva/hsv-apfw/yuxiangw/common/bin/python3 -m debugpy --listen sjcvl-yuxiangw:5487 --wait-for-client ./main.py
