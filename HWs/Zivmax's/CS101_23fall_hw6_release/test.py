import heapq


def heapify_list(nums):
    heapq.heapify(nums)


# Example usage:
numbers = [6,5,4,3,2,1]
heapify_list(numbers)
print("Min-Heap:", numbers)
