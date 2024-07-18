from collections import deque

n, m = tuple(map(int, input().split()))

a = [
    list(map(int, input().split()))
    for _ in range(n)
]

visited = [
    [False for _ in range(m)]
    for _ in range(n)
]

q = deque()

def in_range(x,y):
    return 0 <= x and x < n and 0 <= y and y < m

def can_go(x,y):
    return in_range(x,y) and a[x][y] and not visited[x][y]

def bfs():
    while q:
        x, y = q.popleft()

        dxs = [0,1,0,-1]
        dys = [1,0,-1,0]
        for dx, dy in zip(dxs, dys):

            new_x, new_y = x + dx, y + dy

            if can_go(new_x, new_y):
                q.append((new_x, new_y))
                visited[new_x][new_y] = True






q.append((0,0))
visited[0][0] = True

bfs()

answer = 1 if visited[n-1][m-1] else 0
print(answer)