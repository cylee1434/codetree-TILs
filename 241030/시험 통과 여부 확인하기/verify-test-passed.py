# 점수 입력받기
n = int(input())

# 조건에 따른 출력
if n >= 80:
    print("pass")
else:
    print(f"{80 - n} more score")