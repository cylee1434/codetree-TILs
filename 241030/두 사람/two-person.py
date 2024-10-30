# 첫 번째 사람의 정보 입력받기
age1, sex1 = input().split()
age1 = int(age1)  # 나이를 정수로 변환

# 두 번째 사람의 정보 입력받기
age2, sex2 = input().split()
age2 = int(age2)  # 나이를 정수로 변환

# 조건에 따른 출력
if (age1 >= 19 and sex1 == 'M') or (age2 >= 19 and sex2 == 'M'):
    print(1)
else:
    print(0)