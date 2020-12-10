#include <arm_neon.h>

uint8x8_t vector;
uint8_t   last_vector_element, result;

int calculateMedianNeon(uint8_t * arr) {
    // Get first 8 array elements into an 8 elements vector
    vector = vld1_u8(arr);
    // Get last array element
    last_vector_element = *(arr + 8);
    // Add every element in vector
    result = vaddv_u8(vector);
    // Add parcial results and devide them by 9
    return (int)(((float)(result + last_vector_element)) / 9);
}

int main(int argc, char const *argv[]) {

	uint8_t array[] = {1,2,3,4,5,6,7,8,9};
	printf("La media es %i\n", calculateMedianNeon(array));
	return 0;
}