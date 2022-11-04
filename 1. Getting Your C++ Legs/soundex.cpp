#include <cctype>
#include <fstream>
#include <string>
#include "console.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include "vector.h"
#include "SimpleTest.h" // IWYU pragma: keep (needed to quiet spurious warning)
using namespace std;

string lettersOnly(string s) {
    string result;
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}

/* This function encode a single character to it's corresponding Soundex digit */
string encodeLetter(string c) {
    Vector<string> encoding = {"aeiouhwy", "bfpv", "cgjkqsxz", "dt", "l", "mn", "r"};
    string result;
    for (int i = 0; i < encoding.size(); i++) {
        if (stringContains(encoding[i], toLowerCase(c))) {
            result = integerToString(i);
        }
    }
    return result;
}

string encodeSurname(string surname) {
    string result ="";
    for (int i = 0; i < surname.length(); i++) {
        result += encodeLetter(charToString(surname[i]));
    }
    return result;
}

string coaleseDuplicateDigits(string digits) {
    string result;
    digits += "!"; //Add a non-digit character in the end to ensure last digit gets compared
    int i = 0;
    int j = 1;
    while (j < digits.length()) {
        if (digits[i] == digits[j]) {
            j++;
        } else {
            result += digits[i];
            i = j;
            j++;
        }
    }
    return result;
}

string replaceFirstDigit(string surname, string digits) {
    string result;
    result += toUpperCase(surname[0]);
    for (int i = 1; i < digits.length(); i++) {
        result += digits[i];
    }
    return result;

}

string discardZeros(string code) {
    string result;
    for (int i = 0; i < code.length(); i++) {
        if (charToString(code[i]) != "0") {
            result.append(charToString(code[i]));
        }
    }
    return result;
}

string padWithZeros(string code) {
    string result;

    if (code.length() < 4) {
        result += code;
        while (result.length() < 4) {
           result += "0";
        }
    } else if (code.length() > 4) {
        for (int i = 0; i < 4; i++) {
            result += code[i];
        }
    } else {
        result = code;
    }
    return result;
}

/* This function takes in a string s and return the phonetic encoding
 * using the Soundex algorithm
 */
string soundex(string s) {
    string result;
    result = encodeSurname(s);
    result = coaleseDuplicateDigits(result);
    result = replaceFirstDigit(s, result);
    result = discardZeros(result);
    result = padWithZeros(result);
    return result;
}


/* This function prompt user to enter a surname
 * and return a sorted list of surnames that have the same Soundex encoding.
 */
void soundexSearch(string filepath) {

    ifstream in;
    Vector<string> allNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, allNames);
    }
    cout << "Read file " << filepath << ", "
         << allNames.size() << " names found." << endl;


    while (true) {
        string line = getLine("Enter a surname (RETURN to quit): ");
        if (line == "") {
            cout << "All done!" << endl;
            break;
        }

        Vector<string> searchResults;

        string soundexCode = soundex(line);
        cout << "Soundex code is " + soundexCode << endl;

        for (int i = 0; i < allNames.size(); i++) {
            if (soundexCode == soundex(allNames[i])) {
                searchResults.add(allNames[i]);
            }
        }
        searchResults.sort();
        cout << "Matches from database: " + searchResults.toString() << endl;
    }
}



/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test exclude of punctuation, digits, and spaces") {
    string s = "O'Hara";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

STUDENT_TEST("Test lettersOnly for punctuations, digits and spaces") {
    string s = "Mary-Kate";
    string result = lettersOnly(s);
    EXPECT_EQUAL(result, "MaryKate");

    s = "9West";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "West");

    s = "  McDonalds  ";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "McDonalds");

    s = " &* A # n gel 01 ou ";
    result = lettersOnly(s);
    EXPECT_EQUAL(result, "Angelou");
}

STUDENT_TEST("Test encodeLetter") {
    EXPECT_EQUAL(encodeLetter("C"), "2");
    EXPECT_EQUAL(encodeLetter("r"), "6");
}

STUDENT_TEST("Test encodeSurname") {
    EXPECT_EQUAL(encodeSurname("Curie"), "20600");
    EXPECT_EQUAL(encodeSurname("Angelou"), "0520400");
    EXPECT_EQUAL(encodeSurname("Wong"), "0052");
}

STUDENT_TEST("Test coaleseDuplicateDigits") {
    EXPECT_EQUAL(coaleseDuplicateDigits("20600"), "2060");
    EXPECT_EQUAL(coaleseDuplicateDigits("0520400"), "052040");
    EXPECT_EQUAL(coaleseDuplicateDigits("0052"), "052");
    EXPECT_EQUAL(coaleseDuplicateDigits("0205506"), "020506");
}

STUDENT_TEST("Test replaceFirstDigit") {
    EXPECT_EQUAL(replaceFirstDigit("Curie", "2060"), "C060");
    EXPECT_EQUAL(replaceFirstDigit("Angelou", "052040"), "A52040");
    EXPECT_EQUAL(replaceFirstDigit("O'Conner", "020506"), "O20506");
    EXPECT_EQUAL(replaceFirstDigit("Wong", "052"), "W52");
}

STUDENT_TEST("Test discardZeros") {
    EXPECT_EQUAL(discardZeros("C060"), "C6");
    EXPECT_EQUAL(discardZeros("A52040"), "A524");
    EXPECT_EQUAL(discardZeros("O20506"), "O256");
}

STUDENT_TEST("Test padWithZeros") {
    EXPECT_EQUAL(padWithZeros("C6"), "C600");
    EXPECT_EQUAL(padWithZeros("W52"), "W520");
    EXPECT_EQUAL(padWithZeros("A524"), "A524");
    EXPECT_EQUAL(padWithZeros("C456789"), "C456");
}
