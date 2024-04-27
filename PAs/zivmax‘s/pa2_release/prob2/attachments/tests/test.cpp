#include "../bst.hpp"
// #include "../../testgen/bst.hpp"
#include <cassert>

bool test_query_kth()
{
    std::size_t n = 21;
    init(n);

    SetOperation(1, 1);
    SetOperation(1, 2);
    SetOperation(1, 5);
    SetOperation(1, 6);
    SetOperation(1, 3);
    SetOperation(1, 4);
    SetOperation(1, 7);


    for (int i = 1; i <= 7; ++i)
    {
        if (SetOperation(4, i) != i) { return false; }
    }

    SetOperation(2, 1);
    SetOperation(2, 5);

    if (SetOperation(4, 1) != 2) { return false; }
    if (SetOperation(4, 2) != 3) { return false; }
    if (SetOperation(4, 3) != 4) { return false; }
    if (SetOperation(4, 4) != 6) { return false; }
    if (SetOperation(4, 5) != 7) { return false; }

    clear();

    init(n); // Initialize your data structure with the specified size

    // Perform operations according to your test cases
    SetOperation(1, 83);
    SetOperation(2, 83);
    SetOperation(1, 17);
    SetOperation(2, 17);
    SetOperation(1, 6);
    SetOperation(1, 90);
    SetOperation(2, 6);
    SetOperation(1, 16);
    SetOperation(2, 90);
    SetOperation(1, 65);
    SetOperation(1, 69);
    SetOperation(2, 69);
    SetOperation(2, 16);
    SetOperation(1, 68);
    SetOperation(2, 68);
    SetOperation(1, 7);
    SetOperation(1, 76);
    SetOperation(1, 34);

    // Only check results for operations with opt == 4
    if (SetOperation(4, 2) != 34) { return false; }
    if (SetOperation(4, 2) != 34) { return false; }
    if (SetOperation(4, 1) != 7) { return false; }
    if (SetOperation(4, 4) != 76) { return false; }
    if (SetOperation(4, 1) != 7) { return false; }
    if (SetOperation(4, 3) != 65) { return false; }
    if (SetOperation(4, 2) != 34) { return false; }
    if (SetOperation(4, 3) != 65) { return false; }
    if (SetOperation(4, 4) != 76) { return false; }

    return true;
}

bool test_query_ksum()
{
    std::size_t n = 16;
    init(n);

    SetOperation(1, 1);
    SetOperation(1, 2);
    SetOperation(1, 5);
    SetOperation(1, 6);
    SetOperation(1, 3);
    SetOperation(1, 4);
    SetOperation(1, 7);


    if (SetOperation(5, 1) != 2) { return false; }
    if (SetOperation(5, 2) != 2) { return false; }
    if (SetOperation(5, 3) != 12) { return false; }
    if (SetOperation(5, 4) != 2) { return false; }

    SetOperation(2, 1);
    SetOperation(2, 5);
    if (SetOperation(5, 1) != 8) { return false; }
    if (SetOperation(5, 2) != 5) { return false; }
    if (SetOperation(5, 10) != 0) { return false; }

    clear();

    init(n);

    // Insert your test cases here
    SetOperation(1, 76);
    SetOperation(2, 76);
    SetOperation(1, 22);
    SetOperation(1, 64);
    SetOperation(2, 64);
    SetOperation(1, 94);
    SetOperation(1, 87);
    SetOperation(2, 94);
    SetOperation(2, 87);
    SetOperation(1, 17);
    SetOperation(1, 27);
    SetOperation(2, 17);
    SetOperation(1, 18);
    SetOperation(2, 22);
    SetOperation(2, 27);
    SetOperation(1, 10);
    SetOperation(2, 10);
    SetOperation(2, 18);
    SetOperation(1, 26);
    SetOperation(2, 26);
    SetOperation(1, 56);

    // Check the result of the query operation
    if (SetOperation(5, 56) != 3136) { return false; }

    clear();

    init(n);

    // Perform operations without checking the result
    SetOperation(1, 12);
    SetOperation(2, 12);
    SetOperation(1, 87);
    SetOperation(2, 87);
    SetOperation(1, 82);
    SetOperation(2, 82);
    SetOperation(1, 80);
    SetOperation(1, 98);
    SetOperation(2, 80);
    SetOperation(2, 98);
    SetOperation(1, 95);
    SetOperation(1, 73);
    SetOperation(1, 85);
    SetOperation(1, 25);
    SetOperation(1, 84);
    SetOperation(1, 6);
    SetOperation(2, 84);

    // Perform operations and check the result for opt == 4
    if (SetOperation(4, 4) != 85) { return false; }
    if (SetOperation(4, 2) != 25) { return false; }
    if (SetOperation(4, 5) != 95) { return false; }

    // Perform operations and check the result for opt == 5
    if (SetOperation(5, 78) != 190) { return false; }
    if (SetOperation(5, 33) != 770) { return false; }
    if (SetOperation(4, 2) != 25) { return false; } // This line seems to be a duplicate and might be a mistake
    if (SetOperation(5, 61) != 88) { return false; }
    if (SetOperation(5, 46) != 1173) { return false; }
    if (SetOperation(5, 55) != 52) { return false; }

    return true;
}

bool test_insert()
{
    std::size_t n = 22;
    init(n);

    SetOperation(1, 1);
    SetOperation(1, 2);
    SetOperation(1, 5);
    SetOperation(1, 3);
    SetOperation(1, 4);


    if (SetOperation(3, 1) != true) { return false; }
    if (SetOperation(3, 2) != true) { return false; }
    if (SetOperation(3, 3) != true) { return false; }
    if (SetOperation(3, 4) != true) { return false; }
    if (SetOperation(3, 5) != true) { return false; }
    if (SetOperation(3, 6) != false) { return false; }


    SetOperation(1, 6);
    SetOperation(1, 10);
    SetOperation(1, 8);
    SetOperation(1, 7);
    SetOperation(1, 9);

    if (SetOperation(3, 6) != true) { return false; }
    if (SetOperation(3, 7) != true) { return false; }
    if (SetOperation(3, 8) != true) { return false; }
    if (SetOperation(3, 9) != true) { return false; }
    if (SetOperation(3, 10) != true) { return false; }
    if (SetOperation(3, 10000) != false) { return false; }

    return true;
}

bool test_rebuild()
{
    std::size_t n = 293 + 180;
    init(n);

    for (int i = 10; i <= 100; ++i)
    {
        SetOperation(1, i);
    }

    for (int i = 10; i <= 100; ++i)
    {
        if (SetOperation(3, i) != true) { return false; }
    }

    for (int i = 10; i <= 20; ++i)
        SetOperation(2, i);

    for (int i = 200; i > 100; --i)
    {
        SetOperation(1, i);
    }

    for (int i = 21; i <= 200; ++i)
    {
        if (SetOperation(3, i) != true) { return false; }
    }

    return true;
}

bool test_erase()
{
    std::size_t n = 19;
    init(n);

    SetOperation(1, 1);
    SetOperation(1, 2);
    SetOperation(1, 5);
    SetOperation(1, 6);
    SetOperation(1, 3);
    SetOperation(1, 4);
    SetOperation(1, 7);

    SetOperation(2, 4);
    if (SetOperation(3, 4) != false) { return false; }

    SetOperation(1, 3);
    SetOperation(1, 3);
    SetOperation(1, 4);
    SetOperation(2, 5);
    if (SetOperation(3, 5) != false) { return false; }

    SetOperation(2, 1);
    if (SetOperation(3, 1) != false) { return false; }
    if (SetOperation(3, 2) != true) { return false; }
    SetOperation(2, 2);
    if (SetOperation(3, 2) != false) { return false; }

    return true;
}

void test_runtime()
{
    size_t runtimes = 100000;
    init(runtimes);

    clock_t start_time = clock();
    for (size_t i = 0; i < runtimes; i++)
    {
        SetOperation(1, i);
    }

    clock_t end_time = clock();
    std::cout << "The run time is: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << "s"
              << "\t";
    if ((double) (end_time - start_time) / CLOCKS_PER_SEC > 1.5)
    {
        std::cout << "(too long!)" << std::endl;
    }
}


int main()
{
    bool allPassed = true;

    // Run each test and print individual results.
    bool result;

    result = test_insert();
    printf("test_insert: %s\n", result ? "PASSED" : "FAILED");
    allPassed &= result;
    clear();

    result = test_rebuild();
    printf("test_rebuild: %s\n", result ? "PASSED" : "FAILED");
    allPassed &= result;
    clear();

    result = test_erase();
    printf("test_erase: %s\n", result ? "PASSED" : "FAILED");
    allPassed &= result;
    clear();

    result = test_query_kth();
    printf("test_query_kth: %s\n", result ? "PASSED" : "FAILED");
    allPassed &= result;
    clear();

    result = test_query_ksum();
    printf("test_query_ksum: %s\n", result ? "PASSED" : "FAILED");
    allPassed &= result;
    clear();

    // Print the final result based on the outcome of all tests.
    if (allPassed)
    {
        printf("\nAll tests passed!\n");
    }
    else
    {
        printf("\nSome tests failed.\n");
    }

    test_runtime();

    // Return 0 if all tests passed, or 1 if some tests failed.
    return allPassed ? 0 : 1;
}
