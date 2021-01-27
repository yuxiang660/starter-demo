all: so

so:
	gcc -fPIC -shared -o fputs.so -I/lan/cva/hsv-apfw/yuxiangw/common/include/python3.8 fputsmodule.c

build:
	python3 setup.py build

clean:
	@rm -rf *.so build