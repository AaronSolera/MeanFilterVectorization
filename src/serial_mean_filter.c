#include "../include/serial_mean_filter.h"

unsigned char *** createMatrix(ALLEGRO_BITMAP *image){
    const int width  = al_get_bitmap_width(image);
    const int height = al_get_bitmap_height(image);
    unsigned char *** ptr_image = allocateMemorySpaceForImage(&height, &width);
    ALLEGRO_COLOR colorpixel =  al_get_pixel(image,0,0);
    unsigned char r,g,b;
    al_unmap_rgb(colorpixel,&r, &g, &b);
    float progress_counter = 0;
    for (int i = height - 1; i >= 0 ; i--)
        for (int j = width - 1; j >= 0 ; j--)
        {
            colorpixel = al_get_pixel(image,j,i);
            al_unmap_rgb(colorpixel,&r, &g, &b);
            ptr_image[i][j][0] = (unsigned char) r;
            ptr_image[i][j][1] = (unsigned char) g;
            ptr_image[i][j][2] = (unsigned char) b;
            // Progress measurement
            printf("\rProgress : %f", (float)(progress_counter / (width * height)) * 100);
            progress_counter++;
        }
    return ptr_image;
}

void deallocateMemorySpaceForImage(int height, int width, unsigned char *** image_matrix){
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            free(image_matrix[i][j]);
        }
    }
}

unsigned char *** allocateMemorySpaceForImage(const int * ptr_height, const  int * ptr_width){ 

    unsigned char *** img = (unsigned char ***)calloc(*ptr_height, sizeof(unsigned char **));

    if (img == NULL) {
        fprintf(stderr, "Out of memory");
        exit(0);
    }

    for (int i = 0; i < *ptr_height; i++)
    {
        img[i] = (unsigned char **)calloc(*ptr_width, sizeof(unsigned char*));

        if (img[i] == NULL)
        {
            fprintf(stderr, "Out of memory");
            exit(0);
        }

        for (int j = 0; j < *ptr_width; j++)
        {
            img[i][j] = (unsigned char *)calloc(3, sizeof(unsigned char));

            if (img[i][j] == NULL)
            {
                fprintf(stderr, "Out of memory");
                exit(0);
            }
        }
    }
    return img;
}

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
uint16x4_t v_a, v_b;
uint16x4_t v16_result;
uint32x2_t v32_result; 
uint64x1_t v64_result;
uint64_t result;

void meanFilterNeon(const int * ptr_height, const int * ptr_width, unsigned char *** input_img, unsigned char *** output_img)
{
    int scaled_filter_size = WINDOW_FILTER_SIZE / 2;
    int neighborhood_size = WINDOW_FILTER_SIZE * WINDOW_FILTER_SIZE;
    float progress_counter = 0;
    uint16_t neighborhood[8];
    //Go throughout the pixels of the image
    for (int i = 0; i < *ptr_height; i++)
    {
        for (int j = 0; j < *ptr_width; j++)
        {
            if ((i >= scaled_filter_size && i <= *ptr_height - scaled_filter_size - 1) &&
                (j >= scaled_filter_size && j <= *ptr_width - scaled_filter_size - 1))
            {
                // Progress measurement
                printf("\rProgress : %f", (float)(progress_counter / ((*ptr_width) * (*ptr_height))) * 100);
                progress_counter++;
                // Computing x and y neighborhood initial position
                int x = i - scaled_filter_size;
                int y = j - scaled_filter_size;
                // Getting matrix neighborhood 
                neighborhood = {input_img[x][y][R_CHANNEL], 
                                input_img[x][y + 1][R_CHANNEL], 
                                input_img[x][y + 2][R_CHANNEL], 
                                input_img[x + 1][y][R_CHANNEL],
                                input_img[x + 1][y + 1][R_CHANNEL], 
                                input_img[x + 1][y + 2][R_CHANNEL], 
                                input_img[x + 2][y][R_CHANNEL], 
                                input_img[x + 2][y + 1][R_CHANNEL]};
                // Get first 8 array elements into an 8 elements vector
                v_a = vld1_u16(neighborhood);
                v_b = vld1_u16(neighborhood + 4);
                // Add every element in vector
                v16_result = vpadd_u16(v_a, v_b);
                v32_result = vpaddl_u16(v16_result);
                v64_result = vpaddl_u32(v32_result);
                result     = vget_lane_u64(v64_result, 0) + input_img[x + 2][y + 2][R_CHANNEL];
                // Devide addition result by neighborhood size (neighborhood size = 9)
                int mean = (int)(((float)result) / 9);
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
}
*/

//----------------------------------------------------------------------------------------------------------------------
// FUNCTIONS FOR SAVE BITMAP IMAGE
//----------------------------------------------------------------------------------------------------------------------
void generateBitmapImage(int height, int width, unsigned char *** image, char * imageFileName){
    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width * COLOR_CHANNELS) % 4) % 4;

    unsigned char* fileHeader = createBitmapFileHeader(height, width, paddingSize);
    unsigned char* infoHeader = createBitmapInfoHeader(height, width);

    FILE* imageFile = fopen(imageFileName, "wb");

    fwrite(fileHeader, 1, fileHeaderSize, imageFile);
    fwrite(infoHeader, 1, infoHeaderSize, imageFile);

    //------------------------------------------------------------------------------------------------------------------
    unsigned char img_temp[height][width][COLOR_CHANNELS];
    for (int k = 0; k < 3; k++)
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                img_temp[i][j][k] = image[i][j][k];
    unsigned char * img = (unsigned char *)img_temp;
    //------------------------------------------------------------------------------------------------------------------

    for(int i = height; i > 0; i--)
    {
        fwrite(img+(i * width * COLOR_CHANNELS), COLOR_CHANNELS, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char * createBitmapFileHeader(int height, int width, int paddingSize){
    int fileSize = fileHeaderSize + infoHeaderSize + (COLOR_CHANNELS * width + paddingSize) * height;

    static unsigned char fileHeader[] = {
            0,0,     /// signature
            0,0,0,0, /// image file size in bytes
            0,0,0,0, /// reserved
            0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize    );
    fileHeader[ 3] = (unsigned char)(fileSize>> 8);
    fileHeader[ 4] = (unsigned char)(fileSize>>16);
    fileHeader[ 5] = (unsigned char)(fileSize>>24);
    fileHeader[10] = (unsigned char)(fileHeaderSize + infoHeaderSize);

    return fileHeader;
}

unsigned char * createBitmapInfoHeader(int height, int width){
    static unsigned char infoHeader[] = {
            0,0,0,0, /// header size
            0,0,0,0, /// image width
            0,0,0,0, /// image height
            0,0,     /// number of color planes
            0,0,     /// bits per pixel
            0,0,0,0, /// compression
            0,0,0,0, /// image size
            0,0,0,0, /// horizontal resolution
            0,0,0,0, /// vertical resolution
            0,0,0,0, /// colors in color table
            0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(infoHeaderSize);
    infoHeader[ 4] = (unsigned char)(width    );
    infoHeader[ 5] = (unsigned char)(width>> 8);
    infoHeader[ 6] = (unsigned char)(width>>16);
    infoHeader[ 7] = (unsigned char)(width>>24);
    infoHeader[ 8] = (unsigned char)(height    );
    infoHeader[ 9] = (unsigned char)(height>> 8);
    infoHeader[10] = (unsigned char)(height>>16);
    infoHeader[11] = (unsigned char)(height>>24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(COLOR_CHANNELS * 8);

    return infoHeader;
}