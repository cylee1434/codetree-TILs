MAX_N = 100000 + 5
is_underwater = [False] * MAX_N
ice_berg = []
num_iceberg = int(input())

# 빙산의 높이와 높이를 튜플로 저장
for i in range(num_iceberg):
    heights = int(input())
    ice_berg.append((i, heights))

# 높이를 기준으로 정렬
ice_berg.sort(key=lambda x: x[1])

current_island = 1
max_island = 1

# 모든 빙산에 대해 반복
for i in range(num_iceberg):
    idx = ice_berg[i][0]

    # 현재 빙산을 물에 잠긴 것으로 표시
    is_underwater[idx] = True
    
    # 왼쪽과 오른쪽에 이웃한 빙산이 있는지 확인
    has_left = idx > 0 and not is_underwater[idx - 1]
    has_right = idx < num_iceberg - 1 and not is_underwater[idx + 1]

    # 왼쪽과 오른쪽 모두 빙산이 있는 경우
    if has_left and has_right:
        current_island += 1
    # 왼쪽과 오른쪽 모두 빙산이 없는 경우
    elif not has_left and not has_right:
        current_island -= 1

    # 현재 높이에서의 빙산 덩어리 개수를 최대값과 비교하여 갱신
    if (i == num_iceberg - 1 or ice_berg[i + 1][1] > ice_berg[i][1]) and current_island > max_island:
        max_island = current_island

print(max_island)