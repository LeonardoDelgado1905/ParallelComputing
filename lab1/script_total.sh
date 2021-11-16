#!/bin/bash
# Este es nuestro primer progrma
gcc sobel_image.c -o sobel_image -lm

echo prueba 720
./sobel_image ./720p/castle.jpg ./720p/castle_bordes.jpg

echo prueba 1080
./sobel_image ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg

echo prueba 4k
./sobel_image ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg