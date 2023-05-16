#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // TODO
    // verify the candidate voted by the user with every candidate
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }

    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // TODO

    // Move every row in order to obtain the number of votes
    for (int i = 0; i < voter_count; i++)
    {
        // Only consider the first column
        if (candidates[preferences[i][0]].eliminated == false)
        {
            candidates[preferences[i][0]].votes++;
        }
        // consider the others columns
        else
        {
            for (int j = 1; j < candidate_count; j++)
            {
                // Stop as soon as a non-eliminated candidate is found
                if (candidates[preferences[i][j]].eliminated == false)
                {
                    candidates[preferences[i][j]].votes++;
                    break;
                }
            }
        }
    }

    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // TODO

    int number_votes_winner = 0;

    // find the half of the total numbers of voters
    number_votes_winner = round(voter_count / 2.0);

    // evaluate every candidate whether it is already a winner or not
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes > number_votes_winner)
        {
            printf("%s\n", candidates[i].name);
            return true;
        }

        else if (candidates[i].votes == number_votes_winner)
        {
            return false;
        }

    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // TODO
    int mini = voter_count;

    // find the lowest vote not considering candidates eliminated
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes < mini)
            {
                mini = candidates[i].votes;
            }
        }
    }

    return mini;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // TODO
    int counter_tie = 0, candidate_available = 0;

    // count the number of available candidates and the number of candidates that have the same number of votes
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            candidate_available++;

            if (candidates[i].votes == min)
            {
                counter_tie++;
            }
        }
    }

    // Compare the number of candidates tie and the number of candidates available to determine if there is actually a tie
    if (counter_tie == candidate_available)
    {
        return true;
    }
    else
    {
        return false;
    }

}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    // TODO

    // Check every candidate if it has not been eliminated yet and if it has the same number of votes as the lowest number of votes so it can be eliminated
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes == min)
            {
                candidates[i].eliminated = true;
            }
        }
    }

    return;
}