# 첫 번째 줄에서 a 입력받기
a = int(input())

# 두 번째 줄에서 b, c, d, e 입력받기
b, c, d, e = map(int, input().split())

# 각 비교 결과 출력
print(1 if a > b else 0)
print(1 if a > c else 0)
print(1 if a > d else 0)
print(1 if a > e else 0)