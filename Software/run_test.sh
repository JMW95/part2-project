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

./hardware
./software

popd
