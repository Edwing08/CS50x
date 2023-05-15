#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //Variables
    int length = 0, oddCounter = 0, evenCounter = 0, totalSum, split;
    long LastDigit, digitCount = 1, fstDigit, secDigit;

    //Prompt for input - card number
    long credit = get_long("Number:");

    while (credit >= digitCount)
    {
        //Counter to keep track of the current digit position and the total length
        length++;

        //Operation to detect the last digit
        LastDigit = credit / digitCount;
        LastDigit = LastDigit % 10;

        //every other digit from the number’s second-to-last digit
        if (length % 2 == 0)
        {
            if (LastDigit * 2 > 9)
            {
                split = (LastDigit * 2) / 10 + (LastDigit * 2) % 10;
                evenCounter = evenCounter + split;
            }
            else
            {
                evenCounter = evenCounter + LastDigit * 2;
            }
        }
        //every other digit from the number’s last digit
        else
        {
            oddCounter = oddCounter + LastDigit;
        }
        //counter to track the number of digits and the loop's condition
        digitCount = digitCount * 10;
    }

    //checksum
    totalSum = evenCounter + oddCounter;
    if (totalSum % 10 == 0)
    {
        //obtaining first digit
        fstDigit = (credit * 10) / (digitCount);
        //obtaining first two digits
        secDigit = (credit * 100) / (digitCount);

        //Visa uses 13 - 16 digits and starts with 4
        if (length >= 13 && length <= 16 && fstDigit == 4)
        {
            printf("VISA\n");
        }
        //Mastercard uses 16 digits and starts with 51,52,53,54 or 55
        else if (length == 16 && (secDigit == 51 || secDigit == 52 || secDigit == 53 || secDigit == 54 || secDigit == 55))
        {
            printf("MASTERCARD\n");
        }
        //American Express uses 15 digits and starts with 34 or 37
        else if (length == 15 && (secDigit == 34 || secDigit == 37))
        {
            printf("AMEX\n");
        }
        //Invalid
        else
        {
            printf("INVALID\n");
        }
    }
    //Invalid
    else
    {
        printf("INVALID\n");
    }
}