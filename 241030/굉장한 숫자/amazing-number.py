# 자연수 입력받기
n = int(input())

# 조건에 따른 출력
is_great = (n % 2 != 0 and n % 3 == 0) or (n % 2 == 0 and n % 5 == 0)
print("true" if is_great else "false")