#include <iostream>
#include <queue>
#include <algorithm>
#include <cassert>
using namespace std;

const int MAXN = 20;
const int MAXM = 20;
const int MAXF = 400;
const int MAXP = MAXF * 6;
const int INF = 1e9;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

int N, M, F;
int space_map[MAXN][MAXN];       // 미지의 공간 평면도
int space_node[MAXN][MAXN];      // 평면도에 대응하는 그래프 정점 번호 저장
int time_wall[5][MAXM][MAXM];    // 시간의 벽의 단면도
int time_node[5][MAXM][MAXM];    // 시간의 벽의 대응하는 그래프 정점 번호 저장

// 시간 이상 현상 정보 저장 구조체
struct event {
    // 행번호, 열번호, 방향, 확장주기, 시간 이상 현상 여부
    int xpos, ypos, direction, cycle, alive;
};

event event_lst[MAXF];
vector<vector<int>> graph;

// 필요한 최소한 턴 수
int dist[MAXP];

void make_graph(int N, int M) {
    int cnt = 0; 

    // 각 셀에 대해 대응될 번호를 차례로 부여
    // 평면도 중 시간의 벽이 아닌 부분의 셀들을 순회 후,
    // 단면도에 속하는 셀들을 단면도의 동->남->서->북->위 순회하며 번호 부여
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (space_map[i][j] != 3) {
                cnt++;
                space_node[i][j] = cnt;
            }
        }
    }

    // 단면도의 동, 남, 서, 북, 위 순으로 셀에 번호 부여
    for (int t = 0; t < 5; t++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                cnt++;
                time_node[t][i][j] = cnt;
            }
        }
    }

    // 부여한 번호의 정점들로 구성된 그래프
    // 정점 번호에 대응되는 셀과 인접 셀의 번호를 가지는 정점과 간선으로 연결
    // 최대 4개 정점 연결 가능
    // 단면도에서 오른쪽 인접 0, 아래쪽 인접 1, 왼쪽 인접 2, 위쪽 인접3
    graph = vector<vector<int>>(cnt + 1, vector<int>(4, -1));

    // 평면도에 속하는 셀에 대응되는 번호의 정점 쌍에 대해 간선 추가
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (space_map[i][j] != 3) {
                int idx = space_node[i][j];

                // 동, 남, 북, 서
                for (int dir = 0; dir < 4; dir++) {
                    int nx = i + dx[dir];
                    int ny = j + dy[dir];

                    if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
                    // 셀에 장애물이 있는 경우 넘어감
                    if (space_map[nx][ny] == 3) continue;

                    // 그렇지 않으면 이어줌
                    graph[idx][dir] = space_node[nx][ny];
                }
            }
        }
    }

    // 단면도의 동 남 서 북에 있는 셀들이 인접할 경우 
    // 대응되는 번호의 정점들을 이어줌
    for (int t = 0; t < 4; t++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {

                int idx = time_node[t][i][j];

                for (int dir = 0; dir < 4; dir++) {
                    int nx = i + dx[dir];
                    int ny = j + dy[dir];

                    // 행 범위가 넘어갔을 경우 통과
                    if (nx < 0 || nx >= M) continue;

                    // 열 번호가 0보다 작아질 경우, 시계방향 순으로 하나 앞에 있는 단면도의 가장 오른족 열의 셀과 인접
                    if (ny < 0) {
                        graph[idx][dir] = time_node[(t+1)%4][nx][M-1];
                    }

                    // 행 번호가 M-1보다 커질 경우, 반시계 방향 순으로 하나 앞에 있는 단면도의 가장 왼쪽 열의 셀과 인접
                    else if (ny >= M) {
                        graph[idx][dir] = time_node[(t+3)%4][nx][0];
                    }

                    // 그 외의 경우 평면도의 경우처럼 이어줌
                    else {
                        graph[idx][dir] = time_node[t][nx][ny];
                    }
                }
            }
        }
    }
    // 왼쪽 단면도에 속하는 셀들에 대응되는 번호의 정점 쌍에 대해 간선 추가
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            int idx = time_node[4][i][j];
            for (int d = 0; d < 4; d++) {
                int nx = i + dx[d];
                int ny = j + dy[d];
                // 범위 밖이면 넘어가고
                if (nx < 0 || nx >= M || ny <0 || ny >= M) continue;
                // 아니면 이어줌
                graph[idx][d] = time_node[4][nx][ny];
            }
        }
    }
    // 위쪽 단면도에 인접한 동 남 서 북 단면도 셀에 대해 대응되는 번호 정점 간선 추가

    // 동쪽 단면도 인접한 셀의 경우
    for (int i = 0; i < M; i++) {
        int idx = time_node[4][i][M-1];   // 인접한 위쪽 단면도 셀의 번호
        int idy = time_node[0][0][M-1-i]; // 인접한 동쪽 단면도 셀의 번호
        graph[idx][0] = idy;
        graph[idy][3] = idx;
    }

    // 남쪽 단면도 인접한 셀의 경우
    for (int i = 0; i < M; i++) {
        int idx = time_node[4][M-1][i]; // 인접한 위쪽 단면도 셀의 번호
        int idy = time_node[1][0][i];   // 인접한 남쪽 단면도 셀의 번호
        graph[idx][1] = idy;
        graph[idx][3] = idx;
    }

    // 서쪽 단면도 인접한 셀의 경우
    for (int i = 0; i < M; i++) {
        int idx = time_node[4][i][0];   // 인접한 위쪽 단면도 셀의 번호
        int idy = time_node[2][0][i];   // 인접한 서쪽 단면도 셀의 번호
        graph[idx][2] = idy;
        graph[idy][3] = idx;
    }

    // 북쪽 단면도 인접한 셀의 경우
    for (int i = 0; i < M; i++) {
        int idx = time_node[4][0][i];   // 인접한 위쪽 단면도 셀의 번호
        int idy = time_node[3][0][M-1-i];   // 인접한 북쪽 단면도 셀의 번호
        graph[idx][3] = idy;
        graph[idy][3] = idx;
    }

    // 평면도에서 시간의 벽이 시작하는 셀의 행, 열 번호
    int time_st = -1;
    int time_sy = -1;

    // 평면도에서 시간의 벽이 시작하는 셀의 위치 탐색
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (space_map[i][j] == 3) {
                time_st = i;
                time_sy = j;
                break;
            }
        }
        if (time_st != -1) break;
    }
    // 평면도와 인접한 단면도 셀들에 대응되는 번호의 정점을 잇는 간선 추가
    // 동쪽 단면도
    if (time_sy + M < N) {
        for (int i = 0; i < M; i++) {
            int idx = time_node[0][M-1][i];
            int idy = space_node[time_st + (M-1) - i][time_sy+M];
            graph[idx][1] = idy;
            graph[idy][2] = idx;
        }
    }
    // 남쪽 단면도
    if (time_st + M < N) {
        for (int i = 0; i < M; i++) {
            int idx = time_node[1][M-1][i];
            int idy = space_node[time_st+M][time_sy+i];
            graph[idx][1] = idy;
            graph[idy][3] = idx;
        }
    }
    // 서쪽 단면도
    if (time_sy > 0) {
        for (int i = 0; i < M; i++) {
            int idx = time_node[2][M-1][i];
            int idy = space_node[time_st+i][time_sy-1];
            graph[idx][1] = idy;
            graph[idy][0] = idx;
        }
    }
    // 북쪽 단면도
    if (time_st > 0) {
        for (int i = 0; i < M; i++) {
            int idx = time_node[3][M-1][i];
            int idy = space_node[time_st-1][time_sy+(M-1)-i];
            graph[idx][1] = idy;
            graph[idy][1] = idx;
        }
    }
    return;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M >> F;

    // 미지의 공간 입력
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> space_map[i][j];
        }
    }

    // 시간의 벽 동쪽
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> time_wall[0][i][j];
        }
    }

    // 시간의 벽 서쪽
    for (int i = 0; i < M ; i++) {
        for (int j = 0; j < M; j++) {
            cin >> time_wall[2][i][j];
        }
    } 

    // 시간의 벽 남쪽
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> time_wall[1][i][j];
        }
    }

    // 시간의 벽 북쪽
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> time_wall[3][i][j];
        }
    }

    // 시간의 벽 위쪽
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            cin >> time_wall[4][i][j];
        }
    }

    for (int i = 1; i <= F; i++) {
        cin >> event_lst[i].xpos >> event_lst[i].ypos >> event_lst[i].direction >> event_lst[i].cycle;
        if (event_lst[i].direction == 1) event_lst[i].direction = 2;
        else if (event_lst[i].direction == 2) event_lst[i].direction = 1;
        event_lst[i].alive = 1;
    }

    // 미지의 공간에 대응되는 그래프 생성
    make_graph(N, M);

    // 생성된 그래프의 정점의 개수는 N*N - M*M + 5*M*M = N*N + 4*M*M개
    // -1로 초기화
    fill(dist, dist + (N*N) + 4 * (M*M) + 1, -1);

    // 평면도에 장애물이 있는 경우 도달 불가하므로 미리 큰 값으로 세팅
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (space_map[i][j] == 3) continue;
            if (space_map[i][j] == 1) {
                int idx = space_node[i][j];
                dist[idx] = INF;
            }
        }
    }

    // 평면도에 시간 이상 현상의 시작점도 도달 불가하므로 장애물과 같이 처리
    for (int i = 1; i <= F; i++) {
        int x = event_lst[i].xpos;
        int y = event_lst[i].ypos;
        int idx = space_node[x][y];
        dist[idx] = INF;
    }

    // 단면도 위 장애물도 똑같이 처리
    for (int t = 0; t < 5; t++) {
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                if (time_wall[t][i][j] == 1) {
                    int idx = time_node[t][i][j];
                    dist[idx] = INF;
                }
            }
        }
    }

    // bfs 진행
    queue<int> q;

    int cell_st = -1;
    int cell_en = -1;

    // 탈출구 위치 탐색
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (space_map[i][j] == 4) {
                cell_en = space_node[i][j];
                break;
            } 
        }
    }

    // 타임머신의 시작점 탐색
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            if (time_wall[4][i][j] == 2) {
                cell_st = time_node[4][i][j];
                break;
            }
        }
    }

    q.push(cell_st);
    dist[cell_st] = 0;

    for (int runs = 1;; runs++) {
        // 현재 턴에 확장하는 이상 현상이 있으면 영향을 받는 셀 업데이트
        for (int i = 1; i <= F; i++) {
            if (!event_lst[i].alive) continue;
            if (runs % event_lst[i].cycle) continue;

            int nx = event_lst[i].xpos, ny = event_lst[i].ypos; // 이상 현상의 시작점

            // 이상현상의 방향에 따라 영향을 받는 셀의 좌표 구함
            if (event_lst[i].direction == 0) {
                ny += (runs / event_lst[i].cycle);
            } else if (event_lst[i].direction == 1) {
                nx += (runs / event_lst[i].cycle);
            } else if (event_lst[i].direction == 2) {
                ny -= (runs / event_lst[i].cycle);
            } else {
                nx -= (runs / event_lst[i].cycle);
            }

            if (nx < 0 || nx >= N || ny < 0 || ny >= N) {
                event_lst[i].alive = 0; 
                continue;
            }

            // 이상현상이 장애물이나 탈출구, 시간의 벽을 만난 경우 확산x
            if (space_map[nx][ny]) {
                event_lst[i].alive = 0;
                continue;
            }

            // 대응되는 셀의 번호는 타임머신 도달 불가
            int idx = space_node[nx][ny];
            dist[idx] = INF;
        }

        // 이번엔 도달 가능한 셀들의 번호를 저장할 벡터
        vector<int> next_cells;

        // 도달 가능한 셀 찾기
        while (!q.empty()) {
            int idx = q.front();
            q.pop();

            for (int d = 0; d < 4; d++) {
                int idy = graph[idx][d];    
                if (idy == -1) continue;       // 해당 방향으로 이동가능한 셀이 없는 경우 통과
                if (dist[idy] != -1) continue; // 이미 최소 턴의 수를 계산한 셀의 경우 통과
                dist[idy] = runs;
                next_cells.push_back(idy);     // 이번에 새로 도달 가능한 셀의 번호 추가
            }
        }

        // 새로 도달 가능한 셀들이 없으면 종료
        if (!(int) next_cells.size()) break;

        // 새로 도달 가능한 셀들에 대응하는 번호를 큐에 추가
        for (int i = 0; i < (int) next_cells.size(); i++) {
            q.push(next_cells[i]);
        }

        // 탈출구에 가기 위해 필요한 최소 턴수를 구했다면, 종료
        if (dist[cell_en] != -1) break;
    }
    // 정답 출력, 불가능하면 -1 출력
    cout << dist[cell_en] << '\n';
    return 0;
}


