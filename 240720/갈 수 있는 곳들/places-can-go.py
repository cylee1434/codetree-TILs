from collections import deque

n, k = map(int, input().split())

# 입력 받은 격자 정보
grid = [list(map(int, input().split())) for _ in range(n)]

# 시작점 리스트
start_points = [tuple(map(int, input().split())) for _ in range(k)]

# 방문 여부를 저장할 2차원 리스트
visited = [[False] * n for _ in range(n)]

# 방향 벡터 (상하좌우)
dx = [-1, 1, 0, 0]
dy = [0, 0, -1, 1]

def bfs(start_points):
    q = deque(start_points)
    for x, y in start_points:
        visited[x][y] = True
    
    while q:
        x, y = q.popleft()
        
        for i in range(4):
            nx, ny = x + dx[i], y + dy[i]
            
            if 0 <= nx < n and 0 <= ny < n and not visited[nx][ny] and grid[nx][ny] == 0:
                visited[nx][ny] = True
                q.append((nx, ny))

# 시작점을 0 기반 인덱스로 변환하여 bfs 함수 호출
start_points = [(r-1, c-1) for r, c in start_points]
bfs(start_points)

# 방문 가능한 칸 수를 계산
reachable_count = sum(sum(row) for row in visited)

print(reachable_count)