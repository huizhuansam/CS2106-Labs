#!/bin/bash

function func {
  count=1
  echo Called with $# parameters.
  for i in $@; do
    echo Parameter $count is $i
    let count=count+1
  done
  return 55;
}

func hello world 13.5
echo $?
