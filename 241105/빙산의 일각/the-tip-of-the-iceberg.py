from collections import deque

def count_iceberg(heights, sea_level):
    v = [False] * len(heights)
    groups = 0

    def bfs(i):
        q = deque([i])
        v[i] = True

        while q:
            idx = q.popleft()
            if idx > 0 and not v[idx - 1] and heights[idx - 1] > sea_level:
                v[idx - 1] = True
                q.append(idx - 1)
            if idx < len(heights) - 1 and not v[idx + 1] and heights[idx + 1] > sea_level:
                v[idx + 1] = True
                q.append(idx + 1)

    for i in range(len(heights)):
        if heights[i] > sea_level and not v[i]:
            groups += 1
            bfs(i)
    return groups

def max_iceberg_groups(heights):
    max_groups = 0
    sorted_heights = sorted(set(heights))  # 고유한 높이를 정렬하여 탐색
    
    for sea_level in sorted_heights:
        groups = count_iceberg(heights, sea_level)
        if groups > max_groups:
            max_groups = groups
        else:
            # 덩어리 개수가 줄어들기 시작하면 더 이상 탐색할 필요 없음
            break
    
    return max_groups

# 입력 처리
n = int(input())
heights = [int(input()) for _ in range(n)]

# 결과 출력
print(max_iceberg_groups(heights))