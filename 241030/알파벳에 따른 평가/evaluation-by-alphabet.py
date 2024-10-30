# 문자 입력받기
grade = input().strip()

# 조건에 따른 평가 출력
if grade == 'S':
    print("Superior")
elif grade == 'A':
    print("Excellent")
elif grade == 'B':
    print("Good")
elif grade == 'C':
    print("Usually")
elif grade == 'D':
    print("Effort")
else:
    print("Failure")