from collections import deque

n, k = tuple(map(int, input().split()))

grid = [
    list(map(int,input().split()))
    for _ in range(n)
]

v = [
    [False for _ in range(n)]
    for _ in range(n)
]

dx = [0,0,-1,1]
dy = [-1,1,0,0]

q = deque()

def bfs():
    
    while q:
        x, y = q.popleft()

        for di, dj in zip(dx,dy):
            nx, ny = x + di, y + dj

            if 0<= nx < n and 0 <= ny < n and grid[nx][ny] == 0 and not v[nx][ny]:
                q.append((nx,ny))
                v[nx][ny] = True

for _ in range(k):
    x, y = tuple(map(int, input().split()))
    q.append((x-1, y-1))
    v[x-1][y-1] = True

bfs()

ans = sum([1 for i in range(n) for j in range(n) if v[i][j]])

print(ans)