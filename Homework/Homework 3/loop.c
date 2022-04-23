// CS 33 Homework 3
// Naman Modani
// loop.c

long loop(long x, long n)
{
    long result = 0;
    long mask;
    for (mask = 1; mask != 0; mask = mask << n)
    {
     	result |= (mask & x);
    }
    return result;
}