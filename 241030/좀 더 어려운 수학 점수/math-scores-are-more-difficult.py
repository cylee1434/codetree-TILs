# A 학생의 수학 점수와 영어 점수 입력받기
a_math, a_english = map(int, input().split())

# B 학생의 수학 점수와 영어 점수 입력받기
b_math, b_english = map(int, input().split())

# 우선순위 판단 및 출력
if a_math > b_math:
    print("A")
elif a_math < b_math:
    print("B")
else:
    # 수학 점수가 같은 경우 영어 점수 비교
    if a_english > b_english:
        print("A")
    else:
        print("B")