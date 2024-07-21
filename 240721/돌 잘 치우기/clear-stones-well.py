from itertools import combinations
from collections import deque

# ac = accessible count 
def bfs(grid, sp):
    v = [[False]*n for _ in range(n)]
    q = deque()
    
    for (r,c) in sp:
        q.append((r,c))
        v[r][c] = True

    directions = [(0,1), (1,0), (0,-1), (-1,0)]
    ac = 0

    while q:
        x, y = q.popleft()
        ac += 1
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < n and 0 <= ny < n and not v[nx][ny] and grid[nx][ny] == 0:
                v[nx][ny] = True
                q.append((nx, ny))
                
    return ac


# aa = accessible area
def max_aa(n,k,m,grid,sp):
    stones = [(i,j) for i in range(n) for j in range(n) if grid[i][j] == 1]
    max_area = 0

    for stone_to_remove in combinations(stones,m):
        new_grid = [row[:] for row in grid]

        for (x,y) in stone_to_remove:
            new_grid[x][y] = 0

        aa = bfs(new_grid, sp)
        max_area = max(max_area, aa)
    
    return max_area

    
# Input processing, sp = start_point
n, k, m = list(map(int, input().split()))
grid = [list(map(int, input().split())) for _ in range(n)]
sp = [tuple(map(lambda x: int(x) - 1, input().split())) for _ in range(k)]

# Calculate and print the result
result = max_aa(n,k,m,grid,sp)
print(result)