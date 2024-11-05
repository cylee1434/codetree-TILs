import sys
input = sys.stdin.readline

def consecutive(nums):
    
    k_nums = set(nums)
    max_length = 0

    for k in k_nums:
        remain = [num for num in nums if num != k]

        if not remain:
            continue

        c_length = 1
        max_c_length = 1

        for i in range(1, len(remain)):
            if remain[i] == remain[i-1]:
                c_length += 1
            else:
                max_c_length = max(max_c_length, c_length)
                c_length = 1

        max_c_length = max(max_c_length, c_length)
        max_length = max(max_length, max_c_length)
    
    return max_length

n = int(input())
nums = [int(input()) for _ in range(n)]

print(consecutive(nums))