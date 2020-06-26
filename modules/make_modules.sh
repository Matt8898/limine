#!/bin/sh

for i in $(ls -d */ | sort | sed 's|/||g'); do
    make -C $i MODNAME=$i clean
	make -C $i MODNAME=$i
done
