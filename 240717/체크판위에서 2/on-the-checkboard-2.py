n, m = tuple(map(int, input().split()))
grid = [
    input().split()
    for _ in range(n)
]

# 이동 시에 행, 열이 전부 증가하도록
# 모든 쌍을 다 잡아봄
cnt = 0
for i in range(1,n):
    for j in range(1,m):
        for k in range(i+1, n-1):
            for l in range(j+1, m-1):
                # 그 중 색깔이 전부 달라지는 경우에만 개수를 세줌
                if grid[0][0] != grid[i][j] and \
                   grid[i][j] != grid[k][l] and \
                   grid[k][l] != grid[n-1][m-1]:
                    cnt += 1
print(cnt)