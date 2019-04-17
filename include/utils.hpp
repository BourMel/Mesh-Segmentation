#ifndef UTILS_H
#define UTILS_H

#include <vector>

/**
 * @brief Concatenate two vectors and produce a new one
 * @return The concatenation of A and B
 * @param A First vector
 * @param B Second vector
 */
template <typename T>
std::vector<T> mergeVector(const std::vector<T> &A, const std::vector<T> &B)
{
    std::vector<T> C(A);
    C.insert(C.end(), B.begin(), B.end());

    return C;
}
#endif