from collections import deque

n, k = tuple(map(int, input().split()))

grid = [
    list(map(int, input().split()))
    for _ in range(n)
]

v = [
    [False for _ in range(n)]
    for _ in range(n)
]

sx, sy = map(int, input().split())

sx -= 1
sy -= 1

dx = [0, 0, -1, 1]
dy = [-1, 1, 0, 0]

ans_x = sx
ans_y = sy

for _ in range(k):
    v = [[False] * n for _ in range(n)]
    
    q = deque()
    q.append([ans_x, ans_y])
    v[ans_x][ans_y] = True

    max_num = 0
    max_x, max_y = ans_x, ans_y

    while q:
        cx, cy = q.popleft()

        for di, dj in zip(dx, dy):
            nx, ny = cx + di, cy + dj

            if 0 <= nx < n and 0 <= ny < n:
                if not v[nx][ny] and grid[nx][ny] < grid[sx][sy]:
                    q.append([nx, ny])
                    v[nx][ny] = True

                    if grid[nx][ny] > max_num:
                        max_num = grid[nx][ny]
                        max_x, max_y = nx, ny
                    elif grid[nx][ny] == max_num:
                        if max_x > nx:
                            max_x, max_y = nx, ny
                        elif max_x == nx and max_y > ny:
                            max_x, max_y = nx, ny

    ans_x, ans_y = max_x, max_y
    sx, sy = ans_x, ans_y

print(f'{ans_x + 1} {ans_y + 1}')