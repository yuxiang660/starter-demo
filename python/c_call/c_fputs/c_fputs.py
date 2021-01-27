import fputs

def c_fputs():
    print('--- call c_fputs ---')
    val = fputs.fputs("Real Pyhton", "write.log")
    print(f'return {val}')


