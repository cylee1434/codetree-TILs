#include <bits/stdc++.h>
#define fastio ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;

int N, M, K, ans;
int board[10][10];
int next_board[10][10];

pair<int, int> runners[11];
pair<int, int> exits;

int sx, sy, square_size;

void mvRunners() {
    for (int i = 0; i < M; i++) {
        if (runners[i] == exits) continue;

        int x = runners[i].first;
        int y = runners[i].second;

        // 행 이동
        if (x != exits.first) {
            int nx = x + (x < exits.first ? 1 : -1);
            if (!board[nx][y]) {
                runners[i] = make_pair(nx, y);
                ans++;
                continue;
            }
        }

        // 열 이동
        if (y != exits.second) {
            int ny = y + (y < exits.second ? 1 : -1);
            if (!board[x][ny]) {
                runners[i] = make_pair(x, ny);
                ans++;
                continue;
            }
        }
    }
}

void findSquare() {
    for (int sz = 2; sz <= N; sz++) {
        for (int x1 = 0; x1 <= N - sz; x1++) {
            for (int y1 = 0; y1 <= N - sz; y1++) {
                int x2 = x1 + sz - 1;
                int y2 = y1 + sz - 1;

                if (!(x1 <= exits.first && exits.first <= x2 &&
                      y1 <= exits.second && exits.second <= y2)) {
                    continue;
                }

                bool is_runner = false;
                for (int i = 0; i < M; i++) {
                    int rx = runners[i].first;
                    int ry = runners[i].second;
                    if (x1 <= rx && rx <= x2 && y1 <= ry && ry <= y2 &&
                        runners[i] != exits) {
                        is_runner = true;
                        break;
                    }
                }

                if (is_runner) {
                    sx = x1;
                    sy = y1;
                    square_size = sz;
                    return;
                }
            }
        }
    }
}

void rotateBoard() {
    for (int x = sx; x < sx + square_size; x++) {
        for (int y = sy; y < sy + square_size; y++) {
            if (board[x][y]) board[x][y]--;
        }
    }

    for (int x = sx; x < sx + square_size; x++) {
        for (int y = sy; y < sy + square_size; y++) {
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = square_size - ox - 1;
            next_board[rx + sx][ry + sy] = board[x][y];
        }
    }

    for (int x = sx; x < sx + square_size; x++) {
        for (int y = sy; y < sy + square_size; y++) {
            board[x][y] = next_board[x][y];
        }
    }
}

void rotateAll() {
    for (int i = 0; i < M; i++) {
        int x = runners[i].first;
        int y = runners[i].second;

        if (sx <= x && x < sx + square_size && sy <= y && y < sy + square_size) {
            int ox = x - sx, oy = y - sy;
            int rx = oy, ry = square_size - ox - 1;
            runners[i] = make_pair(rx + sx, ry + sy);
        }
    }

    int x = exits.first;
    int y = exits.second;

    if (sx <= x && x < sx + square_size && sy <= y && y < sy + square_size) {
        int ox = x - sx, oy = y - sy;
        int rx = oy, ry = square_size - ox - 1;
        exits = make_pair(rx + sx, ry + sy);
    }
}

int main() {
    fastio;
    cin >> N >> M >> K;

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> board[i][j];

    for (int i = 0; i < M; i++) {
        cin >> runners[i].first >> runners[i].second;
        runners[i].first--; runners[i].second--;
    }

    cin >> exits.first >> exits.second;
    exits.first--; exits.second--;

    while (K--) {
        mvRunners();

        bool all_escaped = true;
        for (int i = 0; i < M; i++) {
            if (runners[i] != exits) {
                all_escaped = false;
                break;
            }
        }
        if (all_escaped) break;

        findSquare();
        rotateBoard();
        rotateAll();
    }

    cout << ans << '\n';
    cout << exits.first + 1 << ' ' << exits.second + 1 << '\n';
}
