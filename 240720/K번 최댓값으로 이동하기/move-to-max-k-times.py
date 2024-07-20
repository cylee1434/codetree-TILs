from collections import deque

n, turn = map(int, input().split())

ground = [list(map(int, input().split())) for _ in range(n)]

sx, sy = map(int, input().split())

# 조건1 : 시작 위치가 1씩 크게 주어지기 때문에 그래프를 n+1 크기만큼 받을 게 아니라면 -1 해준 값으로 시작하고 결과 출력할 때 +1 해서 출력해야 함
sx -= 1
sy -= 1

dx = [-1, 1, 0, 0]
dy = [0, 0, -1, 1]

ans_x = sx
ans_y = sy

# 조건2 : k(저는 turn 변수로 받았음)만큼 반복하며 bfs를 수행해야 함
for _ in range(turn):
    max_num = 0
    visited = [[False]*n for _ in range(n)]

    q = deque()

    q.append([ans_x, ans_y])
    visited[ans_x][ans_y] = True

    # bfs
    while q:
        now_x, now_y = q.pop()

        for i in range(4):
            nx = now_x + dx[i]
            ny = now_y + dy[i]

            if 0 <= nx < n and 0 <= ny < n:

                # 조건3 : 시작 위치의 값보다 작은값 중에 제일 큰 값과 그 좌표를 갱신해야 함
                if not visited[nx][ny] and ground[nx][ny] < ground[sx][sy]:
                    q.append([nx, ny])
                    visited[nx][ny] = True

                    if ground[nx][ny] > max_num:
                        max_num = ground[nx][ny]
                        ans_x, ans_y = nx, ny

                    # 조건4 : 제일 큰 값을 가진 좌표가 여러개일 경우 먼저 행을 고려하고 그 다음 열을 고려해서 좌표를 갱신해야 함
                    elif ground[nx][ny] == max_num:
                        if ans_x > nx:
                            ans_x, ans_y = nx, ny
                        elif ans_x == nx:
                            if ans_y > ny:
                                ans_x, ans_y = nx, ny

    # 시작 위치 갱신
    sx = ans_x
    sy = ans_y

print(f'{ans_x+1} {ans_y+1}')