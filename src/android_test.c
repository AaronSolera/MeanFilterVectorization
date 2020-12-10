#include <stdio.h>
#include <stdlib.h>
#include <arm_neon.h>

uint16_t last_vector_element;
uint16x4_t v_a, v_b;
uint16x4_t v16_result;
uint32x2_t v32_result; 
uint64x1_t v64_result;
uint64_t result;

void print_uint16x4(uint16x4_t vector){
    printf("[%3i ",  vget_lane_u16(vector, 0));
    printf("%3i ",   vget_lane_u16(vector, 1));
    printf("%3i ",   vget_lane_u16(vector, 2));
    printf("%3i]\n",   vget_lane_u16(vector, 3));
}

int calculateMedianNeon(uint16_t * arr) {
    // Get first 8 array elements into an 8 elements vector
    v_a = vld1_u16(arr);
    v_b = vld1_u16(arr + 4);
    // Get last array element
    last_vector_element = *(arr + 8);
    // Add every element in vector
    v16_result = vpadd_u16(v_a, v_b);
    v32_result = vpaddl_u16(v16_result);
    v64_result = vpaddl_u32(v32_result);
    result     = vget_lane_u64(v64_result, 0) + last_vector_element;
    // Add parcial results and devide them by 9
    return (int)(((float)result) / 9);
}

int main(int argc, char const *argv[]) {

    uint16_t array[] = {0,255,134,255,54,255,43,255,78};
    printf("La media es %i\n", calculateMedianNeon(array));
    return 0;
}