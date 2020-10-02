import sys

def foo():
    print("foo")

def bar():
    print("bar")

if __name__ == "__main__":
    print('hello')
    foo()
    a = 1
    print(a)
    c = sys.stdin.read(1)
    print(c)
    bar()
