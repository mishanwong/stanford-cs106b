/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

/* The divisorSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division using the modulus operator %
 *
 * Note: the C++ long type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* The isPerfect function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* The findPerfects function takes one argument `stop` and performs
 * an exhaustive search for perfect numbers over the range 1 to `stop`.
 * Each perfect number found is printed to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* The smarterSum function takes one argument `n` and calculates the
 * sum of proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to square root of n.
 */
long smarterSum(long n) {
    long total = 0;
    if (n == 1) {
        return 0;
    }
    for (long divisor = 1; divisor <= floor(sqrt(n)); divisor++) {
        if (n % divisor == 0) {
             total += divisor;
             if (n / divisor != divisor && divisor != 1) {
                 total += (n / divisor);
             }
        }
    }
    return total;
}

/* isPerfectSmarter function takes a number `n` and returns a boolean
 * to indicate whether it is a perfect number or not
 */
bool isPerfectSmarter(long n) {
    return (n != 0) && (n == smarterSum(n));
}

/* findPerfectsSmarter does the same thing as findPerfects but
 * using smarterSum instead of divisorSum
 */
void findPerfectsSmarter(long stop) {
     for (long num = 1; num < stop; num++) {
       if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function finds the nth perfect number using Euclid's method
 */
long findNthPerfectEuclid(long n) {
    int k = 1;
    int count = 0;
    long perfectNumber = 1;
    while (count < n) {
        long m = pow(2, k) - 1;
        if (smarterSum(m) == 1 && m !=1) {
            count += 1;
            perfectNumber = pow(2, k-1) * m;
        }
        k += 1;
    }
    return perfectNumber;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trial of findPerfects on input size 1000") {
    TIME_OPERATION(1000, findPerfects(1000));
}

STUDENT_TEST("Time trials of findPerfects on various input size") {
    int smallest = 10000, largest = 100000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfects(size));
    }
}

STUDENT_TEST("Time trials of findPerfectsSmarter on various input size") {
    int smallest = 40000, largest = 320000;

    for (int size = smallest; size <= largest; size *= 2) {
        TIME_OPERATION(size, findPerfectsSmarter(size));
    }
}

STUDENT_TEST("Check that isPerfect handles negative input") {
    EXPECT(!isPerfect(-10));
}

STUDENT_TEST("Check that 0 and 1 are not perfect") {
    EXPECT(!isPerfectSmarter(0));
    EXPECT(!isPerfectSmarter(1));
}
STUDENT_TEST("Check that smarterSum return the same value as divisorSum") {
    EXPECT_EQUAL(divisorSum(12), smarterSum(12));
    EXPECT_EQUAL(divisorSum(25), smarterSum(25));
    EXPECT_EQUAL(divisorSum(0), smarterSum(0));
    EXPECT_EQUAL(divisorSum(1), smarterSum(1));
    EXPECT_EQUAL(divisorSum(-10), smarterSum(-10));
}

STUDENT_TEST("Check that findNthPerfectEuclid returns a perfect number") {
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(3)));
}
STUDENT_TEST("Check that findNthPerfectEuclid returns the 5th perfect number") {
    EXPECT_EQUAL(findNthPerfectEuclid(5), 33550336);
}
