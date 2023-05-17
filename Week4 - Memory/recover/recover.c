#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>
#include <string.h>

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{

    typedef uint8_t BYTE;

    // Check commant-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw \n");
        return 1;
    }

    // Open the file to read
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open the file \n");
        return 1;
    }

    BYTE buffer[512]; // Buffer of 512 bytes
    int img_count = 0; // Number of images found counter

    string filename = "###.jpg"; // Image title model
    char *point = malloc(strlen(filename) + 1); // Pointer to keep track of the images' titles
    FILE *p_output = NULL; // File pointer to store temporaly the file opened

    while (fread(buffer, 1, BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // Check the first four bytes to find the beginning of an image
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // Create a file and write in it the first buffer
            if (img_count == 0)
            {
                // Open and write a new file
                sprintf(point, "%03i.jpg", img_count);

                FILE *output = fopen(point, "w");
                p_output = output;
                if (output == NULL)
                {
                    printf("Could not open file. \n");
                    return 1;
                }
                fwrite(buffer, 1, BLOCK_SIZE, output);
                img_count++; // Keep track of the number of images found
            }
            else
            {
                // Close the previous file
                fclose(p_output);

                // Open a new file and write in it
                sprintf(point, "%03i.jpg", img_count);

                FILE *output = fopen(point, "w");
                p_output = output;
                if (output == NULL)
                {
                    printf("Could not open file. \n");
                    return 1;
                }
                fwrite(buffer, 1, BLOCK_SIZE, p_output);
                img_count++; // Keep track of the number of images found
            }
        }
        // If it is not the beginning of an image, keep writing the file.
        else
        {
            if (img_count > 0)
            {
                fwrite(buffer, 1, BLOCK_SIZE, p_output);
            }
        }
    }
    free(point); // Free memory allocated using malloc
    fclose(p_output); // Close the last file
}