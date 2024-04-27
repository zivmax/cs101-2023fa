#include "../bst.hpp" // Include the header file where your SetOperations class is defined
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

int main()
{

    std::ifstream inFile("D:/CS_learning/Courses/SIST/CS101/HWs/CS101PAs/pa2_release/prob2/testgen/test_cases.txt");
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open the file for reading test cases." << std::endl;
        return -1;
    }

    std::string line;
    long long expected_result, actual_result;
    size_t opt;
    long long x;
    int passed_tests = 0, total_tests = 0;

    // Regular expression to match the input format
    std::regex linePattern(R"(Operation: (\d+), Value: (-?\d+), Result: (-?\d+))");

    std::smatch matches;

    // Initialize your BST or set operations class
    init(1); // You may need to adjust this initialization based on your actual implementation

    // Read and process each test case
    while (getline(inFile, line))
    {
        // Use regex to parse the line
        if (std::regex_match(line, matches, linePattern))
        {
            opt = std::stoul(matches[1].str());
            x = std::stoll(matches[2].str());
            expected_result = std::stoll(matches[3].str());

            // Perform the operation using your code
            actual_result = SetOperation(opt, x);

            // Compare the actual result with the expected result
            if (actual_result == expected_result)
            {
                ++passed_tests; // The test case passed
            }
            else
            {
                std::cerr << "Test case failed: Operation " << opt << " with value " << x
                          << ". Expected result: " << expected_result
                          << ", Actual result: " << actual_result << std::endl;
            }
        }
        else
        {
            std::cerr << "Invalid input format: " << line << std::endl;
        }
        ++total_tests;
    }

    // Cleanup
    clear();
    inFile.close();

    // Output the results of the testing
    std::cout << "Testing complete. Passed " << passed_tests << " out of " << total_tests << " test cases." << std::endl;

    return (passed_tests == total_tests ? 0 : -1);
}
