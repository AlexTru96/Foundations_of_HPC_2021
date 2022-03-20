#ifndef TOOLS_H
#define TOOLS_H
#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#if !defined(DOUBLE_PRECISION) 
#define float_t float 
#define SCAN_KPOINT fscanf(f,"%f %f",&value_x,&value_y)
#define MPI_MYFLOAT MPI_FLOAT
#else 
#define float_t double
#define SCAN_KPOINT fscanf(f,"%lf %lf",&value_x,&value_y)
#define MPI_MYFLOAT MPI_DOUBLE
#endif 

#define SWAP(x, y) { kpoint temp = x; x = y; y = temp; }  //swapping kdpoints in the array
#define NDIM 2
#define TASK_SIZE 20000 // Task size threshold
#define COUNT 12   //This is just for printing reasons

typedef struct {float_t point[NDIM];} kpoint;

typedef struct kdnode { 
int split_axis; // the splitting dimension
kpoint split_point; // the splitting element
struct kdnode *left, *right; // the left and right sub-trees
} kdnode;

// TOOL FUNCTIONS DECLARATION
int choose_splitting_dimension(int);
size_t choose_splitting_point(kpoint*, size_t ,size_t,int);
int readArray(kpoint*,size_t, FILE*);
void print2DTree(kdnode*,int*,int*,int,int);
int *encoder(int, int );
int *depth_encoder(int, int );
int get_depth(int);
void free_tree(kdnode*);

// SORTING FUNCTIONS DECLARATION
void quickselect(kpoint* , size_t ,size_t, int , size_t);
size_t partial_sort(kpoint* , size_t ,size_t, int , size_t);


// TREE BUILDERS DECLARATION
kdnode* build_kdtree(kpoint*, int , int , int,int*,int);
kdnode* build_master_tree(kpoint*, int , int , int,int);

#endif
