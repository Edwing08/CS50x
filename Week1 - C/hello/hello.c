#include <stdio.h>
#include <cs50.h>

int main(void)
{
    //ask the user to input their name
    string name = get_string("What is your name?\n");
    //print in terminal the name of the user
    printf("hello, %s\n", name);
}
