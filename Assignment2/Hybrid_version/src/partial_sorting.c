#include "tools.h"

// partial sorting to allocate numbers greater to the right and lesser to the left of pivot
size_t partial_sort(kpoint* array, size_t start, size_t end, int axis, size_t pIndex)
{
    double pivot = array[pIndex].point[axis];
 
    // Move pivot to end
    SWAP(array[pIndex], array[end]);
    pIndex = start;

    // each time we find an element less than or equal to the pivot, pIndex
    // is incremented, and that element would be placed before the pivot.

    for (size_t i = start; i < end; i++)
    {
        if (array[i].point[axis] <= pivot)
        {
            SWAP(array[i], array[pIndex]);
            pIndex++;
        }
    }
 
    // move pivot to its final place
    SWAP(array[pIndex], array[end]);
 
    // return pIndex
    return pIndex;
}
 
// Returns the median according to the start and end index of the KDpoint array
void quickselect(kpoint* array, size_t start,size_t end, int axis, size_t median)
{
    // If the array contains only one element, return that element
    if (start==end) {
        return;
    }

    // select random pIndex between start and end
    size_t pIndex = start+ rand() % (end-start+1);
 
    pIndex = partial_sort(array, start, end, axis, pIndex);
 
    // The pivot is in its final sorted position
    if (median == pIndex) {
        return ;
    }
 
    else if (median < pIndex) {
        return quickselect(array, start, pIndex -1, axis, median);
    }

    else {
        return quickselect(array, pIndex+1 , end, axis, median);
    }
}

