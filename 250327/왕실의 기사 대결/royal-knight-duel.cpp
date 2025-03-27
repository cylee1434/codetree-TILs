#include <bits/stdc++.h>
#define fastio ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

using namespace std;
using ll = long long;

int L, N, Q;
int board[40][40];
int bef_k[31];
int r[31], c[31], h[31], w[31], k[31];
int nr[31], nc[31];
int dmg[31];
bool is_moved[31];

// 상, 우, 하, 좌
int dx[4] = {-1, 0, 1, 0};
int dy[4] = {0, 1, 0, -1};

// 움직임 시도 
bool update(int idx, int dir) {
    for (int i = 1; i <= N; i++) {
        dmg[i] = 0;
        is_moved[i] = false;
        nr[i] = r[i];
        nc[i] = c[i];
    }

    queue<int> q;
    q.push(idx);
    is_moved[idx] = true;

    while (!q.empty()) {
        int x = q.front(); q.pop();

        nr[x] += dx[dir];
        nc[x] += dy[dir];

        // 경계 체크
        if (nr[x] < 1 || nr[x] + h[x] - 1 > L || nc[x] < 1 || nc[x] + w[x] -1 > L) {
            return false;
        }

        // 함정이나 다른 조각들과 충돌하는지 체크
        for (int i = nr[x]; i <= nr[x] + h[x] -1; i++) {
            for (int j = nc[x]; j <= nc[x] + w[x] - 1; j++) {
                if (board[i][j] == 1) 
                    dmg[x]++;
                if (board[i][j] == 2) 
                    return false;
            }
        }

        // 다른 조각과 충돌 시 연쇄 이동
        for(int i = 1; i <= N; i++) {
            if(is_moved[i] || k[i] <= 0) 
                continue;
            if(r[i] > nr[x] + h[x] - 1 || nr[x] > r[i] + h[i] - 1) 
                continue;
            if(c[i] > nc[x] + w[x] - 1 || nc[x] > c[i] + w[i] - 1) 
                continue;

            is_moved[i] = true;
            q.push(i);
        }
    }

    dmg[idx] = 0;
    return true;
}

// 특정 조각 지정된 방향으로 이동 
void canMove(int idx, int dir) {
    if (k[idx] <= 0) return;

    // 이동 가능 경우, 실제 위치와 체력 업데이트
    if (update(idx, dir)) {
        for (int i = 1; i <= N; i++) {
            r[i] = nr[i];
            c[i] = nc[i];
            k[i] -= dmg[i];
        }
    }
}

int main() {
    fastio;

    cin >> L >> N >> Q;
    for (int i = 1; i <= L; i++) {
        for (int j = 1; j <= L; j++) {
            cin >> board[i][j];
        }
    }

    for (int i = 1; i <= N; i++) {
        cin >> r[i] >> c[i] >> h[i] >> w[i] >> k[i];
        bef_k[i] = k[i];
    }

    for (int i = 1; i <= Q; i++) {
        int idx, dir;
        cin >> idx >> dir;
        canMove(idx, dir);
    }

    ll ans = 0; 
    for (int i = 1; i <= N; i++) {
        if (k[i] > 0) {
            ans += bef_k[i] - k[i];
        }
    }

    cout << ans;
    return 0;    
}