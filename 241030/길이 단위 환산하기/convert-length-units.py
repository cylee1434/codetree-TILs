# 환산 상수
ft_to_cm = 30.48

# 실수 입력받기
n = float(input())

# cm로 환산 후 소수 첫째 자리까지 반올림하여 출력
cm = n * ft_to_cm
print(f"{cm:.1f}")