#!/bin/bash
# Este es nuestro primer progrma
gcc sobel_image_omp.c -o sobel_image_omp -lm -fopenmp

echo prueba 720 >> times.txt
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 1 >> times.txt
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 2 >> times.txt
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 4 >> times.txt
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 6 >> times.txt
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 8 >> times.txt
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 12 >> times.txt
./sobel_image_omp ./720p/castle.jpg ./720p/castle_bordes.jpg 16 >> times.txt

echo prueba 1080 >> times.txt
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 1 >> times.txt
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 2 >> times.txt
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 4 >> times.txt
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 6 >> times.txt
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 8 >> times.txt
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 12 >> times.txt
./sobel_image_omp ./1080p/dolphin.jpg ./1080p/dolphin_bordes.jpg 16 >> times.txt

echo prueba 4k >> times.txt
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 1 >> times.txt
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 2 >> times.txt
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 4 >> times.txt
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 6 >> times.txt
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 8 >> times.txt
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 12 >> times.txt
./sobel_image_omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_bordes.jpg 16 >> times.txt

echo Ejecucion finalizada
