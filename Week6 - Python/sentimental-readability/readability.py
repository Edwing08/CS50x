# TODO
from cs50 import get_string

# Prompt the user for text
text = get_string("Text: ")

# Obtain the number of characters in the text
length = len(text)

# Initialize counters
n_letters = 0
n_words = 0
n_sentences = 0

# Go through every character in the text
for i in range(length):
    letter = text[i]
    # Determine the number of letters
    if letter.isalpha():
        n_letters += 1
    else:
        if n_letters > 1:
            # Determine the number of words
            if letter == " ":
                n_words += 1
            # Determine the number of sentences
            elif letter in ['.', '?', '!'] and text[i-1].isalpha():
                n_sentences += 1

# Consider the last word if there is more than one word
if n_words > 1:
    n_words += 1

# Calculate Coleman-Liau index
words = n_words/100
L = n_letters/words
S = n_sentences/words

index = (0.0588 * L) - (0.296 * S) - 15.8

# Sort the index with the grade
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade", round(index))

