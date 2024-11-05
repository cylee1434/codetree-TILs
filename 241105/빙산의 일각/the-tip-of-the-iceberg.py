from collections import deque

def count_iceberg_groups(heights, sea_level):
    n = len(heights)
    visited = [False] * n  # Reset visited array for each sea level
    groups = 0

    def bfs(start):
        q = deque([start])
        visited[start] = True
        while q:
            idx = q.popleft()
            for neighbor in (idx - 1, idx + 1):
                if 0 <= neighbor < n and not visited[neighbor] and heights[neighbor] > sea_level:
                    visited[neighbor] = True
                    q.append(neighbor)

    for i in range(n):
        if heights[i] > sea_level and not visited[i]:
            groups += 1
            bfs(i)

    return groups

def max_iceberg_groups(heights):
    heights_sorted = sorted(set(heights), reverse=True)  # Unique heights in descending order
    max_groups = 0

    # Calculate the number of iceberg groups for each sea level
    for sea_level in heights_sorted:
        groups = count_iceberg_groups(heights, sea_level)
        max_groups = max(max_groups, groups)

    return max_groups

# Input processing
n = int(input())
heights = [int(input()) for _ in range(n)]

print(max_iceberg_groups(heights))