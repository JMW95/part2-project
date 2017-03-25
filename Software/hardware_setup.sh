#!/bin/bash

rmmod pixelstream_vsync
rmmod gpu_workqueue

if [ ! -f "vsync/pixelstream_vsync.ko" ]
then
    cd vsync && make && cd ..
fi

if [ ! -f "workqueue/gpu_workqueue.ko" ]
then
    cd workqueue && make && cd ..
fi

insmod vsync/pixelstream_vsync.ko
insmod workqueue/gpu_workqueue.ko
