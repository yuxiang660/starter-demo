#!/bin/csh -f

setenv BASE_CMD base_cmd
setenv CMD cmd

alias do_base_cmd 'echo $BASE_CMD'
alias do_cmd 'echo $CMD; echo "custom cmd start"; echo \!*; echo custom cmd stop'
alias do_cmd_two 'echo \!:1; echo aaa; echo \!:2'

alias do_all_cmd 'do_base_cmd; do_cmd'

