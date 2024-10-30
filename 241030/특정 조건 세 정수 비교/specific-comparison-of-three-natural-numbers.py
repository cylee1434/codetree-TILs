a, b, c = map(int, input().split())

fist_condition = 1 if a == min(a,b,c) else 0
second_condition = 1 if a==b==c else 0

print(fist_condition, second_condition)