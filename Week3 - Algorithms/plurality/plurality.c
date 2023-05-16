#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        //Compare the input to the candidate's name using the function strcmp
        if (strcmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }

    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // TODO
    int highest = 0, count = 0, highest_spot = 0, tie[candidate_count];

    for (int i = 0; i < candidate_count ; i++)
    {
        //Compare each candidate's vote and set a new highest value if the number is higher
        if (candidates[i].votes > highest)
        {
            highest = candidates[i].votes;
            highest_spot = i; //save the location in the array, if a new highest value is found
            count = 0;
        }

        //In case there are more than two candidates that have same number of votes and are the highest, fill an array with the location of those candidates.
        if (candidates[i].votes == highest)
        {
            tie[count] = i;
            count++;
        }
    }

    //Print output as there is only one winner
    if (count == 0)
    {
        printf("%s\n", candidates[highest_spot].name);
    }
    //Print output as there are more than one winner
    else
    {
        for (int j = 0; j < count; j++)
        {
            printf("%s\n", candidates[tie[j]].name);
        }
    }

    return;
}