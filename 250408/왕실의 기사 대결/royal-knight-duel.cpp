#include <iostream>
#include <queue>
using namespace std;
using ll = long long;

/*
L x L 체스판 
- 0 : 빈칸
- 1 : 함정
- 2 : 벽

초기 기사들 정보
- r : 체스판 크기
- n : 기사의 수
- q : 명령의 수
- k : 기사 체력

생존한 기사들이 총 받은 데미지 합 출력
*/

int L, N, Q;
int board[41][41];
int r[31], c[31], h[31], w[31], k[31], dmg[31];

int bef_k[31];
int nr[31], nc[31];
bool is_moved[31];

// 상우하좌
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
		int cur = q.front();
		q.pop();

		nr[cur] += dx[dir];
		nc[cur] += dy[dir];

		if (nr[cur] < 1 || nr[cur] + h[cur] - 1 > L || nc[cur] < 1 || nc[cur] + w[cur] - 1 > L) {
			return false;
		}

		// 함정이나 다른 기사들과 충돌하는지 체크
		for (int i = nr[cur]; i <= nr[cur] + h[cur] - 1; i++) {
			for (int j = nc[cur]; j <= nc[cur] + w[cur] - 1; j++) {
				if (board[i][j] == 1) dmg[cur]++;
				if (board[i][j] == 2) return false;
			}
		}

		// 다른 기사와 충돌 시 연쇄 이동
		for (int i = 1; i <= N; i++) {
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

	// 상: 0, 우: 1, 하:2, 좌:3
	// 이동 가능한 경우, 실제 위치와 체력 업데이트
	if (update(idx, dir)) {
		for (int i = 1; i <= N; i++) {
			r[i] = nr[i];
			c[i] = nc[i];
			k[i] -= dmg[i];
		}
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

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

	int ans = 0;
	for (int i = 1; i <= N; i++) {
		if (k[i] > 0) {
			ans += bef_k[i] - k[i];
		}
	}
	cout << ans;
	return 0;
}