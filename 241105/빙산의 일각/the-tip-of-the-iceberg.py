from collections import deque

def count_iceberg(heights, sea_level):
    v = [False] * len(heights)
    groups = 0

    def bfs(i):
        q = deque([i])
        v[i] = True

        while q:
            idx = q.popleft()
            if idx > 0 and not v[idx-1] and heights[idx-1] > sea_level:
                v[idx-1] = True
                q.append(idx-1)
            if idx < len(heights) - 1 and not v[idx+1] and heights[idx+1] > sea_level:
                v[idx+1] = True
                q.append(idx+1)

    for i in range(len(heights)):
        if heights[i] > sea_level and not v[i]:
            groups += 1
            bfs(i)
    return groups

def max_iceberg_groups(heights):
    max_groups = 0
    for sea_level in set(heights):
        groups = count_iceberg(heights, sea_level)
        max_groups = max(max_groups, groups)
    return max_groups

n = int(input())
heights = [int(input()) for _ in range(n)]

print(max_iceberg_groups(heights))