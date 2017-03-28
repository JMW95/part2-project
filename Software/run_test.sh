#!/bin/bash

./hardware_setup.sh

pushd triangles
if [ ! -f "software" ]; then
	make software
	mv a.out software
fi

if [ ! -f "hardware" ]; then
	make hardware
	mv a.out hardware
fi
popd

python average.py hardware
python average.py software
