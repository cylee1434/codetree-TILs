#include <iostream>
#include <vector>
#include <queue>
#define INF 1e9
using namespace std;
typedef pair<int, int> pii;

int N, M, P, C, D;
int board[52][52];
int points[52];
pii rudolf;

struct Santa {
    int r, c;
    bool is_out = false;
    int score = 0;
    int faint = 0;
};

Santa santas[33];

int dx[4] = {-1, 0, 1, 0};  // ↑ → ↓ ←
int dy[4] = {0, 1, 0, -1};

bool is_inrange(int x, int y) {
    return 1 <= x && x <= N && 1 <= y && y <= N;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M >> P >> C >> D;
    cin >> rudolf.first >> rudolf.second;
    board[rudolf.first][rudolf.second] = -1;

    for (int i = 0; i < P; i++) {
        int santaNum, sr, sc;
        cin >> santaNum >> sr >> sc;
        santas[santaNum] = {sr, sc, false, 0, 0};
        board[sr][sc] = santaNum;
    }

    for (int t = 1; t <= M; t++) {
        int closestX = 10000, closestY = 10000, closestIdx = 0;

        for (int i = 1; i <= P; i++) {
            if (santas[i].is_out) continue;

            pair<int, pii> curBest = {
                (closestX - rudolf.first) * (closestX - rudolf.first) +
                (closestY - rudolf.second) * (closestY - rudolf.second),
                {-closestX, -closestY}
            };

            pair<int, pii> curVal = {
                (santas[i].r - rudolf.first) * (santas[i].r - rudolf.first) +
                (santas[i].c - rudolf.second) * (santas[i].c - rudolf.second),
                {-santas[i].r, -santas[i].c}
            };

            if (curVal < curBest) {
                closestX = santas[i].r;
                closestY = santas[i].c;
                closestIdx = i;
            }
        }

        if (closestIdx) {
            pii prev = rudolf;
            int dxr = (closestX > rudolf.first) - (closestX < rudolf.first);
            int dyr = (closestY > rudolf.second) - (closestY < rudolf.second);

            rudolf.first += dxr;
            rudolf.second += dyr;
            board[prev.first][prev.second] = 0;

            if (rudolf.first == santas[closestIdx].r && rudolf.second == santas[closestIdx].c) {
                int fx = rudolf.first + dxr * C;
                int fy = rudolf.second + dyr * C;
                int lx = fx, ly = fy;

                santas[closestIdx].faint = t + 1;

                while (is_inrange(lx, ly) && board[lx][ly] > 0) {
                    lx += dxr;
                    ly += dyr;
                }

                while (!(lx == fx && ly == fy)) {
                    int bx = lx - dxr;
                    int by = ly - dyr;
                    int idx = board[bx][by];

                    if (!is_inrange(lx, ly)) {
                        santas[idx].is_out = true;
                    } else {
                        board[lx][ly] = idx;
                        santas[idx].r = lx;
                        santas[idx].c = ly;
                    }

                    lx = bx;
                    ly = by;
                }

                points[closestIdx] += C;
                board[santas[closestIdx].r][santas[closestIdx].c] = 0;
                santas[closestIdx].r = fx;
                santas[closestIdx].c = fy;

                if (is_inrange(fx, fy)) {
                    board[fx][fy] = closestIdx;
                } else {
                    santas[closestIdx].is_out = true;
                }
            }
        }

        board[rudolf.first][rudolf.second] = -1;

        // 산타 이동
        for (int i = 1; i <= P; i++) {
            if (santas[i].is_out || santas[i].faint >= t) continue;

            int cx = santas[i].r;
            int cy = santas[i].c;
            int minDist = (cx - rudolf.first) * (cx - rudolf.first) + (cy - rudolf.second) * (cy - rudolf.second);
            int dir = -1;

            for (int d = 0; d < 4; d++) {
                int nx = cx + dx[d];
                int ny = cy + dy[d];

                if (!is_inrange(nx, ny) || board[nx][ny] > 0) continue;

                int dist = (nx - rudolf.first) * (nx - rudolf.first) + (ny - rudolf.second) * (ny - rudolf.second);
                if (dist < minDist) {
                    minDist = dist;
                    dir = d;
                }
            }

            if (dir != -1) {
                int nx = cx + dx[dir];
                int ny = cy + dy[dir];

                if (nx == rudolf.first && ny == rudolf.second) {
                    santas[i].faint = t + 1;
                    int rx = -dx[dir], ry = -dy[dir];
                    int fx = nx + rx * D;
                    int fy = ny + ry * D;
                    int lx = fx, ly = fy;

                    while (is_inrange(lx, ly) && board[lx][ly] > 0) {
                        lx += rx;
                        ly += ry;
                    }

                    while (!(lx == fx && ly == fy)) {
                        int bx = lx - rx;
                        int by = ly - ry;
                        int idx = board[bx][by];

                        if (!is_inrange(lx, ly)) {
                            santas[idx].is_out = true;
                        } else {
                            board[lx][ly] = idx;
                            santas[idx].r = lx;
                            santas[idx].c = ly;
                        }

                        lx = bx;
                        ly = by;
                    }

                    points[i] += D;
                    board[cx][cy] = 0;
                    santas[i].r = fx;
                    santas[i].c = fy;

                    if (is_inrange(fx, fy)) {
                        board[fx][fy] = i;
                    } else {
                        santas[i].is_out = true;
                    }
                } else {
                    board[cx][cy] = 0;
                    santas[i].r = nx;
                    santas[i].c = ny;
                    board[nx][ny] = i;
                }
            }
        }

        // 살아있는 산타 점수 +1
        for (int i = 1; i <= P; i++) {
            if (!santas[i].is_out) {
                points[i]++;
            }
        }
    }

    for (int i = 1; i <= P; i++) {
        cout << points[i] << " ";
    }

    return 0;
}
