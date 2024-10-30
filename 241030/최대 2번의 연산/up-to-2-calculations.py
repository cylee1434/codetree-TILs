a = int(input())

# 첫 번째 과정: 짝수인지 확인
if a % 2 == 0:
    a = a // 2
else:
    a = (a + 1) // 2

# 두 번째 과정: 최종 조건 확인
if a % 2 == 0:
    print(a)
else:
    print((a + 1) // 2)