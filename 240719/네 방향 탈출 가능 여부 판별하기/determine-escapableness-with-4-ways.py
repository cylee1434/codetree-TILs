from collections import deque

n, m = tuple(map(int, input().split()))

a = [
    list(map(int, input().split()))
    for _ in range(n)
]

v = [
    [False for _ in range(m)]
    for _ in range(n)
]

dx = [0, 0, -1, 1]
dy = [1, -1, 0, 0]

q = deque()

def bfs():
    while q:
        x, y = q.popleft()

        for di, dj in zip(dx, dy):
            nx, ny = x + di, y + dj

            if 0 <= nx < n and 0 <= ny < m and not v[nx][ny] and a[nx][ny] > 0:
                q.append((nx, ny))
                v[nx][ny] = True
        
q.append((0, 0))
v[0][0] = True

bfs()

answer = 1 if v[n-1][m-1] else 0
print(answer)