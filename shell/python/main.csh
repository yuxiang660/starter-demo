#!/bin/csh

set PIN1 = "TERM_U_HDESB_1_DATA"
set PIN2 = "TERM_U_HDESB_2_DATA"

./script.py $PIN1

if ($? == 0) then
  echo "\n\nTEST_PASSED"
else
  echo "\n\nTEST_FAILED"
endif
