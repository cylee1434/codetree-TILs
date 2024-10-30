a = int(input())

# 첫 번째 과정
a = a // 2 if a % 2 == 0 else (a + 1) // 2

# 두 번째 과정
result = a // 2 if a % 2 == 0 else (a + 1) // 2

print(result)