#!/bin/csh -f

echo $?
echo $?A2SVN_ROOT
if (! $?A2SVN_ROOT) then
  echo 'A2SVN_ROOT not setted'
  echo $_
  set rpath=`echo $_ | awk '{print $2'}`
  echo $rpath
  if ( -l test ) then
    echo 'remove test.txt'
    rm -f test
  endif
  echo 'end'
else
  echo 'A2SVN_ROOT setted'
endif
