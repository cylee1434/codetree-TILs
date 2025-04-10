#include <iostream>
#include <utility>
#include <queue>

using namespace std;
using pii = pair<int, int>;

int N, M, K;
int board[15][15];
int last_attack[15][15];


// 우 하 좌 상
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

bool inRange(int x, int y) {
	return x >= 0 && x < N && y >= 0 && y < M;
}

/*
- 공격자: 부서지지 않은 포탑 중 가장 약한 포탑
1. 공격력이 가장 낮은,
2. 2개 이상이라면 가장 최근에 공격한 포탑
3. 행과 열의 합이 가장 큰
4. 열 값이 가장 큰
*/
pii select_attacker() {
	pii res = { -1, -1 };
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] <= 0) continue;
			if (res == make_pair(-1, -1)) {	// res가 초기 상태일 때
				res = { i, j };				// 처음 발견한 유효 좌표 저장
				continue;
			}
			int x1 = res.first, y1 = res.second;
			int x2 = i, y2 = j;

			if (board[x2][y2] < board[x1][y1]) res = { x2, y2 };
			else if (board[x1][y1] == board[x2][y2]) {
				if (last_attack[x2][y2] > last_attack[x1][y1]) res = { x2, y2 };
				else if (last_attack[x2][y2] == last_attack[x1][y1]) {
					if (x1 + y1 < x2 + y2) res = { x2, y2 };
					else if (x1 + y1 == x2 + y2 && y1 > y2) res = { x2, y2 };
				}
			}
		}
	}
	return res;
}

/*
- 공격 대상: 자신을 제외한 가장 강한 포탑
1. 공격력이 가장 높은,
2. 2개 이상이라면 공격한지 가장 오래된 포탑
3. 행과 열의 합이 가장 작은
4. 열 값이 가장 작은
*/
pii select_victim(pii attacker) {
	pii res = { -1,-1 };
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] <= 0 || make_pair(i, j) == attacker) continue;
			if (res == make_pair(-1, -1)) {
				res = { i, j }; 
				continue;
			}
			int x1 = res.first, y1 = res.second;
			int x2 = i, y2 = j;

			if (board[x2][y2] > board[x1][y1]) res = { x2, y2 };
			else if (board[x1][y1] == board[x2][y2]) {
				if (last_attack[x2][y2] < last_attack[x1][y1]) res = { x2,y2 };
				else if (last_attack[x2][y2] == last_attack[x1][y1]) {
					if (x1 + y1 > x2 + y2) res = { x2, y2 };
					else if (x1 + y1 == x2 + y2 && y1 > y2) res = { x2, y2 };
				}
			}
		}
	}
	return res;
}

/*
- 레이저 공격
1. 상하좌우 4방향
2. 부서진 포탑 위치는 지나갈 수 없음
3. 가장 자리 -> 막힌 방향 진행 원하면, 반대편으로 나옴

- 공격자 위치 ~ 공격 대상까지 최단 경로 공격 (2개 이상이면 우/하/좌/상 순서)
공격 대상 값 -= 공격력만큼 피해, 피해를 입은 만큼 수치 줄어듬
공격 대상을 제외한 레이저 경로에 있는 포탑도 절반 만큼 피해 받음
(존재 하지 않으면, 포탄 공격 진행)
*/
bool laser_attack(pii attacker, pii victim, bool demaged[15][15]) {
	queue<pii> q;
	pii prev[15][15];
	bool visited[15][15] = {};
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			prev[i][j] = { -1, -1 };
		}
	}
	q.push(attacker);
	visited[attacker.first][attacker.second] = true;
	prev[attacker.first][attacker.second] = attacker;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (make_pair(cur.first, cur.second) == victim) break;
		for (int d = 0; d < 4; d++) {
			int nx = (cur.first + dx[d] + N) % N;
			int ny = (cur.second + dy[d] + M) % M;
			if (board[nx][ny] <= 0 || visited[nx][ny]) continue;
			visited[nx][ny] = true;
			prev[nx][ny] = { cur.first, cur.second };
			q.push({ nx, ny });
		}
	}

	if (prev[victim.first][victim.second].first == -1) return false;
	int dmg = board[attacker.first][attacker.second];
	pii cur_v = victim;

	while (cur_v != attacker) {
		if (cur_v != victim) board[cur_v.first][cur_v.second] -= dmg / 2;
		demaged[cur_v.first][cur_v.second] = true;
		cur_v = prev[cur_v.first][cur_v.second];
	}

	board[victim.first][victim.second] -= dmg;
	demaged[victim.first][victim.second] = true;
	demaged[attacker.first][attacker.second] = true;
	return true;
}

/*
- 포탄 공격
1. 공격 대상은 공격력 만큼 피해 받음
2. 주위 8개 방향은 절반 만큼 피해 받음
3. 가장 자리 -> 반대편 격자에 피해 받음
*/
void bomb_attack(pii attacker, pii victim, bool demaged[15][15]) {
	int dmg = board[attacker.first][attacker.second];

	int dxx[8] = {-1, 0, 1, 0, -1, -1, 1, 1};
	int dyy[8] = {0, -1, 0, 1, -1, 1, -1, 1};

	for (int d = 0; d < 8; d++) {
		int nx = (victim.first + dxx[d] + N) % N;
		int ny = (victim.second + dyy[d] + M) % M;
		if (board[nx][ny] <= 0 || make_pair(nx, ny) == attacker) continue;
		board[nx][ny] -= dmg / 2;
		demaged[nx][ny] = true;
	}

	board[victim.first][victim.second] -= dmg;
	demaged[victim.first][victim.second] = true;
	demaged[attacker.first][attacker.second] = true;
}

void repair(bool demaged[15][15]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] > 0 && !demaged[i][j]) {
				board[i][j] += 1;
			}
		}
	}
}

bool check_end() {
	int alive = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (board[i][j] > 0) alive++;
		}
	}
	return alive <= 1;	// 살아있는 포탑이 1개 이하면 게임 종료 (true)
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> board[i][j];
		}
	}

	for (int turn = 1; turn <= K; turn++) {
		if (check_end()) break;
		pii attacker = select_attacker();
		pii victim = select_victim(attacker);
		last_attack[attacker.first][attacker.second] = turn;
		board[attacker.first][attacker.second] += (N + M);

		bool demaged[15][15] = {};
		bool success = laser_attack(attacker, victim, demaged);
		if (!success) {
			bomb_attack(attacker, victim, demaged);
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (board[i][j] < 0) board[i][j] = 0;
			}
		}
		repair(demaged);
	}

	int ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			ans = max(ans, board[i][j]);
		}
	}
	cout << ans << '\n';
	return 0;

}
