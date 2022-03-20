#include "tools.h"

kdnode* build_master_tree(kpoint *points, int start, int end, int axis, int depth)
/*
 * Similar as build_kdtree but the parallelization strategy is by creating omp tasks on the recursion calls
 * Master tree is built by every MPI processor
*/
{
	int N= end - start+ 1;

	if(!depth){
   		return NULL;}

  	//allocate the memory for a new node
	kdnode *node = (kdnode*) malloc(sizeof(kdnode));

	//implementation to obtain the median as the array is sorted using mergesort and the axis using round robin fashion	
	int separation_index = choose_splitting_point( points, N, start , axis);
	axis = choose_splitting_dimension(axis);

	//storing data in the knode
	node->split_axis= axis;
	node->split_point= points[separation_index];

	// Using recursion to build the left and right branches while there are enough points 
	if(depth){
		depth--;
		#pragma omp task default(none) mergeable \
		shared(points,node) firstprivate(start,separation_index,axis,depth) untied 
		node->left = build_master_tree(points, start, separation_index-1, axis, depth);
		
		#pragma omp task default(none) mergeable \
		shared(points,node) firstprivate(end,separation_index,axis,depth) untied 
		node->right = build_master_tree(points, separation_index+1, end, axis, depth);
	
	#pragma omp taskwait
	}
	return node;
}

kdnode* build_kdtree(kpoint *points, int start, int end, int axis, int* flags, int depth)

/*
 * points is a pointer to the relevant section of the data set;
 * start and end are the indexes of points to be considered, from points+start to points+end
 * axis is the dimension used previsously as the splitting one
 * flags is the pointer to the flag array which will contain the binary information to omit certain branches ( 0 = left, 1 =right)
 * the flags will allow the parallel work of this KDTree construction
 * depth is the integer which indicates the flag array until what level of the tree should be omitting certain branches
 * aux_array is the auxiliary array needed for the merge sorting, this is for performance reasons, it is much better to deal with only one malloc 
*/
{
	int N= end - start+ 1;

	if( N == 2 ){
   		kdnode *node = (kdnode*) malloc(sizeof(kdnode));
   		kdnode *node_D = (kdnode*) malloc(sizeof(kdnode));
   		node->left= node_D; node->right= NULL;
   		node_D->right= NULL; node_D->left= NULL;

		node->split_point = *(points+start+1);	 node_D->split_point= *(points+start);
   		return node;}	//return a leaf with the point *points;

	if( N == 1 ){
   		kdnode *node = (kdnode*) malloc(sizeof(kdnode));
   		node->left= NULL; node->right= NULL;

   		node->split_point = *(points+start);
   		return node;}

  	//allocate the memory for a new node
	kdnode *node = (kdnode*) malloc(sizeof(kdnode));


	//implementation to obtain the median as the array is sorted using mergesort and the axis using round robin fashion
	int separation_index = choose_splitting_point(points, N, start , axis);
	axis = choose_splitting_dimension(axis);

	//storing data in the knode
	node->split_axis= axis;
	node->split_point= points[separation_index];

	// implementation to omit the construction of certain branches in the kdtree by using a flag array 
	int flag;

	if(depth) flag= flags[depth-1];

	if(depth&&flag){
		depth--;
		node->right=NULL;
		node->left = build_kdtree( points, start,separation_index-1, axis,flags,depth);
		return node;
	}

	if(depth&&!flag){
		depth--;
		node->left=NULL;
		node->right = build_kdtree( points, separation_index+1,end, axis,flags,depth);
		return node;
	}

	// Using recursion to build the left and right branches while there are enough points 
	node->left = build_kdtree(points, start, separation_index-1, axis, 0, 0);
	node->right = build_kdtree(points, separation_index+1, end, axis, 0, 0);
	return node;
}

