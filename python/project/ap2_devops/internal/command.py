import logging
import os
import psutil
import subprocess
import time


class CShell:

    def __init__(self, cmd, log_file=None):
        self._cmd = cmd
        self._log_file = log_file
        self._start = time.time()

        start_log = f">>> Start command '{self._cmd}' <<<"
        logging.debug(start_log)
        if self._log_file is None:
            self._proc = subprocess.Popen(self._cmd,
                                          shell=True,
                                          executable='/bin/csh',
                                          universal_newlines=True)
        else:
            assert os.path.exists(os.path.dirname(log_file))
            with open(self._log_file, 'a') as f:
                f.write(start_log + '\n')
                self._proc = subprocess.Popen(self._cmd,
                                              shell=True,
                                              executable='/bin/csh',
                                              universal_newlines=True,
                                              stdout=f,
                                              stderr=f)

    def communicate(self, timeout_s=None):
        elapsed = None
        try:
            self._proc.communicate(None, timeout_s)
        except subprocess.TimeoutExpired as e:
            self.kill()
            log = f"!!! End command '{self._cmd}' with timeout error: {e.__str__()} !!!"
            logging.error(log)
        else:
            elapsed = time.time() - self._start
            log = ">>> End command '{}' in {:.3f} seconds <<<".format(self._cmd, elapsed)
            logging.debug(log)
        if self._log_file:
            with open(self._log_file, 'a') as f:
                f.write(log + '\n\n')
        return elapsed

    def kill(self):
        assert self._proc.pid != os.getpid(), "won't kill myself"

        parent = psutil.Process(self._proc.pid)
        processes = parent.children(recursive=True)
        processes.append(parent)

        logs = []
        for p in processes:
            log = f">>> Kill command '{self._cmd}' process '{p.pid}' - '{p.name()}' <<<"
            logs.append(log)
            logging.debug(log)
            p.kill()

        _, alive = psutil.wait_procs(processes)
        assert len(alive) == 0, "all processes should be killed"

        if self._log_file:
            with open(self._log_file, 'a') as f:
                for log in logs:
                    f.write(log + '\n')
