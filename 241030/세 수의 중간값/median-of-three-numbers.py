# 세 정수 입력받기
a, b, c = map(int, input().split())

# 조건에 따른 출력
print(1 if a < b < c else 0)