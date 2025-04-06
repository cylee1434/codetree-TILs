#include <iostream>
using namespace std;
typedef pair<int, int> pii;

int N, M, P, C, D;
int board[53][53];
int santa_score[33];    // 산타 점수
bool is_alive[33];      // 산타 생존 여부
int stun[33];

pii rudolf;
pii santa_pos[33];

int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

bool inRange(int x, int y) {
    return 1 <= x && x <= N && 1 <= y && y <= N;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M >> P >> C >> D;
    cin >> rudolf.first >> rudolf.second;
    board[rudolf.first][rudolf.second] = -1;    // 루돌프 위치 -1

    for (int i = 1; i <= P; i++) {
        int id;
        cin >> id;
        cin >> santa_pos[id].first >> santa_pos[id].second;
        board[santa_pos[id].first][santa_pos[id].second] = id; 
        is_alive[id] = true;
    }

    for (int turn = 1; turn <= M; turn++) {
        // 루돌프 이동
        int cx = 10000, cy = 10000, ci = 0;

        // 살아 있는 산타 중 가장 가까운 산타 찾기
        for (int i = 1; i <= P; i++) {
            if (!is_alive[i]) continue;

            pair<int, pii> curBest = {
                (cx - rudolf.first) * (cx - rudolf.first) +
                (cy - rudolf.second) * (cy - rudolf.second),
                {-cx, -cy}
            };

            pair<int, pii> curVal = {
                (santa_pos[i].first - rudolf.first) * (santa_pos[i].first - rudolf.first) +
                (santa_pos[i].second - rudolf.second) * (santa_pos[i].second - rudolf.second),
                {-santa_pos[i].first, -santa_pos[i].second}
            };

            if (curVal < curBest) { // 값이 작을수록 좋은 조건을 pair로 구성, 거리->x좌표->y좌표 순
                cx = santa_pos[i].first;
                cy = santa_pos[i].second;
                ci = i;
            }
        }

        // 가장 가까운 산타 쪽으로 루돌프 이동
        if (ci) {
            pii prevRudolf = rudolf;
            int mv_x = 0;
            if (cx > rudolf.first) mv_x = 1;
            else if (cx < rudolf.first) mv_x = -1;

            int mv_y = 0;
            if (cy > rudolf.second) mv_y = 1;
            else if (cy < rudolf.second) mv_y = -1;

            rudolf.first += mv_x;
            rudolf.second += mv_y;
            board[prevRudolf.first][prevRudolf.second] = 0; 

            // 루돌프 충돌 -> 산타 점수 획득 및 이동 시킴
            if (rudolf.first == cx && rudolf.second == cy) {
                int fx = cx + mv_x * C;
                int fy = cy + mv_y * C;
                int lx = fx;
                int ly = fy;
                stun[ci] = turn + 1;

                // 만약 이동 위치에 다른 산타 있으면 연쇄 이동 
                while (inRange(lx, ly) and board[lx][ly] > 0) {
                    lx += mv_x;
                    ly += mv_y;
                }

                // 연쇄 이동 일어난 경우, 마지막 위치 -> 순차적으로 산타 한 칸 이동
                while (!(lx == fx and ly == fy)) {
                    int bx = lx - mv_x;
                    int by = ly - mv_y;

                    if (!inRange(bx, by)) break;
                    int idx = board[bx][by];

                    if (!inRange(lx, ly)) {
                        is_alive[idx] = false;
                    } else {
                        board[lx][ly] = board[bx][by];
                        santa_pos[idx] = {lx, ly};
                    }
                    lx = bx;
                    ly = by;
                }
                santa_score[ci] += C;
                santa_pos[ci] = {fx, fy};
                if (inRange(fx, fy)) {
                    board[fx][fy] = ci;
                } else {
                    is_alive[ci] = false;
                }
            }
        }
        board[rudolf.first][rudolf.second] = -1; 

        // 산타들은 가장 가까운 루돌프 방향으로 한 칸 이동
        for (int i = 1; i <= P; i++) {
            if (!is_alive[i] || stun[i] >= turn) continue;

            int minDist = (santa_pos[i].first - rudolf.first) * (santa_pos[i].first - rudolf.first) +
            (santa_pos[i].second - rudolf.second) * (santa_pos[i].second - rudolf.second);
            int mvDir = -1;

            for (int dir = 0; dir < 4; dir++) {
                int nx = santa_pos[i].first + dx[dir];
                int ny = santa_pos[i].second + dy[dir];

                if (!inRange(nx, ny) || board[nx][ny] > 0) continue;

                int dist = (nx - rudolf.first) * (nx - rudolf.first) + (ny - rudolf.second) * (ny - rudolf.second);
                if (dist < minDist) {
                    minDist = dist;
                    mvDir = dir;
                }
            }
            if (mvDir != -1) {
                int nx = santa_pos[i].first + dx[mvDir];
                int ny = santa_pos[i].second + dy[mvDir];

                // 산타 이동으로 충돌한 경우, 산타 점수 획득 및 이동 처리
                if (nx == rudolf.first && ny == rudolf.second) {
                    stun[i] = turn + 1;

                    int mv_x = -dx[mvDir];
                    int mv_y = -dy[mvDir];
                    
                    int fx = nx + mv_x * D;
                    int fy = ny + mv_y * D;
                    int lx = fx;
                    int ly = fy;

                    if (D == 1) {
                        santa_score[i] += D;
                    } else {
                        while (inRange(lx, ly) and board[lx][ly] > 0) {
                            lx += mv_x;
                            ly += mv_y;
                        }

                        while (!(lx == fx and ly == fy)) {
                            int bx = lx - mv_x;
                            int by = ly - mv_y;

                            if (!inRange(bx, by)) break;

                            int idx = board[bx][by];

                            if (!inRange(lx, ly)) {
                                is_alive[idx] = false;
                            } else {
                                board[lx][ly] = board[bx][by];
                                santa_pos[idx] = {lx, ly};
                            }

                            lx = bx;
                            ly = by;
                        }
                        santa_score[i] += D;
                        board[santa_pos[i].first][santa_pos[i].second] = 0;
                        santa_pos[i] = {fx, fy};
                        if (inRange(fx, fy)) {
                            board[fx][fy] = i;
                        } else {
                            is_alive[i] = false;
                        }
                    }
                }
                else {
                    board[santa_pos[i].first][santa_pos[i].second] = 0;
                    santa_pos[i] = {nx, ny};
                    board[nx][ny] = i;
                }
            }
        }
        for (int i = 1; i <= P; i++) {
            if (is_alive[i]) santa_score[i]++;
        }
    }
    for (int i = 1; i <= P; i++) {
        cout << santa_score[i] << " ";
    }

}