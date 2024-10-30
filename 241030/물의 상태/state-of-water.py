# 물의 온도 입력받기
n = int(input())

# 조건에 따른 출력
if n < 0:
    print("ice")
elif n >= 100:
    print("vapor")
else:
    print("water")