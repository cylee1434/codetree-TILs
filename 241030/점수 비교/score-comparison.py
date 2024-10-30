# 첫 번째 줄에 A의 수학과 영어 점수 입력받기
a_math, a_english = map(int, input().split())

# 두 번째 줄에 B의 수학과 영어 점수 입력받기
b_math, b_english = map(int, input().split())

# 조건에 따른 출력
print(1 if a_math > b_math and a_english > b_english else 0)