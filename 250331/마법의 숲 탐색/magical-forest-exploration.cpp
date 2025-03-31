#include <iostream>
#include <queue>
#include <algorithm>

#define MAX_L 70

using namespace std;

int R, C, K;
int A[80][80]; // [3 ~ R+2][0 ~ C-1]
bool isExit[80][80];
int answer = 0;

int dy[4] = {-1, 0, 1, 0}; // 북 동 남 서
int dx[4] = {0, 1, 0, -1};

bool inRange(int y, int x) {
    return (3 <= y && y < R + 3 && 0 <= x && x < C);
}

void resetMap() {
    for (int i = 0; i < R + 3; ++i) {
        for (int j = 0; j < C; ++j) {
            A[i][j] = 0;
            isExit[i][j] = false;
        }
    }
}

bool canGo(int y, int x) {
    if (!(0 <= x - 1 && x + 1 < C && y + 1 < R + 3)) return false;

    return A[y - 1][x - 1] == 0 &&
           A[y - 1][x] == 0 &&
           A[y - 1][x + 1] == 0 &&
           A[y][x - 1] == 0 &&
           A[y][x] == 0 &&
           A[y][x + 1] == 0 &&
           A[y + 1][x] == 0;
}

int bfs(int y, int x) {
    bool visit[MAX_L + 3][MAX_L] = {};
    queue<pair<int, int>> q;

    int result = y;
    q.push({y, x});
    visit[y][x] = true;

    while (!q.empty()) {
        int cy = q.front().first;
        int cx = q.front().second;
        q.pop();

        for (int d = 0; d < 4; ++d) {
            int ny = cy + dy[d];
            int nx = cx + dx[d];
            if (inRange(ny, nx) && !visit[ny][nx]) {
                if (A[ny][nx] == A[cy][cx] || (A[ny][nx] != 0 && isExit[cy][cx])) {
                    visit[ny][nx] = true;
                    q.push({ny, nx});
                    result = max(result, ny);
                }
            }
        }
    }

    return result;
}

void down(int y, int x, int d, int id) {
    if (canGo(y + 1, x)) {
        down(y + 1, x, d, id);
    }
    else if (canGo(y + 1, x - 1)) {
        down(y + 1, x - 1, (d + 3) % 4, id); // 반시계
    }
    else if (canGo(y + 1, x + 1)) {
        down(y + 1, x + 1, (d + 1) % 4, id); // 시계
    }
    else {
        if (!inRange(y - 1, x - 1) || !inRange(y + 1, x + 1)) {
            resetMap();
        } else {
            A[y][x] = id;
            for (int k = 0; k < 4; ++k) {
                A[y + dy[k]][x + dx[k]] = id;
            }
            isExit[y + dy[d]][x + dx[d]] = true;
            answer += bfs(y, x) - 3 + 1;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> R >> C >> K;
    for (int id = 1; id <= K; ++id) {
        int x, d;
        cin >> x >> d;
        --x;
        down(0, x, d, id);
    }

    cout << answer << '\n';
    return 0;
}
