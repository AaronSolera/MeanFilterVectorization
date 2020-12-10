void medianFilterNeon(const int * ptr_height, const int * ptr_width, unsigned char *** input_img, unsigned char *** output_img)
{
    int scaled_filter_size = WINDOW_FILTER_SIZE / 2;
    int neighborhood_size = WINDOW_FILTER_SIZE * WINDOW_FILTER_SIZE;
    unsigned char * neighborhood = calloc(neighborhood_size, sizeof(unsigned char));
    float progress_counter = 0;
    //Go throughout the pixels of the image
    for (int i = 0; i < *ptr_height; i++)
        for (int j = 0; j < *ptr_width; j++)
        {
            // Progress measurement
            printf("\rProgress : %f", (float)(progress_counter / ((*ptr_width) * (*ptr_height))) * 100);
            progress_counter++;
            if ((i >= scaled_filter_size && i <= *ptr_height - scaled_filter_size - 1) &&
                (j >= scaled_filter_size && j <= *ptr_width - scaled_filter_size - 1))
            {
                //Go throughout the neighborhood of the current pixel
                for (int x = i - scaled_filter_size, nx = 0; nx < WINDOW_FILTER_SIZE; x++, nx++)
                    for (int y = j - scaled_filter_size, ny = 0; ny < WINDOW_FILTER_SIZE; y++, ny++)
                        //Add the pixel to the current neighborhood
                        *(neighborhood + nx * WINDOW_FILTER_SIZE + ny) = input_img[x][y][R_CHANNEL];

                int median = calculateMedian(neighborhood, neighborhood_size);

                //Set the median value of the neighborhood of the current pixel in RGB channels
                output_img[i][j][R_CHANNEL] = median;
                output_img[i][j][G_CHANNEL] = median;
                output_img[i][j][B_CHANNEL] = median;
            }
            //Keep border pixels value unchanged
            else
            {
                output_img[i][j][R_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][G_CHANNEL] = input_img[i][j][R_CHANNEL];
                output_img[i][j][B_CHANNEL] = input_img[i][j][R_CHANNEL];
            }
        }

    free(neighborhood);
}

int calculateMedianNeon(unsigned char * arr, int length){
    //Sort the elements of the array
    for (int i = 0; i < length; i++)
        for (int j = i + 1; j < length; j++)
        {
            if (*(arr + j) < *(arr + i))
            {
                int temp = *(arr + i);
                *(arr + i) = *(arr + j);
                *(arr + j) = temp;
            }
        }

    return *(arr + (length / 2));
}