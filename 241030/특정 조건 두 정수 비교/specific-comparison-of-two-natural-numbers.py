# 두 정수 입력받기
a, b = map(int, input().split())

# 첫 번째 수가 더 작으면 1, 아니면 0
first_condition = 1 if a < b else 0

# 두 수가 같으면 1, 아니면 0
second_condition = 1 if a == b else 0

# 결과 출력
print(first_condition, second_condition)