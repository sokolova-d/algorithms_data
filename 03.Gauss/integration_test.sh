#!/bin/bash

status=true

./gauss AB.csv > AB_solution.test.csv

if cmp -s AB_solution.csv AB_solution.test.csv; then
  echo "Small CSV solve ok!"
else
  echo "Small CSV solve failed!" >&2
  status=false
fi

./gauss AB.csv AB_solution.file.test.csv

if cmp -s AB_solution.csv AB_solution.file.test.csv; then
  echo "CSV file output ok!"
else
  echo "CSV file output failed!" >&2
  status=false
fi

$status
