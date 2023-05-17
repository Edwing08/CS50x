#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    int mean;

    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // Go through each column
        for (int j = 0; j < width; j++)
        {
            // Calculate the average between each color channel from the current pixel
            mean = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Assign the average to each color channel in the current pixel
            image[i][j].rgbtRed = mean;
            image[i][j].rgbtGreen = mean;
            image[i][j].rgbtBlue = mean;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    int sepiaRed = 0, sepiaGreen = 0, sepiaBlue = 0;

    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // Go through each column
        for (int j = 0; j < width; j++)
        {
            // Modify the current pixel from the red channel according to the sepia´s formula
            sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            // Modify the current pixel from the green channel according to the sepia´s formula
            sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            // Modify the current pixel from the blue channel according to the sepia´s formula
            sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    int *temp_red = malloc(sizeof(int)), *temp_green = malloc(sizeof(int)), *temp_blue = malloc(sizeof(int));
    int half_width = 0;

    // Determine whether the number of pixels across the width are even or odd
    if (width % 2 == 0)
    {
        half_width = width / 2;
    }
    else
    {
        half_width = (width - 1) / 2; // skip the pixel from the middle
    }

    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // Go through each column until the middle
        for (int j = 0; j < half_width; j++)
        {
            // Store temporally some values from the first half of the image
            *temp_red = image[i][j].rgbtRed;
            *temp_green = image[i][j].rgbtGreen;
            *temp_blue = image[i][j].rgbtBlue;

            // Swap the pixel values from left to right
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;

            // Swap the pixel values from right to left
            image[i][width - j - 1].rgbtRed = *temp_red;
            image[i][width - j - 1].rgbtGreen = *temp_green;
            image[i][width - j - 1].rgbtBlue = *temp_blue;
        }
    }

    // Free the memory used to store some values
    free(temp_red);
    free(temp_green);
    free(temp_blue);

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE copy[height][width];

    // Copy the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    int window_x = 0, window_y = 0, mean_numerator_red = 0, mean_numerator_green = 0, mean_numerator_blue = 0;
    int mean_red = 0, mean_green = 0, mean_blue = 0;
    float mean_denominator = 0.0;

    // Go through each row
    for (int i = 0; i < height; i++)
    {
        // Go through each column
        for (int j = 0; j < width; j++)
        {
            // Go through a 3X3 window in order to obtain the neighbour pixel values around the current pixel in i and j
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    window_x = i + x;
                    window_y = j + y;

                    // Add the pixel values and keep track of the number of pixel, skip the pixels outside the image
                    if ((window_x >= 0 && window_x < height) && (window_y >= 0 && window_y < width))
                    {
                        mean_denominator++;
                        mean_numerator_red += copy[window_x][window_y].rgbtRed;
                        mean_numerator_green += copy[window_x][window_y].rgbtGreen;
                        mean_numerator_blue += copy[window_x][window_y].rgbtBlue;
                    }
                }
            }

            // Calculate the average value
            mean_red = round(mean_numerator_red / mean_denominator);
            mean_green = round(mean_numerator_green / mean_denominator);
            mean_blue = round(mean_numerator_blue / mean_denominator);

            image[i][j].rgbtRed = mean_red;
            image[i][j].rgbtGreen = mean_green;
            image[i][j].rgbtBlue = mean_blue;

            // Reset the counters
            mean_denominator = 0;
            mean_numerator_red = 0;
            mean_numerator_green = 0;
            mean_numerator_blue = 0;
        }
    }
    return;
}
