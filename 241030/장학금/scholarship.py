# 중간고사와 기말고사 점수 입력받기
midterm, final = map(int, input().split())

# 조건에 따른 장학금 계산 및 출력
if midterm >= 90:
    if final >= 95:
        print(100000)
    elif final >= 90:
        print(50000)
    else:
        print(0)
else:
    print(0)