python
import sys
sys.path.insert(0, '/lan/cva/hsv-apfw/yuxiangw/opt/gdb/stlprettyprinter')
from libstdcxx.v6.printers import register_libstdcxx_printers
register_libstdcxx_printers (None)
end
