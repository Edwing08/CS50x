# TODO
from cs50 import get_int

gap = 2

# Prompt the height until the input is valid, between 1 and 8
while(True):
    Height_number = get_int("Height: ")

    if Height_number >= 1 and Height_number <= 8:
        break

# Loop acording to the pyramid's height
for i in range(Height_number):
    # Loop that prints the hashes
    for j in range(0, Height_number + gap + i + 1, 1):
        if j + i >= Height_number - 1 and not(j <= Height_number + gap - 1 and j >= Height_number):
            print("#", end="")
        else:
            print(" ", end="")
    print("")