#!/bin/bash

mpicc sobel_mpi-omp.c -o sobel_mpi-omp -lm -fopenmp



mpirun -np 2 sobel_mpi-omp ./720p/castle.jpg ./720p/castle_sobel.jpg 4
mpirun -np 2 sobel_mpi-omp ./1080p/dolphin.jpg ./1080p/dolphin_sobel.jpg 4
mpirun -np 2 sobel_mpi-omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_sobel.jpg 4

mpirun -np 2 sobel_mpi-omp ./720p/castle.jpg ./720p/castle_sobel.jpg 2
mpirun -np 2 sobel_mpi-omp ./1080p/dolphin.jpg ./1080p/dolphin_sobel.jpg 2
mpirun -np 2 sobel_mpi-omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_sobel.jpg 2


mpirun -np 4 sobel_mpi-omp ./720p/castle.jpg ./720p/castle_sobel.jpg 4
mpirun -np 4 sobel_mpi-omp ./1080p/dolphin.jpg ./1080p/dolphin_sobel.jpg 4
mpirun -np 4 sobel_mpi-omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_sobel.jpg 4

mpirun -np 4 sobel_mpi-omp ./720p/castle.jpg ./720p/castle_sobel.jpg 2
mpirun -np 4 sobel_mpi-omp ./1080p/dolphin.jpg ./1080p/dolphin_sobel.jpg 2
mpirun -np 4 sobel_mpi-omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_sobel.jpg 2


mpirun -np 6 sobel_mpi-omp ./720p/castle.jpg ./720p/castle_sobel.jpg 2
mpirun -np 6 sobel_mpi-omp ./1080p/dolphin.jpg ./1080p/dolphin_sobel.jpg 2
mpirun -np 6 sobel_mpi-omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_sobel.jpg 2


mpirun -np 6 sobel_mpi-omp ./720p/castle.jpg ./720p/castle_sobel.jpg 4
mpirun -np 6 sobel_mpi-omp ./1080p/dolphin.jpg ./1080p/dolphin_sobel.jpg 4
mpirun -np 6 sobel_mpi-omp ./4k/ciudad_4k.jpg ./4k/ciudad_4k_sobel.jpg 4