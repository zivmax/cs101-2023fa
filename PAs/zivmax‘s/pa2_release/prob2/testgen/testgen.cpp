#include "bst.hpp" // Include the header file where your SetOperations class is defined
#include <fstream>
#include <iostream>
#include <random>
#include <set>

// Function to generate a random integer within a given range
long long generateRandomNumber(long long min, long long max)
{
    static std::mt19937_64 rng(std::random_device{}());
    std::uniform_int_distribution<long long> dist(min, max);
    return dist(rng);
}

int main()
{
    std::ofstream outFile("D:/CS_learning/Courses/SIST/CS101/HWs/CS101PAs/pa2_release/prob2/testgen/test_cases.txt");
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open the file for writing test cases." << std::endl;
        return -1;
    }

    init(1);

    // Define the number of test cases and the range for random numbers
    const int numTestCases = 30;
    const long long minVal = 1;
    const long long maxVal = 100; // 10^11

    // Container to store inserted values
    std::vector<long long> insertedValues;

    for (int i = 0; i < numTestCases * .7; ++i)
    {
        long long x;
        size_t opt = generateRandomNumber(1, 2);
        if (opt == 1)
        {
            x = generateRandomNumber(minVal, maxVal);
        }
        else
        {
            if (insertedValues.empty())
            {
                // If no values have been inserted yet, skip the erase operation
                continue;
            }

            // Pick a random value from the inserted values to erase
            auto it = insertedValues.begin();
            std::advance(it, generateRandomNumber(0, insertedValues.size() - 1));
            x = *it;
            // Erase the value from the set
            insertedValues.erase(it);
        }

        // If we are inserting, add the value to the set
        if (opt == 1)
        {
            insertedValues.push_back(x);
        }

        // Perform the operation and get the result
        long long result = SetOperation(opt, x);

        // Write the operation and the result to the file
        outFile << "Operation: " << opt << ", Value: " << x << ", Result: " << result << std::endl;
    }

    for (int i = 0; i < numTestCases * .3; ++i)
    {
        size_t opt = generateRandomNumber(4, 5);
        long long x;

        if (opt == 4)
        {
            if (insertedValues.empty())
            {
                // If no values have been inserted yet, skip
                continue;
            }

            // Pick a random value from the inserted values to query
            x = generateRandomNumber(1, insertedValues.size());
        }
        else
        {
            x = generateRandomNumber(minVal, maxVal);
        }

        // Perform the operation and get the result
        long long result = SetOperation(opt, x);

        // Write the operation and the result to the file
        outFile << "Operation: " << opt << ", Value: " << x << ", Result: " << result << std::endl;
    }

    clear();
    outFile.close();

    std::cout << "Test cases have been generated and written to test_cases.txt" << std::endl;
    return 0;
}
