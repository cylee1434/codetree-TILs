#include <bits/stdc++.h>
#define fastio ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int N, M, K;
int miro[10][10];
int mv_time = 0;

pair<int, int> exit_pos;
vector<pair<int, int>> runners;

// 출구 기준 거리 계산용 BFS
pair<int, int> getNextMove(pair<int, int> start) {
    queue<pair<int, int>> q;
    bool visited[10][10] = {};
    pair<int, int> from[10][10];
    int dist[10][10] = {};

    q.push(start);
    visited[start.first][start.second] = true;
    from[start.first][start.second] = {-1, -1};

    while (!q.empty()) {
        pair<int, int> cur = q.front(); q.pop();
        int x = cur.first, y = cur.second;

        if (x == exit_pos.first && y == exit_pos.second) {
            pair<int, int> now = cur;
            while (from[now.first][now.second] != start) {
                now = from[now.first][now.second];
            }
            return now;
        }

        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if (visited[nx][ny]) continue;
            if (miro[nx][ny] >= 1 && miro[nx][ny] <= 9) continue;

            visited[nx][ny] = true;
            from[nx][ny] = {x, y};
            dist[nx][ny] = dist[x][y] + 1;
            q.push({nx, ny});
        }
    }
    return start; // 이동 불가
}

int getDistance() {
    int total = 0;
    for (int i = 0; i < runners.size(); i++) {
        pair<int, int> r = runners[i];
        queue<pair<int, int>> q;
        bool visited[10][10] = {};
        int dist[10][10] = {};

        q.push(r);
        visited[r.first][r.second] = true;

        while (!q.empty()) {
            pair<int, int> cur = q.front(); q.pop();
            int x = cur.first, y = cur.second;

            if (x == exit_pos.first && y == exit_pos.second) {
                total += dist[x][y];
                break;
            }

            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
                if (visited[nx][ny]) continue;
                if (miro[nx][ny] >= 1 && miro[nx][ny] <= 9) continue;

                visited[nx][ny] = true;
                dist[nx][ny] = dist[x][y] + 1;
                q.push({nx, ny});
            }
        }
    }
    return total;
}

void moveRunners() {
    vector<pair<int, int>> updated;
    for (int i = 0; i < runners.size(); i++) {
        pair<int, int> r = runners[i];
        if (r == exit_pos) continue;
        pair<int, int> nxt = getNextMove(r);
        if (nxt == exit_pos) continue;
        updated.push_back(nxt);
    }
    runners = updated;
}

// 회전 가능한 가장 작은 정사각형 영역 찾기
pair<int, int> findTopLeft(int size) {
    for (int i = 0; i <= N - size; i++) {
        for (int j = 0; j <= N - size; j++) {
            for (int x = i; x < i + size; x++) {
                for (int y = j; y < j + size; y++) {
                    if (make_pair(x, y) == exit_pos) return {i, j};
                    for (int k = 0; k < runners.size(); k++) {
                        if (runners[k] == make_pair(x, y)) return {i, j};
                    }
                }
            }
        }
    }
    return {-1, -1};
}

void rotateSquare(int sx, int sy, int size) {
    int tmp[10][10];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            tmp[j][size - 1 - i] = miro[sx + i][sy + j];
        }
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            miro[sx + i][sy + j] = tmp[i][j];
            if (miro[sx + i][sy + j] >= 1) miro[sx + i][sy + j]--;
        }
    }
    // 러너, 출구 회전
    for (int i = 0; i < runners.size(); i++) {
        int x = runners[i].first, y = runners[i].second;
        if (x >= sx && x < sx + size && y >= sy && y < sy + size) {
            int nx = sx + (y - sy);
            int ny = sy + (size - 1 - (x - sx));
            runners[i] = {nx, ny};
        }
    }
    int x = exit_pos.first, y = exit_pos.second;
    if (x >= sx && x < sx + size && y >= sy && y < sy + size) {
        int nx = sx + (y - sy);
        int ny = sy + (size - 1 - (x - sx));
        exit_pos = {nx, ny};
    }
}

void rotate() {
    for (int size = 1; size <= N; size++) {
        pair<int, int> topleft = findTopLeft(size);
        if (topleft.first != -1) {
            rotateSquare(topleft.first, topleft.second, size);
            break;
        }
    }
}

int main() {
    fastio;
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> miro[i][j];
        }
    }
    for (int i = 0; i < M; i++) {
        int a, b; cin >> a >> b;
        runners.push_back({a - 1, b - 1});
    }
    int ex1, ex2;
    cin >> ex1 >> ex2;
    exit_pos = {ex1 - 1, ex2 - 1};

    while (K--) {
        if (runners.empty()) break;
        mv_time += getDistance();
        moveRunners();
        rotate();
    }

    cout << mv_time << '\n';
    cout << exit_pos.first + 1 << ' ' << exit_pos.second + 1 << '\n';
}