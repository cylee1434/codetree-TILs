import sys

INT_MIN = -sys.maxsize

a = list(map(int, list(input())))
length = len(a)

ans = INT_MIN
for i in range(length):
    a[i] = 1 - a[i]

    num = 0 
    for j in range(length):
        num = num * 2 + a[j]

    ans = max(ans, num)

    a[i] = 1 - a[i]
print(ans)