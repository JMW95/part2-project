#!/bin/bash

./hardware_setup.sh

pushd triangles
make
popd

python average.py hardware
python average.py software
