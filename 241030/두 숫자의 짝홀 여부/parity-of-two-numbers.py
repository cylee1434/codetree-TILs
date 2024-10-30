# 두 정수 입력받기
a, b = map(int, input().split())

# 첫 번째 수의 짝홀 여부 출력
print("even" if a % 2 == 0 else "odd")

# 두 번째 수의 짝홀 여부 출력
print("even" if b % 2 == 0 else "odd")