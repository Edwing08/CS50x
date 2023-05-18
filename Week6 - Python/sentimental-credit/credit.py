# TODO
from cs50 import get_string

holder = []
counter1 = 0
counter2 = 0

# Get the number in string format
number = get_string("Number: ")

# lenght of the number
n_length = len(number)

# Loop through every number's digit
for i in range(n_length):
    # Process every digit from back to front
    int_number = int(number[n_length-i-1])
    # Sum certain number
    if i % 2 == 0:
        counter1 += int_number
    # Multiply by 2 and then sum the products’ digits
    else:
        multiply = 2*int_number
        # Split the number if it has more than two digits
        if (multiply > 9):
            split_number = int(multiply/10)
            counter2 += split_number

            split_number = int(multiply % 10)
            counter2 += split_number
        else:
            counter2 += multiply

total = counter1 + counter2
beginning_2n = int(number[:2])
beginning_1n = int(number[:1])

# Establish the card's brand based on the results
if (total % 10 == 0):

    if n_length == 15 and beginning_2n in [34, 37]:
        print("AMEX")
    elif n_length == 16 and beginning_2n in [51, 52, 53, 54, 55]:
        print("MASTERCARD")
    elif n_length == 13 or n_length == 16 and beginning_1n == 4:
        print("VISA")
    else:
        print("INVALID")

else:
    print("INVALID")