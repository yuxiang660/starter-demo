import sys
print('--- begin add so path')
sys.path.insert(0, '/lan/cva/hsv-apfw/yuxiangw/GitHub/starter-demo/python/c_call/lib')
print(sys.path)
print('--- end add so path')
import os
LD_PATH = "/lan/cva_rel/wxe_main/red/tools.lnx86/lib/64bit"
os.environ['LD_LIBRARY_PATH'] = LD_PATH + ':' + os.environ['LD_LIBRARY_PATH']
print(os.environ['LD_LIBRARY_PATH'])

from .c_fputs import c_fputs