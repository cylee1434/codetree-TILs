# 키(cm)와 몸무게(kg) 입력받기
h, w = map(int, input().split())

# BMI 계산
b = 10000 * w / (h * h)

# 소수 첫째 자리에서 버림하여 출력
print(int(b))

# 비만 판정
if b >= 25:
    print("Obesity")