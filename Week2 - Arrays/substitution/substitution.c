#include <cs50.h>
#include <stdio.h>
#include <ctype.h>

int key_checker(string text);

int main(int argc, string argv[])
{
    //alloying only one command-line argument
    if (argc == 2)
    {
        string key = argv[1];
        int i = 0, index = 0;
        const int UPPER_INDEX = 65, LOWER_INDEX = 97;

        //function that validates the key
        int key_valid = key_checker(key);

        //Proceeding only if the key is valid
        if (key_valid == 2)
        {
            //prompting plain text from the user
            string message = get_string("plaintext: ");

            while (message[i] != '\0')
            {
                //Uppercase characters
                if (isupper(message[i]))
                {
                    index = message[i] - UPPER_INDEX;
                    message[i] = toupper(key[index]);
                }
                //Lowercase characters
                else if (islower(message[i]))
                {
                    index = message[i] - LOWER_INDEX;
                    message[i] = tolower(key[index]);
                }
                i++;
            }
            //Printing output
            printf("ciphertext: %s\n", message);
            return 0;
        }
        else
        {
            return 1;
        }

    }
    else
    {
        printf("Error message!, Problems with the Key\n");
        return 1;
    }

}

//function that validates the key
int key_checker(string text)
{
    int i = 0, char_counter = 0, valid = 0, no_repeated[26], repeat_index = 0;
    char letter;
    const int UPPER_INDEX = 65;

    //filling an array of zeros to detect repeated characters
    for (; i < 26 ; i++)
    {
        no_repeated[i] = 0;
    }

    i = 0;

    while (text[i] != '\0' && valid == 0)
    {
        //Only alphabetic characters are counted
        if (isalpha(text[i]))
        {
            //counting characters
            char_counter++;

            //detecting repeated characters
            letter = toupper(text[i]);
            repeat_index = letter - UPPER_INDEX;

            if (no_repeated[repeat_index] == 0)
            {
                no_repeated[repeat_index] = 1;
            }
            else
            {
                printf("Key must not contain repeated characters.\n");
                valid = 1;
            }

        }
        //Print error message and return a invalid signal
        else
        {
            printf("Only alphabetic characters allowed.\n");
            valid = 1;
        }

        i++;
    }

    //The key must contain 26 alphabetic characters
    if (char_counter == 26)
    {
        valid = 2;
    }
    else if (valid == 0)
    {
        printf("Key must contain 26 characters.\n");
        valid = 1;
    }

    return valid;
}