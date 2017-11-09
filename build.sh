#!/usr/bin/sh

gcc `pkg-config --cflags gtk+-3.0` -o colta-gtk src/main.c -g `pkg-config --libs gtk+-3.0`
