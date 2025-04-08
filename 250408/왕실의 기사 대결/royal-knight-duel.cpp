#include <iostream>
#include <queue>

using namespace std;

int l, n, q;
int r[31], c[31], h[31], w[31], k[31];
int board[41][41];
int bef_k[31];
int dmg[31];
int nr[31], nc[31];
bool is_moved[31];

int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 };

bool update(int idx, int dir) {
	for (int i = 1; i <= n; i++) {
		dmg[i] = 0;
		is_moved[i] = false;
		nr[i] = r[i];
		nc[i] = c[i];
	}

	queue<int> q;
	q.push(idx);
	is_moved[idx] = true;

	while (!q.empty()) {
		int cur = q.front(); q.pop();

		nr[cur] += dx[dir];
		nc[cur] += dy[dir];

		if (nr[cur] < 1 || nr[cur] + h[cur] - 1 > l || nc[cur] < 1 || nc[cur] + w[cur] - 1 > l) return false;

		for (int i = nr[cur]; i <= nr[cur] + h[cur] - 1; i++) {
			for (int j = nc[cur]; j <= nc[cur] + w[cur] - 1; j++) {
				if (board[i][j] == 1) dmg[cur]++;
				if (board[i][j] == 2) return false;
			}
		}

		// 충돌 안하는 경우를 건너뜀
		for (int i = 1; i <= n; i++) {
			if (is_moved[i] || k[i] <= 0) continue;
			if (r[i] > nr[cur] + h[cur] - 1 || nr[cur] > r[i] + h[i] - 1) continue;
			if (c[i] > nc[cur] + w[cur] - 1 || nc[cur] > c[i] + w[i] - 1) continue;

			is_moved[i] = true;
			q.push(i);
		}
	}
	dmg[idx] = 0;
	return true;
}

void canMove(int idx, int dir) {
	if (k[idx] <= 0) return;

	if (update(idx, dir)) {
		for (int i = 1; i <= n; i++) {
			r[i] = nr[i];
			c[i] = nc[i];
			k[i] -= dmg[i];
		}

	}


}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> l >> n >> q;
	for (int i = 1; i <= l; i++) {
		for (int j = 1; j <= l; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 1; i <= n; i++) {
		cin >> r[i] >> c[i] >> h[i] >> w[i] >> k[i];
		bef_k[i] = k[i];
	}

	for (int i = 1; i <= q; i++) {
		int idx, dir;
		cin >> idx >> dir;
		canMove(idx, dir);
	}
	
	int ans = 0;
	for (int i = 1; i <= n; i++) {
		if (k[i] > 0) {
			ans += bef_k[i] - k[i];
		}
	}
	cout << ans;
	return 0;
}