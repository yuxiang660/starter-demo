class CShell:

    def __init__(self, cmd):
        self._cmd = cmd

    def communicate(self):
        print(f'Command "{self._cmd}" done')
