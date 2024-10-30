# 두 정수 입력받기
a, b = map(int, input().split())

# 조건에 따른 출력
if a > b:
    print(a * b)
else:
    print(b // a)