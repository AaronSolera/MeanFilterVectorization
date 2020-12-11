void meanFilter(const int * ptr_height, const int * ptr_width, unsigned char *** input_img, unsigned char *** output_img)
{
    int scaled_filter_size = WINDOW_FILTER_SIZE / 2;
    int neighborhood_size = WINDOW_FILTER_SIZE * WINDOW_FILTER_SIZE;
    double sum;
    float progress_counter = 0;
    //Go throughout the pixels of the image
    for (int i = 0; i < *ptr_height; i++)
        for (int j = 0; j < *ptr_width; j++)
        {
            if ((i >= scaled_filter_size && i <= *ptr_height - scaled_filter_size - 1) &&
                (j >= scaled_filter_size && j <= *ptr_width - scaled_filter_size - 1))
            {
                // Progress measurement
                printf("\rProgress : %f", (float)(progress_counter / ((*ptr_width) * (*ptr_height))) * 100);
                progress_counter++;
                sum = 0;
                //Go throughout the neighborhood of the current pixel
                for (int x = i - scaled_filter_size, nx = 0; nx < WINDOW_FILTER_SIZE; x++, nx++)
                    for (int y = j - scaled_filter_size, ny = 0; ny < WINDOW_FILTER_SIZE; y++, ny++)
                        //Add the pixel to the current neighborhood
                        sum += input_img[x][y][R_CHANNEL];

                int mean = (int)round(sum / neighborhood_size);

                //Set the median value of the neighborhood of the current pixel in RGB channels
                output_img[i][j][R_CHANNEL] = mean;
                output_img[i][j][G_CHANNEL] = mean;
                output_img[i][j][B_CHANNEL] = mean;
            }
            //Keep border pixels value unchanged
            else
            {
                output_img[i][j][R_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][G_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][B_CHANNEL] = input_img[i][j][R_CHANNEL];
            }
        }
}

/*
uint16_t last_vector_element;
uint16x4_t v_a, v_b;
uint16x4_t v16_result;
uint32x2_t v32_result; 
uint64x1_t v64_result;
uint64_t result;

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
    // Devide addition result by neighborhood size (neighborhood size = 9)
    return (int)(((float)result) / 9);
}
*/