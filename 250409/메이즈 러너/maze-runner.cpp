#include <iostream>
#include <utility>

using namespace std;
using pii = pair<int, int>;
/*
N: 미로 크기, M: 참가자 수, K; 게임 시간

N x N 
- 0 : 빈 칸
- 1 ~ 9 : 내구도

모든 참가자들의 이동 거리 합과 출구 좌표 출력
*/

int N, M, K;
int ans;
int board[15][15];
int next_board[15][15];
pair<int, int> runners[11];
pair<int, int> exits;

int sx, sy, square_size;	// 회전해야 하는 최소 정사각형 기록

// 모든 참가자 이동
void mvRunners() {
	for (int i = 0; i < M; i++) {
		// 이미 출구인 경우 스킵
		if (runners[i] == exits) continue;

		int x = runners[i].first;
		int y = runners[i].second;

		// 행이 다른 경우 이동
		if (x != exits.first) {
			int nx = x;
			if (x < exits.first) nx = x + 1;
			else nx = x - 1;

			// 벽이 없다면 행 이동 가능 -> 이동 후 다음 참가자로 넘어감
			if (!board[nx][y]) {
				runners[i] = make_pair(nx, y);
				ans++;
				continue;
			}
		}

		// 열이 다른 경우 이동
		if (y != exits.second) {
			int ny = y;
			if (y < exits.second) ny = y + 1;
			else ny = y - 1;

			if (!board[x][ny]) {
				runners[i] = make_pair(x, ny);
				ans++;
				continue;
			}
		}
	}
}

// 한 명 이상의 참가자와 출구 포함 가장 작은 정사각형
void findSqure() {
	// 가장 작은 정사각형 ~ 모든 정사각형 만들어보기
	for (int sz = 2; sz <= N; sz++) {
		// 가장 좌상단 r 좌표가 작은 것부터
		for (int x1 = 0; x1 <= N - sz; x1++) {
			// 가장 좌상단 c 좌표가 작은 것부터
			for (int y1 = 0; y1 <= N - sz; y1++) {
				int x2 = x1 + sz - 1;
				int y2 = y1 + sz - 1;

				// 만약 출구가 해당 정사각형 안에 없으면 스킵
				if (!(x1 <= exits.first && exits.first <= x2 &&
					y1 <= exits.second && exits.second <= y2)) {
					continue;
				}

				// 한 명 이상의 참가자가 해당 정사각형에 있는지 확인
				bool is_runner = false;
				for (int i = 0; i < M; i++) {
					int rx = runners[i].first;
					int ry = runners[i].second;
					// 출구에 있는 참가자는 패스
					if (x1 <= rx && rx <= x2 && y1 <= ry && ry <= y2 &&
						runners[i] != exits) {
						is_runner = true;
						break;
					}
				}
				// 만약 한 명 이상의 참가자 있으면 sx, sy, squre_size 갱신 후 종료
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

// 정사각형 내부 벽 회전
void rotateBoard() {
	// 1. 정사각형 내부 내구도 1 감소
	for (int x = sx; x < sx + square_size; x++) {
		for (int y = sy; y < sy + square_size; y++) {
			if (board[x][y]) board[x][y]--;
		}
	}

	// 2. 시계 방향 90도 회전
	for (int x = sx; x < sx + square_size; x++) {
		for (int y = sy; y < sy + square_size; y++) {
			// step1. (sx,sy)를 (0,0)으로 옮김
			int ox = x - sx;
			int oy = y - sy;
			// step2. 변환 된 상태에서 회전 이후 좌표 (x, y) -> (y, squre_n - x - 1)
			int rx = oy;
			int ry = square_size - ox - 1;
			next_board[rx + sx][ry + sy] = board[x][y];
		}
	}

	for (int x = sx; x < sx + square_size; x++) {
		for (int y = sy; y < sy + square_size; y++) {
			board[x][y] = next_board[x][y];
		}
	}
}

// 모든 참가자 및 출구 회전
void rotateAll() {
	for (int i = 0; i < M; i++) {
		int x = runners[i].first;
		int y = runners[i].second;

		// 해당 참가자가 정사각형 안에 포함되어 있을 경우만 회전
		if (sx <= x && x < sx + square_size && sy <= y && y < sy + square_size) {
			// step1. (sx, sy) -> (0,0) 옮김
			int ox = x - sx;
			int oy = y - sy;
			// step2. 변환 된 상태에서 회전 이후 좌표 (x, y) -> (y, squre_n - x - 1)
			int rx = oy;
			int ry = square_size - ox - 1;
			// step3. 다시 (sx, sy) 더해줌
			runners[i] = make_pair(rx + sx, ry + sy);
		}
	}

	// 출구도 회전 진행
	int x = exits.first;
	int y = exits.second;

	if (sx <= x && x < sx + square_size && sy <= y && y < sy + square_size) {
		// step1. (sx, sy) -> (0,0) 옮김
		int ox = x - sx;
		int oy = y - sy;
		// step2. 변환 된 상태에서 회전 이후 좌표 (x, y) -> (y, squre_n - x - 1)
		int rx = oy;
		int ry = square_size - ox - 1;
		// step3. 다시 (sx, sy) 더해줌
		exits = make_pair(rx + sx, ry + sy);
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M >> K;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
		}
	}

	for (int i = 0; i < M; i++) {
		cin >> runners[i].first >> runners[i].second;
		runners[i].first--; runners[i].second--;
	}

	cin >> exits.first >> exits.second;
	exits.first--; exits.second--;

	while (K--) {
		// 모든 참가자 이동
		mvRunners();

		// 모든 사람이 출구로 탈출했는지 판단
		bool all_escaped = true;
		for (int i = 0; i < M; i++) {
			if (runners[i] != exits) {
				all_escaped = false;
				break;
			}
		}
		// 탈출 했으면 바로 종료
		if (all_escaped) break;

		// 한 명 이상의 참가자와 출구 포함한 미니 정사각형 찾기
		findSqure();

		// 정사각형 내부 벽 회전
		rotateBoard();

		// 모든 참가자 및 출구 회전
		rotateAll();
	}
	cout << ans << '\n';
	cout << exits.first + 1 << ' ' << exits.second + 1 << '\n';
	return 0;
}