#!/bin/bash
# Este es nuestro primer progrma
gcc sobel_image_omp.c -o sobel_image_omps -lm

echo prueba 720
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 1
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 2
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 4
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 8
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 16

echo prueba 1080
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 1
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 2
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 4
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 8
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 16

echo prueba 4k
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 1
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 2
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 4
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 8
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 16