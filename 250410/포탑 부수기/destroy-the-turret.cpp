#include <bits/stdc++.h>
#define fastio ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
using namespace std;

const int INF = 1e9;
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};
int dx8[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
int dy8[8] = {-1, 0, 1, 1, 1, 0, -1, -1};

int N, M, K;
int board[10][10], last_attack[10][10];

bool inRange(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < M;
}

pair<int, int> select_attacker() {
    pair<int, int> res = {-1, -1};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (board[i][j] <= 0) continue;
            if (res == make_pair(-1, -1)) { // res가 아직 초기 상태일 때
                res = {i, j}; continue;     // 처음 발견한 유효 좌표 저장
            }
            int x1 = res.first, y1 = res.second;
            int x2 = i, y2 = j;

            if (board[x2][y2] < board[x1][y1]) res = {x2, y2};
            else if (board[x1][y1] == board[x2][y2]) {
                if (last_attack[x2][y2] > last_attack[x1][y1]) res = {x2, y2};
                else if (last_attack[x2][y2] == last_attack[x1][y1]) {
                    if (x1 + y1 < x2 + y2) res = {x2, y2};
                    else if (x1 + y1 == x2 + y2 && y1 < y2) res = {x2, y2};
                }
            }
        }
    }
    return res;
}

pair<int, int> select_victim(pair<int, int> attacker) {
    pair<int, int> res = {-1, -1};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (board[i][j] <= 0 || make_pair(i, j) == attacker) continue;
            if (res == make_pair(-1, -1)) {
                res = {i, j}; continue;
            }
            int x1 = res.first, y1 = res.second;
            int x2 = i, y2 = j;

            if (board[x2][y2] > board[x1][y1]) res = {x2, y2};
            else if (board[x1][y1] == board[x2][y2]) {
                if (last_attack[x2][y2] < last_attack[x1][y1]) res = {x2, y2};
                else if (last_attack[x2][y2] == last_attack[x1][y1]) {
                    if (x1 + y1 > x2 + y2) res = {x2, y2};
                    else if (x1 + y1 == x2 + y2 && y1 > y2) res = {x2, y2};
                }
            }
        }
    }
    return res;
}

bool laser_attack(pair<int, int> a, pair<int, int> v, bool damaged[10][10]) {
    queue<pair<int, int>> q;
    pair<int, int> prev[10][10];
    bool visited[10][10] = {};
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            prev[i][j] = {-1, -1};  // (i,j) 오기 전 이전 좌표 저장 역할, {-1,-1}은 방문 전 or 이전 경로 없음 의미

    q.push(a);
    visited[a.first][a.second] = true;
    prev[a.first][a.second] = a;

    while (!q.empty()) {
        int x = q.front().first, y = q.front().second; q.pop();
        if (make_pair(x, y) == v) break;
        for (int d = 0; d < 4; d++) {
            int nx = (x + dx[d] + N) % N;
            int ny = (y + dy[d] + M) % M;
            if (board[nx][ny] <= 0 || visited[nx][ny]) continue;
            visited[nx][ny] = true;
            prev[nx][ny] = {x, y};
            q.push({nx, ny});
        }
    }

    if (prev[v.first][v.second].first == -1) return false;
    int dmg = board[a.first][a.second];
    pair<int, int> cur = v;

    while (cur != a) {
        if (cur != v) board[cur.first][cur.second] -= dmg / 2;
        damaged[cur.first][cur.second] = true;
        cur = prev[cur.first][cur.second];
    }
    
    board[v.first][v.second] -= dmg;
    damaged[v.first][v.second] = true;
    damaged[a.first][a.second] = true;
    return true;
}

void bomb_attack(pair<int, int> a, pair<int, int> v, bool damaged[10][10]) {
    int dmg = board[a.first][a.second];
    for (int d = 0; d < 8; d++) {
        int nx = (v.first + dx8[d] + N) % N;
        int ny = (v.second + dy8[d] + M) % M;
        if (make_pair(nx, ny) == a || board[nx][ny] <= 0) continue;
        board[nx][ny] -= dmg / 2;
        damaged[nx][ny] = true;
    }
    board[v.first][v.second] -= dmg;
    damaged[v.first][v.second] = true;
    damaged[a.first][a.second] = true;
}

void repair(bool damaged[10][10]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (board[i][j] > 0 && !damaged[i][j])
                board[i][j] += 1;
        }
    }
}

bool check_end() {
    int alive = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (board[i][j] > 0) alive++;
    return alive <= 1;  // 살아있는 포탑이 1개 이하이면 게임 종료(true)
}

int main() {
    fastio
    cin >> N >> M >> K;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            cin >> board[i][j];

    for (int turn = 1; turn <= K; turn++) {
        if (check_end()) break;
        pair<int, int> attacker = select_attacker();
        pair<int, int> victim = select_victim(attacker);
        //last_attack[attacker.first][attacker.second] = turn;
        board[attacker.first][attacker.second] += (N + M);

        bool damaged[10][10] = {};
        bool success = laser_attack(attacker, victim, damaged);
        if (!success)
            bomb_attack(attacker, victim, damaged);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                if (board[i][j] < 0) board[i][j] = 0;

        repair(damaged);
    }

    int ans = 0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            ans = max(ans, board[i][j]);
    cout << ans << '\n';
    return 0;
}