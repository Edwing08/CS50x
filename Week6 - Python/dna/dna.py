import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    database = sys.argv[1]
    database_list = []
    with open(database) as sequence_file:
        dna_database = csv.DictReader(sequence_file)

        for rows in dna_database:
            database_list.append(rows)

    # TODO: Read DNA sequence file into a variable
    dna = sys.argv[2]
    dna = open(dna, 'r')
    dna_sequence = dna.read()

    # TODO: Find longest match of each STR in DNA sequence
    STR_dict = {}
    candidate_match = 0
    for key in database_list[0].keys():
        # Skip the key name
        if key != 'name':
            # Finde the longest match regarding the STR and save the result in a dictionary
            result = longest_match(dna_sequence, key)
            STR_dict[key] = str(result)

    # TODO: Check database for matching profiles
    # Check each candidate in the database
    for candidate in database_list:
        # Bool variable that keeps track that every STR count match in number
        sequence = True
        # Compare each longest STR in DNA sequence with each candidate in the database
        for STR in STR_dict:
            if (STR_dict[STR] == candidate[STR] and sequence == True):
                pass
            # Set the bool variable to false and break the loop if there is at least one mismatch
            else:
                sequence = False
                break

        # Print name of candidate that matched every STR count with the sequence
        if sequence == True:
            candidate_match += 1
            print(candidate['name'])

    if candidate_match == 0:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
