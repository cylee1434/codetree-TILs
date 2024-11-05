import sys

# 최대값 정의
MAX_N = 100000 + 5

# 빙산 배열과 물에 잠긴 여부를 기록할 배열
icebergs = []
is_underwater = [False] * MAX_N

# 빙산의 개수 입력
num_icebergs = int(input())

# 빙산의 높이와 인덱스를 입력받아 튜플로 저장
for i in range(num_icebergs):
    height = int(input())
    icebergs.append((i, height))  # (index, height) 형태의 튜플로 저장

# 빙산 배열을 높이를 기준으로 정렬 (height 기준으로 정렬)
icebergs.sort(key=lambda x: x[1])

# 현재 덩어리의 개수와 최대 덩어리 개수를 초기화
current_islands = 1
max_islands = 1

# 모든 빙산에 대해 반복
for i in range(num_icebergs):
    idx = icebergs[i][0]

    # 현재 빙산을 물에 잠긴 것으로 표시
    is_underwater[idx] = True

    # 왼쪽과 오른쪽에 이웃한 빙산이 있는지 확인
    has_left_neighbor = idx > 0 and not is_underwater[idx - 1]
    has_right_neighbor = idx < num_icebergs - 1 and not is_underwater[idx + 1]

    # 왼쪽과 오른쪽에 이웃한 빙산이 모두 있는 경우
    if has_left_neighbor and has_right_neighbor:
        current_islands += 1
    # 왼쪽과 오른쪽에 이웃한 빙산이 모두 없는 경우
    elif not has_left_neighbor and not has_right_neighbor:
        current_islands -= 1

    # 현재 높이에서의 빙산 덩어리 개수를 최대값과 비교하여 갱신
    if (i == num_icebergs - 1 or icebergs[i + 1][1] > icebergs[i][1]) and current_islands > max_islands:
        max_islands = current_islands

# 최대 빙산 덩어리 개수를 출력
print(max_islands)