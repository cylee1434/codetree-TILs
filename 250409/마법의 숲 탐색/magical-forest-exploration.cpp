#include <iostream>
#include <queue>
using namespace std;

int R, C, K;
int board[80][80];
bool is_exit[80][80]; 
int ans;

// 0 1 2 3 -> 북 동 남 서
int dy[4] = {-1, 0, 1, 0};
int dx[4] = {0, 1, 0, -1};

bool inRange(int y, int x) {
	return 3 <= y && y < R + 3 && 0 <= x && x < C;
}

void resetMap() {
	for (int i = 0; i < R + 3; i++) {
		for (int j = 0; j < C; j++) {
			board[i][j] = 0;
			is_exit[i][j] = false;
		}
	}
}

/*
골렘의 중심이 (y,x)이므로 이때 위치 범위와 (y-1,x) 위치 모두 범위 체크
*/
bool canGo(int y, int x) {
	if (!(0 <= x - 1 && x + 1 < C && y + 1 < R + 3)) return false;

	return board[y - 1][x - 1] == 0 &&
		board[y - 1][x] == 0 && board[y - 1][x + 1] == 0 &&
		board[y][x - 1] == 0 && board[y][x] == 0 &&
		board[y][x + 1] == 0 && board[y + 1][x] == 0;
}

// 정령이 내려갈 수 있는 최하단 행 반환
int bfs(int y, int x) {
	int mx_y = y;
	queue<pair<int, int>> q;
	bool visited[80][80];

	for (int i = 0; i < R + 3; i++) {
		for (int j = 0; j < C; j++) {
			visited[i][j] = false;
		}
	}

	visited[y][x] = true;
	q.push({ y, x });

	while (!q.empty()) {
		pair<int, int> cur = q.front();
		q.pop();
		for (int dir = 0; dir < 4; dir++) {
			int ny = cur.first + dy[dir];
			int nx = cur.second + dx[dir];
			// 정령의 움직임은 골렘 내부이거나, 
			// 골렘의 탈출구에 위치한다면 다른 골렘으로 이동 가능
			if (inRange(ny, nx) && !visited[ny][nx] &&
				(board[ny][nx] == board[cur.first][cur.second] ||
				(board[ny][nx] != 0 && is_exit[cur.first][cur.second]))) {
				q.push({ ny, nx });
				visited[ny][nx] = true;
				mx_y = max(mx_y, ny);
			}
		}
	}
	return mx_y;	
}

/*
골렘 id가 중심 (x, y), 출구 방향이 d일 때 움직임 취하는 함수
1. 남쪽으로 한 칸 내려감
2. (1) 방법 이동 불가 -> 서쪽 방향 회전 내려감
3. (1), (2) 불가 시 -> 동쪽 방향 회전 내려감
*/
void down(int y, int x, int d, int id) {
	if (canGo(y + 1, x)) {
		down(y + 1, x, d, id);
	}
	else if (canGo(y + 1, x - 1)) {
		down(y + 1, x - 1, (d+3)%4, id);
	}
	else if (canGo(y + 1, x + 1)) {
		down(y + 1, x + 1, (d+1)%4, id);
	}
	else {
		// 숲을 벗어나는 경우 -> 모든 골렘이 숲을 빠져나감
		if (!inRange(y - 1, x - 1) || !inRange(y + 1, x + 1)) {
			resetMap();
		}
		else {
			// 골렘이 숲 안에 정착함
			board[y][x] = id;
			for (int dir = 0; dir < 4; dir++) {
				board[y + dy[dir]][x + dx[dir]] = id;
			}
			is_exit[y + dy[d]][x + dx[d]] = true;	// 골렘 출구 기록
			ans += bfs(y, x) - 3 + 1;				// bfs를 통해 골렘이 내려갈 수 있는 최대 행 계산, 누적합
		}
	}
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> R >> C >> K;
	for (int id = 1; id <= K; id++) {
		int x, d;
		cin >> x >> d;		// 골렘 출발 열 좌표, 방향 번호
		x--;
		down(0, x, d, id);
	}
	cout << ans << '\n';
	return 0;
}