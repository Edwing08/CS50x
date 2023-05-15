#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text:");
    float index, L, S, words;
    int grade;

    int num_letters = count_letters(text);
    int num_words = count_words(text);
    int num_sentences = count_sentences(text);

    //Coleman-Liau index operations
    words = 100.0 / num_words;

    L = num_letters * words;
    S = num_sentences * words;

    index = (0.0588 * L) - (0.296 * S) - 15.8;

    //rounding the index
    grade = round(index);

    //Conditionals regarding the grade level's classification
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}

//function that counts letters
int count_letters(string text)
{
    int i = 0, counter = 0;

    //Only alphabetical characters are counted and ends when there are not characters left
    while (text[i] != '\0')
    {
        if (isalpha(text[i]))
        {
            counter++;
        }
        i++;
    }

    return counter;
}

//function that counts words
int count_words(string text)
{
    int i = 0, counter = 0;

    //Counting spaces and the last null space in order to determine the number of words
    do
    {
        if (isspace(text[i]))
        {
            counter++;
        }
        if (text[i + 1] == '\0')
        {
            counter++;
        }
        i++;
    }
    while (text[i] != '\0');

    return counter;
}

//function that counts sentences
int count_sentences(string text)
{
    int i = 0, counter = 0;

    //Counting special characters after alphabetical characters in order to determine the number of sentences
    while (text[i] != '\0')
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            if (isalpha(text[i - 1]))
            {
                counter++;
            }
        }
        i++;
    }

    return counter;
}