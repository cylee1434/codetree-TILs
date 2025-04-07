#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;
typedef pair<int, int> pii;

const int INF = 1e9;

int board[55][55];
int dist[55][55];
pii warrior[305];
int sightMap[55][55];
int warrior_cnt[55][55];

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};


int cal_manhatten(const pii& a, const pii& b) {
	return abs(a.first - b.first) + abs(a.second - b.second);
}

void cal_dist(int st, int sy, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			dist[i][j] = board[i][j] ? INF : -1;
		}
	}

	queue<pii> q;
	q.push({ st, sy });
	dist[st][sy] = 0; // 시작 지점 거리는 0

	while (!q.empty()) {
		int cx = q.front().first;
		int cy = q.front().second;
		q.pop();

		for (int d = 0; d < 4; d++) {
			int nx = cx + dx[d];
			int ny = cy + dy[d];

			if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
			if (dist[nx][ny] != -1) continue;

			dist[nx][ny] = dist[cx][cy] + 1;
			q.push({ nx, ny });
		}
	}
}

// 시야 방향에 따른 함수 선언 (위, 아래, 왼쪽, 오른쪽)
int sightUp(int x, int y, int N, bool isTest);
int sightDown(int x, int y, int N, bool isTest);
int sightLeft(int x, int y, int N, bool isTest);
int sightRight(int x, int y, int N, bool isTest);

int bestSight(int x, int y, int n) {
	int maxCover = 0; // 최대 커버리지 저장 변수
	int bestDir = -1; // 최적의 시야 방향

	// 0: 상, 1: 하, 2: 좌, 3: 우
	for (int d = 0; d < 4; d++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				sightMap[i][j] = 0;
			}
		}

		int coverage = 0;

		// 방향에 따른 시야 계산 함수 호출
		if (d == 0) {
			coverage = sightUp(x, y, n, true);
		}
		else if (d == 1) {
			coverage = sightDown(x, y, n, true);
		}
		else if (d == 2) {
			coverage = sightLeft(x, y, n, true);
		}
		else if (d == 3) {
			coverage = sightRight(x, y, n, true);
		}

		if (maxCover < coverage) {
			maxCover = coverage;
			bestDir = d;
		}
	}

	// 시야 맵 초기화
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			sightMap[i][j] = 0;
		}
	}

	if (bestDir == 0) {
		sightUp(x, y, n, false);
	}
	else if (bestDir == 1) {
		sightDown(x, y, n, false);
	}
	else if (bestDir == 2) {
		sightLeft(x, y, n, false);
	}
	else if (bestDir == 3) {
		sightRight(x, y, n, false);
	}

	return maxCover;
}


int sightUp(int x, int y, int n, bool isTest) {
	for (int i = x - 1; i >= 0; i--) {
		int left = max(0, y - (x - i));
		int right = min(n - 1, y + (x - i));
		for (int j = left; j <= right; j++) {
			sightMap[i][j] = 1;
		}
	}

	// 장애물 처리: 시야 막힘 여부 확인
	bool obstacle = false;
	for (int i = x - 1; i >= 0; i--) {
		if (obstacle) {
			sightMap[i][y] = 0;
		}
		else {
			sightMap[i][y] = 1;
		}
		if (warrior_cnt[i][y]) {	// 시야에 전사가 있는 경우 장애물로 간주
			obstacle = true;
		}
	}

	// 장애물에 따라 시야 조정
	for (int i = x - 1; i >= 1; i--) {
		int left = max(0, y - (x - i));
		int right = min(n - 1, y + (x - i));

		// 왼쪽 측면 조정
		for (int j = left; j < y; j++) {
			if (!sightMap[i][j] || warrior_cnt[i][j]) {
				if (j > 0) sightMap[i - 1][j - 1] = 0;	// 왼쪽 위 셀의 시야 제거
				sightMap[i - 1][j] = 0;
			}
		}

		// 오른쪽 측면 조정
		for (int j = y + 1; j <= right; j++) {
			if (!sightMap[i][j] || warrior_cnt[i][j]) {
				if (j + 1 < n) sightMap[i - 1][j + 1] = 0;
				sightMap[i - 1][j] = 0;
			}
		}
	}

	// 시야로 커버 된 전사 수 계산
	int coverage = 0;
	for (int i = x - 1; i >= 0; i--) {
		int left = max(0, y - (x - i));
		int right = min(n - 1, y + (x - i));
		for (int j = left; j <= right; j++) {
			if (sightMap[i][j]) {
				coverage += warrior_cnt[i][j];	// 시야 내에 전사 수 더함
			}
		}
	}

	// 테스트 모드인 경우 원래대로 맵 되돌림
	if (isTest) {
		for (int i = x - 1; i >= 0; i--) {
			int left = max(0, y - (x - i));
			int right = min(n - 1, y + (x - i));
			for (int j = left; j <= right; j++) {
				sightMap[i][j] = 0;
			}
		}
	}
	return coverage;
}

int sightDown(int x, int y, int n, bool isTest) {
	for (int i = x + 1; i < n; i++) {
		int left = max(0, y - (i - x));
		int right = min(n - 1, y + (i - x));
		for (int j = left; j <= right; j++) {
			sightMap[i][j] = 1;
		}
	}

	// 장애물 처리
	bool obstacle = false;
	for (int i = x + 1; i < n; i++) {
		if (obstacle) {
			sightMap[i][y] = 0;
		}
		else {
			sightMap[i][y] = 1;
		}
		if (warrior_cnt[i][y]) {
			obstacle = true;
		}
	}

	// 장애물 따라 시야 조정
	for (int i = x + 1; i < n - 1; i++) {
		int left = max(0, y - (i - x));
		int right = min(n - 1, y + (i - x));
		
		// 왼쪽 측면 조정
		for (int j = left; j < y; j++) {
			if (!sightMap[i][j] || warrior_cnt[i][j]) {
				if (j > 0) sightMap[i + 1][j - 1] = 0;
				else {
					sightMap[i + 1][j] = 0;
				}
			}
		}

		// 오른쪽 측면 조정
		for (int j = y + 1; j <= right; j++) {
			if (!sightMap[i][j] || warrior_cnt[i][j]) {
				if (j + 1 < n) sightMap[i + 1][j + 1] = 0;
				else {
					sightMap[i + 1][j] = 0;
				}
			}
		}
	}
	// 시야로 커버 된 전사 수 계산
	int coverage = 0;
	for (int i = x + 1; i < n; i++) {
		int left = max(0, y - (i - x));
		int right = min(n - 1, y + (i - x));
		for (int j = left; j <= right; j++) {
			if (sightMap[i][j]) {
				coverage += warrior_cnt[i][j];
			}
		}
	}

	// 테스트 모드 원래대로 되돌림
	if (isTest) {
		for (int i = x + 1; i < n; i++) {
			int left = max(0, y - (i - x));
			int right = min(n - 1, y + (i - x));
			for (int j = left; j <= right; j++) {
				sightMap[i][j] = 0;
			}
		}
	}
	return coverage;
}

int sightLeft(int x, int y, int n, bool isTest) {
	for (int i = y - 1; i >= 0; i--) {
		int top = max(0, x - (y - i));
		int bottom = min(n - 1, x + (y - i));
		for (int j = top; j <= bottom; j++) {
			sightMap[j][i] = 1;
		}
	}

	// 장애물 처리
	bool obstacle = false;
	for (int i = y - 1; i >= 0; i--) {
		if (obstacle) {
			sightMap[x][i] = 0;
		}
		else {
			sightMap[x][i] = 1;
		}
		if (warrior_cnt[x][i]) {
			obstacle = true;
		}
	}

	// 장애물에 따라 시야 조정
	for (int i = y - 1; i > 0; i--) {
		int top = max(0, x - (y - i));
		int bottom = min(n - 1, x + (y - i));
		
		// 상단 측면 조정
		for (int j = top; j < x; j++) {
			if (!sightMap[j][i] || warrior_cnt[j][i]) {
				if (j > 0) sightMap[j - 1][i - 1] = 0;
				else {
					sightMap[j][i - 1] = 0;
				}
			}
		}

		// 하단 측면 조정
		for (int j = x+1; j <= bottom; j++) {
			if (!sightMap[j][i] || warrior_cnt[j][i]) {
				if (j + 1 < n) sightMap[j + 1][i - 1] = 0;
				else sightMap[j][i - 1] = 0;
			}
		}
	}

	// 시야로 커버 된 전사 수 계산
	int coverage = 0;
	for (int i = y - 1; i >= 0; i--) {
		int top = max(0, x - (y - i));             // 상단 경계 계산
		int bottom = min(n - 1, x + (y - i));      // 하단 경계 계산
		for (int j = top; j <= bottom; j++) {
			if (sightMap[j][i]) {
				coverage += warrior_cnt[j][i]; // 시야 내에 있는 전사의 수를 더함
			}
		}
	}

	// 테스트 모드인 경우 시야 맵을 원래대로 되돌림
	if (isTest) {
		for (int i = y - 1; i >= 0; i--) {
			int top = max(0, x - (y - i));
			int bottom = min(n - 1, x + (y - i));
			for (int j = top; j <= bottom; j++) {
				sightMap[j][i] = 0;                   // 시야 제거
			}
		}
	}

	return coverage;    // 커버리지 반환
}

int sightRight(int x, int y, int n, bool isTest) {
	for (int i = y + 1; i < n; i++) {
		int top = max(0, x - (i - y));            // 상단 경계 계산
		int bottom = min(n - 1, x + (i - y));     // 하단 경계 계산
		for (int j = top; j <= bottom; j++) {
			sightMap[j][i] = 1;                    // 시야 설정
		}
	}

	// 장애물 처리: 시야 막힘 여부 확인
	bool obstacle = false;
	for (int i = y + 1; i < n; i++) {
		if (obstacle) {
			sightMap[x][i] = 0;                    // 장애물이 발견된 후에는 시야 제거
		}
		else {
			sightMap[x][i] = 1;                    // 장애물이 발견되지 않으면 시야 유지
		}

		if (warrior_cnt[x][i]) {               // 전사가 있는 경우 장애물로 간주
			obstacle = true;
		}
	}

	// 장애물에 따라 시야 조정
	for (int i = y + 1; i < n - 1; i++) {
		int top = max(0, x - (i - y));             // 상단 경계 계산
		int bottom = min(n - 1, x + (i - y));      // 하단 경계 계산

		// 상단 측면 조정
		for (int j = top; j < x; j++) {
			if ((!sightMap[j][i]) || warrior_cnt[j][i]) {
				if (j > 0) sightMap[j - 1][i + 1] = 0; 
				sightMap[j][i + 1] = 0;           
			}
		}

		// 하단 측면 조정
		for (int j = x + 1; j <= bottom; j++) {
			if ((!sightMap[j][i]) || warrior_cnt[j][i]) {
				if (j + 1 < n) sightMap[j + 1][i + 1] = 0; 
				sightMap[j][i + 1] = 0;                
			}
		}
	}

	// 시야로 커버된 전사 수 계산
	int coverage = 0;
	for (int i = y + 1; i < n; i++) {
		int top = max(0, x - (i - y));             // 상단 경계 계산
		int bottom = min(n - 1, x + (i - y));      // 하단 경계 계산
		for (int j = top; j <= bottom; j++) {
			if (sightMap[j][i]) {
				coverage += warrior_cnt[j][i]; // 시야 내에 있는 전사의 수를 더함
			}
		}
	}

	// 테스트 모드인 경우 시야 맵을 원래대로 되돌림
	if (isTest) {
		for (int i = y + 1; i < n; i++) {
			int top = max(0, x - (i - y));
			int bottom = min(n - 1, x + (i - y));
			for (int j = top; j <= bottom; j++) {
				sightMap[j][i] = 0;                   // 시야 제거
			}
		}
	}

	return coverage;    // 커버리지 반환
}
/*
 * @brief 플레이어를 향해 전사들을 이동시키는 함수
 *
 * @param mx 메두사의 현재 x 좌표
 * @param my 메두사의 현재 y 좌표
 * @param N 그리드의 크기
 * @param M 전사의 수
 * @return pii (총 이동한 전사 수, 플레이어에게 도달한 전사 수)
 */
pii moveWarriors(int mx, int my, int n, int m) {
	int totalMoved = 0;		// 총 이동한 전사 수
	int totalHits = 0;		// 메두사에게 도달한 전사 수

	// 모든 전사에 대한 이동 처리
	for (int i = 0; i < m; i++) {
		if (warrior[i].first == -1) continue;

		int wx = warrior[i].first;
		int wy = warrior[i].second;

		// 시야 내 전사는 이동 안함
		if (sightMap[wx][wy]) continue;

		// 현재 메두사와의 거리 계산
		int curDist = cal_manhatten({ mx,my }, { wx, wy });
		bool hasMoved = false;	// 이동 여부 플래그

		// 첫번째 이동
		for (int d = 0; d < 4; d++) {
			int nx = wx + dx[d];
			int ny = wy + dy[d];

			// 이동할 위치가 그리드 내에 있고 시야 내에 있는지 확인
			if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
			if (sightMap[nx][ny]) continue;

			// 새로운 위치에서 거리 계산
			int newDist = cal_manhatten({ mx, my }, { nx, ny });
			if (newDist < curDist) {
				wx = nx;
				wy = ny;
				hasMoved = true;
				totalMoved++;
				break;	// 첫번째 이동 후 루프 탈출
			}
		}

		// 두번째 이동: 추가로 거리 줄일 수 있는지 확인
		if (hasMoved) {
			int newDist = cal_manhatten({ mx,my }, { wx, wy });
			for (int d = 0; d < 4; d++) {
				// 반대 방향 이동 시도
				int nx = wx + dx[(d + 2) % 4];
				int ny = wy + dy[(d + 2) % 4];

				// 이동할 위치가 그리드 내에 있고, 시야 내에 있는지 확인
				if (nx < 0 || nx >= n || ny < 0 || ny >= n) continue;
				if (sightMap[nx][ny]) continue;

				// 새로운 위치에서 거리 계산
				int futherDist = cal_manhatten({ mx, my }, { nx, ny });
				if (futherDist < newDist) {
					wx = nx;
					wy = ny;
					totalMoved++;
					break;
				}
			}
		}

		// 전사 위치 업데이트
		warrior[i] = { wx, wy };
	}

	// 메두사에 도달한 전사 수 계산
	for (int i = 0; i < m; i++) {
		if (warrior[i].first == -1) continue;
		if (warrior[i].first == mx && warrior[i].second == my) {
			totalHits++;
			warrior[i] = { -1,-1 };
		}
	}
	return { totalMoved, totalHits };
}

void updateWarrior(int n, int m) {
	// 전사 수 그리드 초기화
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			warrior_cnt[i][j] = 0;
		}
	}

	// 각 전사 위치 확인해서 보드에 반영
	for (int i = 0; i < m; i++) {
		if (warrior[i].first == -1) continue; // 이미 잡힌 전사는 패스
		int x = warrior[i].first;
		int y = warrior[i].second;
		warrior_cnt[x][y]++;
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N, M;
	cin >> N >> M;

	int stX, stY, enX, enY;
	cin >> stX >> stY >> enX >> enY;

	for (int i = 0; i < M; i++) {
		cin >> warrior[i].first >> warrior[i].second;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
		}
	}
	// 도착 지점~모든 지점까지 최단 거리 한 번에 구함 -> 전사들이 각각 도달 가능한지, 최소 거리 알 수 있음
	cal_dist(enX, enY, N);

	// 시작 지점이 종료 지점에 도달할 수 없는 경우 종료
	if (dist[stX][stY] == -1) {
		cout << "-1\n";
		return 0;
	}

	int mx = stX;	// 현재 메두사 x좌표
	int my = stY;	// 현재 메두사 y좌표

	// 메두사가 종료 지점에 도달할 때까지
	while (true) {
		bool moved = false; // 메두사 이동 여부

		for (int d = 0; d < 4; d++) {
			int nx = mx + dx[d];
			int ny = my + dy[d];
			if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
			if (dist[nx][ny] < dist[mx][my]) {
				mx = nx;
				my = ny;
				moved = true;
				break; // 이동 후 루프 탈출
			}
		}
		// 종료 지점 도달 시 종료
		if (mx == enX && my == enY) {
			cout << "0\n";
			return 0;
		}
		// 현재 위치에 있는 전사들 잡음
		for (int i = 0; i < M; i++) {
			if (warrior[i].first == mx && warrior[i].second == my) {
				warrior[i] = { -1, -1 }; // 잡힌 상태 표시
			}
		}

		// 현재 전사들 위치 기반 전사 수 보드 업데이트
		updateWarrior(N, M);

		// 최적의 시야 방향 선택 후 시야로 커버 된 전사 수 계산
		int sightCover = bestSight(mx, my, N);

		// 전사 이동 후, 이동한 전사 수와 메두사에게 도달한 전사 수 얻음
		pii war_result = moveWarriors(mx, my, N, M);

		cout << war_result.first << " " << sightCover << " " << war_result.second << '\n';
	}
	return 0;
}