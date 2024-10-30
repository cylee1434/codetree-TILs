# 월 입력받기
m = int(input())

# 조건에 따른 계절 출력
if 3 <= m <= 5:
    print("Spring")
elif 6 <= m <= 8:
    print("Summer")
elif 9 <= m <= 11:
    print("Fall")
else:
    print("Winter")