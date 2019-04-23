#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include "edge.hpp"

/**
 * @brief Concatenate two vectors and produce a new one
 * @return The concatenation of A and B
 * @param A First vector
 * @param B Second vector
 * @author Nathan R.
 */
template <typename T>
std::vector<T> mergeVector(const std::vector<T> &A, const std::vector<T> &B)
{
    std::vector<T> C(A);
    C.insert(C.end(), B.begin(), B.end());

    return C;
}

/**
 * @brief Apply bubble sort (ascending) to the vector \c v
 * 
 * @tparam T Must be able to use < or > operator
 * @param v The vector to sort
 * @author Nathan R.
 */
template <typename T>
void bubbleSort(std::vector<T> &v)
{ 
   for(int i = v.size()-1; i>=1 ; i--)
   {
       for(int j = 0; j < i; j++)
       {
           if(v[j+1] < v[j])
           {
               std::swap(v[j+1], v[j]);
           }
       }
   }
}

/**
* \brief sort and merge two subarrays of Edge
*
* \param e : array of Edge
* \param l : left index of the first subarray
* \param m : right index of the first subarray, left index of the second subarray
* \param r : right index of the second subarray
* \author Tiphaine R.
*/
void merge(std::vector<Edge *> &e, int l, int m, int r);

/**
* \brief merge sort of an array of Edge
*
* must copy e before executing the function or it will be overwriten
* initialization : l = 0, r = e.size() -1
*
* \param e : array of Edge
* \param l : left index of the first subarray
* \param r : right index of the second subarray
* \author Tiphaine R.
*/
void mergeSort(std::vector<Edge *> &e, int l, int r);

#endif