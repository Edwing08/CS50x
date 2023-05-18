// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 676;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    node *cursor = NULL; // Pointer that goes through every node
    int hash_result = hash(word); // Hash the word

    cursor = table[hash_result];
    // Stop if the cursor reachs an empty node in the linked list
    while (cursor != NULL)
    {
        // Compare the two words
        if (strcasecmp(cursor -> word, word) == 0)
        {
            return true;
        }
        cursor = cursor -> next; // Move the pointer to the next node
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    //return toupper(word[0]) - 'A';

    // Improved hash function, each bucket considers the first two words of the alphabet
    return (((toupper(word[0]) - 'A') * 26) + (toupper(word[0]) - 'A'));
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    int hash_result = 0;
    char word[LENGTH + 1];

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Stop once the end of the file is reached
    while (fscanf(file, "%s", word) != EOF)
    {

        node *n = malloc(sizeof(node)); // Obtain temporary space in memory
        if (n == NULL)
        {
            return 1;
        }

        strcpy(n -> word, word); // Copy the word

        hash_result = hash(word); // Call hash function

        // If the bucket was empty in the current position regarding the hash function
        if (table[hash_result] == NULL)
        {
            n -> next = NULL;
            table[hash_result] = n;
        }
        // If the bucket already had a linked list in the current position regarding the hash function
        else
        {
            n -> next = table[hash_result];
            table[hash_result] = n;
        }
    }

    fclose(file); // Close the file once opened

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    int words_count = 0;
    node *temporal = NULL;

    // Go through every bucket
    for (int i = 0; i < N; i++)
    {
        temporal = table[i];
        // Stop once the temporal cursor has reached the end of the linked list in the bucket
        while (temporal != NULL)
        {
            temporal = temporal -> next;
            words_count++;
        }
    }

    return words_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *tmp = NULL;
    node *cursor = NULL;

    // Go through every bucket
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        tmp = table[i];
        // Stop once the temporal cursor has reached the end of the linked list in the bucket
        while (cursor != NULL)
        {
            cursor = cursor -> next; // the cursor moves first
            free(tmp); // then tmp is freed

            tmp = cursor; // after that the tmp moves to where the cursor is pointing
        }
    }

    return true;
}
