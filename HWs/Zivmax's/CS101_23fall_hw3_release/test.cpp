#include <iostream>

void print(int *arr, int n)
{
    for (size_t i = 0; i < n; i++)
    {
        std::cout << arr[i] << ", ";
    }
    std::cout << "\b\b " << std::endl;
}
int merge(int arr[], int left, int mid, int right)
{
    int length = right - left + 1;
    int *temp = new int[length];
    int i = 0;
    int p1 = left;
    int p2 = mid + 1;
    int result = 0;
    while (p1 <= mid && p2 <= right)
    {
        if (arr[p1] < arr[p2])
        {
            // Add arr[p1]'s contribution to final answer.
            result += arr[p1] * (right - p2 + 1);
            // move the elements of arr into temp.
            // (for the following three lines, you may not fill all of them.)
            temp[i++] = arr[p1++];
        }
        else
        {
            // move the elements of arr into temp.
            // (for the following three lines, you may not fill all of them.)
            temp[i++] = arr[p2++];
        }
    }
    while (p1 <= mid)
    {
        temp[i++] = arr[p1++];
    }
    while (p2 <= right)
    {
        temp[i++] = arr[p2++];
    }
    for (int j = 0; j < length; j++)
    {
        arr[left + j] = temp[j];
    }
    delete[] temp;
    return result;
}
int split(int arr[], int left, int right)
{
    if (left == right)
        return 0;
    int mid = (left + right) / 2;
    int result = split(arr, left, mid) + split(arr, mid + 1, right);
    return result + merge(arr, left, mid, right);
}

int main()
{
    int arr[] = {1, 3, 4, 2, 5};
    int n = sizeof(arr) / sizeof(arr[0]);
    print(arr, n);
    std::cout << split(arr, 0, n - 1) << std::endl;
    print(arr, n);
    return 0;
}
