#include "utils.hpp"
#include <cstdlib>
#include <vector>

// mergeVector()
// merge vector is missing because the compiler must be able to see 
// the implementation of the function in order to generate code 
// for all specializations.

void merge(std::vector<Edge*> &e, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* Copy data to L[] and R[] */
    std::vector<Edge*> L;
    std::vector<Edge*> R;

    for (i = 0; i < n1; i++)
        L[i] = e[l + i];

    for (i = 0; i < n2; i++)
        R[i] = e[m + 1 + i];

    // Merge tmp arrays in e[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        //compare the cost
        if (L[i]->cost() <= R[j]->cost())
        {
            e[k] = L[i];
            i++;
        }
        else
        {
            e[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[] and R[], if there are any
    while (i < n1)
    {
        e[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        e[k] = R[j];
        j++;
        k++;
    }
}


void mergeSort(std::vector<Edge*> &e, int l, int r)
{
    if (l < r)
    {
        // Sort the two half of the array
        int m = (l+r)/2;
        mergeSort(e, l, m);
        mergeSort(e, m+1, r);

        // Merge the two arrays
        merge(e, l, m, r);
    }
}