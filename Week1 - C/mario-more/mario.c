#include <cs50.h>
#include <stdio.h>

int main(void)
{

    int height, i, j;
    const int LOWERLIMIT = 1, UPPERLIMIT = 8, GAP = 2;

    do
    {
        //prompt the height of the pyramid
        height = get_int("Height");
    }

    //check whether the input is between the range
    while (height < LOWERLIMIT || height > UPPERLIMIT);

    //Loop for Columns
    for (i = 0; i < height; i++)
    {
        //Loop for left side of the pyramid
        for (j = 0; j < height; j++)
        {
            if (j >= height - i - 1)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        //Loop for the gap of the pyramid
        for (j = 0; j < GAP; j++)
        {
            printf(" ");
        }
        //Loop for right side of the pyramid
        for (j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        //new line
        printf("\n");
    }

}