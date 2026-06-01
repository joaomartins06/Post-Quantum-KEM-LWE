#include "modular.h"

int64_t mod_add(int64_t a, int64_t b, int32_t q){
    int64_t sum = a + b;
    sum =sum % q;
    //if negative, add q to get the positive equivalent
    if (sum < 0) sum += q; 
    return sum;
}


void mod_matmul(int64_t *out, const int64_t *A, const int64_t *B,
                int32_t rows, int32_t inner, int32_t cols, int32_t q){

    for (int32_t i = 0; i < rows; i++) {
        for (int32_t j = 0; j < cols; j++){
            //C[i][j] = sum_k A[i][k] * B[k][j]
            int64_t sum = 0;
            for (int32_t k = 0; k < inner; k++) {
                sum += (*(A+i*inner+k)) * (*(B+k*cols+j));
            }
            //C[i][j] = sum % q
            *(out+i*cols+j) = sum % q;
            //again, if negative, add q to get the positive equivalent
            if (*(out+i*cols+j) < 0) *(out+i*cols+j) += q; 
        }
    }
}

