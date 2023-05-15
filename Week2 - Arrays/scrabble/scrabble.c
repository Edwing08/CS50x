#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    int i = 0, score = 0, index;
    const int A = 65, Z = 90, a = 97, z = 122 ;

    //While loop that interates until null or space
    while (word[i] != '\0' && word[i] != ' ')
    {
        //obtaining every letter from the word
        char letter = word[i];

        //ignoring non-letter characters
        if ((letter >= A && letter <= Z) || (letter >= a && letter <= z))
        {
            //obtaining the index of the array "POINTS" through substracting the current letter's value with the first letter's value possible
            if (islower(letter))
            {
                index = letter - a;
            }
            else if (isupper(letter))
            {
                index = letter - A;
            }
            //obtaining the current score with the previous value
            score = score + POINTS[index];
        }
        //move to the next letter of the string
        i++;
    }
    //return the value
    return score;
}
