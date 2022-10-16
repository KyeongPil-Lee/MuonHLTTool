#!/bin/bash
echo "Will run:"
echo "ROOT macro:   $1"
echo "Sample type:  $2"
echo "Split number: $3"

cmd="root -l -b -q '$1++(\"$2\", \"$3\")'"
echo "will run: $cmd"
eval $cmd

echo "done"