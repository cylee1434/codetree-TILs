# 두 정수 입력받기
a, b = map(int, input().split())

# 조건에 따른 출력
print(1 if a >= b else 0)  # a가 b보다 같거나 큰가?
print(1 if a > b else 0)   # a가 b보다 큰가?
print(1 if b >= a else 0)  # b가 a보다 같거나 큰가?
print(1 if b > a else 0)   # b가 a보다 큰가?