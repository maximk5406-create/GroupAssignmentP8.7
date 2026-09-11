#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

const string PLAIN_ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// ============================================================================
// Author: Phil Tapes (Team Lead)
// Purpose: Constructs the 26-letter substitution cipher array from a keyword.
// ============================================================================
string generateCipherAlphabet(const string& keyword)
{
    string cipherKey = "";

    // Step 1: Remove duplicates from keyword and convert to uppercase
    for (char ch : keyword)
    {
        if (isalpha(ch))
        {
            char upperCh = toupper(ch);
            if (cipherKey.find(upperCh) == string::npos)
            {
                cipherKey += upperCh;
            }
        }
    }

    // Step 2: Append remaining alphabet letters in reverse order
    for (int i = 25; i >= 0; i--)
    {
        char ch = PLAIN_ALPHABET[i];
        if (cipherKey.find(ch) == string::npos)
        {
            cipherKey += ch;
        }
    }

    return cipherKey;
}

// ============================================================================
// Author: Member 2
// Purpose: Reads input stream character-by-character and writes encrypted/decrypted output.
// ============================================================================
void processFile(ifstream& inFile, ofstream& outFile, const string& cipherAlphabet, bool isEncrypt)
{
    char ch;
    while (inFile.get(ch))
    {
        if (isalpha(ch))
        {
            bool isLower = islower(ch);
            char upperCh = toupper(ch);
            char substitutedCh;

            if (isEncrypt)
            {
                size_t index = PLAIN_ALPHABET.find(upperCh);
                substitutedCh = cipherAlphabet[index];
            }
            else
            {
                size_t index = cipherAlphabet.find(upperCh);
                substitutedCh = PLAIN_ALPHABET[index];
            }

            if (isLower)
            {
                substitutedCh = tolower(substitutedCh);
            }
            outFile.put(substitutedCh);
        }
        else
        {
            // Preserve whitespace and non-alphabetic formatting
            outFile.put(ch);
        }
    }
}

// ============================================================================
// Author: Omar Llerena
// Purpose: Parses command-line arguments and validates encryption/decryption flags.
// ============================================================================
int main(int argc, char* argv[])
{
    if (argc < 5)
    {
        cerr << "Usage: crypt <-e|-d> -k<KEYWORD> <input_file> <output_file>" << endl;
        cerr << "Example: crypt -d -kFEATHER encrypt.txt output.txt" << endl;
        return 1;
    }

    bool isEncrypt = true;
    string keyword = "";
    string inputFile = "";
    string outputFile = "";

    // Parse flags
    for (int i = 1; i < argc; i++)
    {
        string arg = argv[i];
        if (arg == "-e")
        {
            isEncrypt = true;
        }
        else if (arg == "-d")
        {
            isEncrypt = false;
        }
        else if (arg.substr(0, 2) == "-k")
        {
            keyword = arg.substr(2);
        }
        else if (inputFile.empty())
        {
            inputFile = arg;
        }
        else if (outputFile.empty())
        {
            outputFile = arg;
        }
    }

    if (keyword.empty())
    {
        cerr << "Error: Keyword required! Use flag -kKEYWORD" << endl;
        return 1;
    }

    string cipherAlphabet = generateCipherAlphabet(keyword);

    cout << "Using Keyword        : " << keyword << endl;
    cout << "Generated Cipher Key : " << cipherAlphabet << endl;
    cout << "Mode                 : " << (isEncrypt ? "Encrypting" : "Decrypting") << endl;

    ifstream inFile(inputFile);
    if (!inFile.is_open())
    {
        cerr << "Error: Cannot open input file: " << inputFile << endl;
        return 1;
    }

    ofstream outFile(outputFile);
    if (!outFile.is_open())
    {
        cerr << "Error: Cannot open output file: " << outputFile << endl;
        return 1;
    }

    processFile(inFile, outFile, cipherAlphabet, isEncrypt);

    cout << "Operation completed successfully. Output written to " << outputFile << endl;

    inFile.close();
    outFile.close();

    return 0;
}
