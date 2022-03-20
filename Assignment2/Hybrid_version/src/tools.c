#include "tools.h"

int choose_splitting_dimension(int axis){
	return (axis+1)%NDIM;
}

size_t choose_splitting_point(kpoint* array_points, size_t n_points, size_t start, int myaxis){

	size_t median_idx = n_points / 2 ;	
	quickselect(array_points, start, start+n_points-1, myaxis , start + median_idx);
	return start + median_idx;

}

int readArray(kpoint array[], size_t size, FILE* f){
	float_t value_x,value_y;
        size_t counter=0;
	while(counter<size){ /* LOOP UNTIL REACH THE SIZE OF THE LIST */
		if(SCAN_KPOINT){ /* SCAN THE LINE AND JUMPS TO THE NEXT*/
			array[counter].point[0]=value_x;
			array[counter].point[1]=value_y;  //Just for the 2D dimension
	 		counter++;}
		else{
			printf("ERROR WHILE READING");
			return 1;
		}
	}
	return 0;
}


void print2DTree(kdnode *root, int* flags,int* depth_flags,int depth, int space)
{
	if (root == NULL){
		printf("\n");
		return;}
 
	// Increase distance between levels
	space += COUNT;
	int depth_flag;
	int flag;
	// Implementation using flag array to avoid printing certain branches

	if(depth){
		flag= flags[depth-1];
		depth_flag= depth_flags[depth-1];
	}
	if(depth_flag&&flag&&depth){
		depth--;
		print2DTree(root->left,flags,depth_flags,depth,space);
		return;}

	if(depth_flag&&!flag&&depth){
		depth--;
		print2DTree(root->right,flags,depth_flags,depth,space);
		return;}
	// Process right
	print2DTree(root->right, 0,0,0,space); 
	printf("\n");

	for (int i = COUNT; i < space; i++) printf(" ");

	printf("(%.1lf %.1lf) ",root->split_point.point[0],root->split_point.point[1]);
 
    	// Process left
    	print2DTree(root->left, 0,0,0,space);
}
 

void free_tree(kdnode* binary_tree){
	// frees the entire binary tree until it finds NULL
	if(binary_tree){
		kdnode* aux= binary_tree->right;
		free_tree(binary_tree->left);
		free(binary_tree);
		free_tree(aux);
	}
	return;
}

int *encoder(int n, int bitswanted){
	 // returns an array filled with the n argument as binary digits
	int *bits = malloc(sizeof(int) * bitswanted);
  	for(int k=0; k<bitswanted; k++){
    		int mask =  1 << k;
    		int masked_n = n & mask;
    		int thebit = masked_n >> k;
    		bits[k] = thebit;}
  	return bits;
}


int *depth_encoder(int rank, int depth){
	// Tool to obtain the flags to only print the partial tree corresponding a certain process
	int *depth_flags = calloc(depth,sizeof(int));
  	int k;

  	for(k=0; k<depth; k++) depth_flags[k] = 1;
	
	if(rank==0){
  		for(k=0; k<depth; k++) depth_flags[k] = 0;
		return depth_flags;}

  	if((rank%2)==1)	return depth_flags;
  	
  	int power_2 = 1;
  	k=0;
	for(int i=0; i<depth-1; i++){
    		power_2*=2;
    		if(rank%power_2==0) depth_flags[k] = 0;
    		k++;
  	}
	return depth_flags;
}

int get_depth(int size){
 	// just a log in base 2
	int depth=0;
	if(size){
		while(size!=1){
			size/=2;
			depth++;}
	}
	return depth;
}
