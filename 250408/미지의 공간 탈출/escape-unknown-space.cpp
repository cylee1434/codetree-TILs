#include <iostream>
#include <vector>
#include <queue>
using namespace std;

const int INF = 1e9;

struct EVENT {
	int r, c, d, v;
	bool alive = true;
};

int n, m, f;
int space_map[20][20];
int time_map[5][10][10];	// 3차원 공간: 동, 서, 남, 북, 윗면
vector<EVENT> event_list;
int wall_st, wall_sy, exit_x, exit_y, tmc_x, tmc_y;

// 그래프 관련 변수
int num = 0;
int space_graph[20][20];
int time_graph[5][10][10];
int graph[900][4];			// [출발 노드 번호][방향] = 도착 노드 번호

// 동남서북 
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};

queue<int> q;
int dist[901];

/*
필요한 함수
 - 노드 번호 부여하고 그래프 만드는 함수: 노드 번호는 1번부터 부여
 - event 확산 시키는 함수
 - bfs: 최소 턴 수 구하기
*/

/*
- 0 : 빈 공간
- 1 : 장애물
- 2 : 타임 머신 위치
- 3 : 시간의 벽 위치
- 4 : 탈출구
*/

void make_graph() {
	/*
	평면 공간에 노드 번호 부여
	space_graph[i][j] = (i, j) 번째 공간의 노드 번호
	*/
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 3) continue;
			space_graph[i][j] = num++;
		}
	}

	// 3차원 공간 노드 번호 부여
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				time_graph[k][i][j] = num++;
			}
		}
	}

	for (int i = 0; i < 900; i++) {
		for (int j = 0; j < 4; j++) graph[i][j] = -1;
	}

	// 그래프 만들기 -> 평면부터 edge 추가
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 3) continue;
			
			int from_node = space_graph[i][j];

			for (int d = 0; d < 4; d++) {
				int ni = i + dx[d];
				int nj = j + dy[d];

				if (ni < 0 || ni >= n || nj < 0 || nj >= n) continue;
				if (space_map[ni][nj] == 3) continue;

				// graph[i][j]는 노드 i에서 j방향(→↓←↑)으로 갈 수 있는 이웃 노드 번호를 저장
				int to_node = space_graph[ni][nj];
				graph[from_node][d] = to_node;
			}
		}
	}

	// 3차원 공간 동서남북 edge 추가
	for (int l = 0; l < 4; l++) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				int from_node = time_graph[l][i][j];

				for (int d = 0; d < 4; d++) {
					int ni = i + dx[d];
					int nj = j + dy[d];
					
					int to_node;

					// 상하는 면 전환이 없음
					if (ni < 0 || ni >= m) continue;
					// 현재 기준 왼쪽 면의 오른쪽 열
					else if (nj < 0) {
						int left = (l + 1) % 4;
						to_node = time_graph[left][ni][m - 1];
					}
					else if (nj >= m) {
						int right = (l + 3) % 4;
						to_node = time_graph[right][ni][0];
					}
					else {
						to_node = time_graph[l][ni][nj];
					}
					graph[from_node][d] = to_node;
				}
			}
		}
	}

	/*
	graph[from_node][dir] = to_node
	graph[to_node][dir] = from_node
	*/

	// 평면~ 3차원 공간 맨 아랫줄 연결
	// 동
	for (int a = 0; a < m; a++) {
		int from_node = time_graph[0][m - 1][a];
		int to_node = space_graph[wall_st + m - 1 -a][wall_sy + m];

		graph[from_node][1] = to_node;
		graph[to_node][2] = from_node;
	}

	// 남
	for (int a = 0; a < m; a++) {
		int from_node = time_graph[1][m - 1][a];
		int to_node = space_graph[wall_st + m][wall_sy+a];

		graph[from_node][1] = to_node;
		graph[to_node][3] = from_node;
	}

	// 서
	for (int a = 0; a < m; a++) {
		int from_node = time_graph[2][m - 1][a];
		int to_node = space_graph[wall_st + a][wall_sy - 1];

		graph[from_node][1] = to_node;
		graph[to_node][0] = from_node;
	}

	// 북
	for (int a = 0; a < m; a++) {
		int from_node = time_graph[3][m - 1][a];
		int to_node = space_graph[wall_st - 1][wall_sy + m -1 - a];

		graph[from_node][1] = to_node;
		graph[to_node][1] = from_node;
	}

	// 3차원 공간 윗면 ~ 동서남북 연결 -> 옆면 빠져나가기 위함
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			int from_node = time_graph[4][i][j];

			for (int d = 0; d < 4; d++) {
				int ni = i + dx[d];
				int nj = j + dy[d];

				if (ni < 0 || ni >= m || nj < 0 || nj >= m) continue;

				int to_node = time_graph[4][ni][nj];
				graph[from_node][d] = to_node;
			}
		}
	}

	// 3차원 공간의 윗면 모서리 연결
	// 동
	for (int i = 0; i < m; i++) {
		int from_node = time_graph[4][i][m-1];
		int to_node = time_graph[0][0][m - 1 - i];

		graph[from_node][0] = to_node;
		graph[to_node][3] = from_node;
 	}

	// 서
	for (int i = 0; i < m; i++) {
		int from_node = time_graph[4][i][0];
		int to_node = time_graph[2][0][i];

		graph[from_node][2] = to_node;
		graph[to_node][3] = from_node;
	}

	// 남
	for (int i = 0; i < m; i++) {
		int from_node = time_graph[4][m - 1][i];
		int to_node = time_graph[1][0][i];

		graph[from_node][1] = to_node;
		graph[to_node][3] = from_node;
	}

	// 북
	for (int i = 0; i < m; i++) {
		int from_node = time_graph[4][0][i];
		int to_node = time_graph[3][0][m-1-i];

		graph[from_node][3] = to_node;
		graph[to_node][3] = from_node;
	}
} 

void spread(int turn) {
	int s = event_list.size();
	for (int i = 0; i < s; i++) {
		if (turn % event_list[i].v == 0 && event_list[i].alive == true) {
			int nx = event_list[i].r;
			int ny = event_list[i].c;

			if (event_list[i].d == 0) {
				ny += (turn / event_list[i].v);
			}
			else if (event_list[i].d == 1) {
				nx += (turn / event_list[i].v);
			}
			else if (event_list[i].d == 2) {
				ny -= (turn / event_list[i].v);
			}
			else {
				nx -= (turn / event_list[i].v);
			}

			if (nx < 0 || nx >= n || ny < 0 || ny >= n) {
				event_list[i].alive = false;
				continue;
			}

			if (space_map[nx][ny]) {
				event_list[i].alive = false;
				continue;
			}

			int to_node = space_graph[nx][ny];
			dist[to_node] = INF;
		}
	}
}

int bfs() {
	// 사전 작업
	int start_node = time_graph[4][tmc_x][tmc_y];
	dist[start_node] = 0;

	q.push(start_node);

	for (int cnt = 1; ; cnt++) {
		spread(cnt);

		vector<int> node_list;

		while (!q.empty()) {
			int from_node = q.front();
			q.pop();

			for (int d = 0; d < 4; d++) {
				int to_node = graph[from_node][d];
				if (to_node == -1) continue;
				if (dist[to_node] != -1) continue;

				node_list.push_back(to_node);
				dist[to_node] = cnt;
			}
		}

		if (!node_list.size()) break;

		for (int i = 0; i < node_list.size(); i++) {
			q.push(node_list[i]);
		}

		if (dist[space_graph[exit_x][exit_y]] != -1) break;
	}
	return dist[space_graph[exit_x][exit_y]];
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> n >> m >> f;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> space_map[i][j];
		}
	}

	// 동
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[0][i][j];
		}
	}

	// 서
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[2][i][j];
		}
	}

	// 남
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[1][i][j];
		}
	}

	// 북
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[3][i][j];
		}
	}

	// 윗면
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[4][i][j];
		}
	}

	// d : 동(0) 서(1) 남(2) 북(3)
	for (int i = 0; i < f; i++) {
		int r, c, d, v, new_d;
		cin >> r >> c >> d >> v;

		if (d == 1) new_d = 2;
		else if (d == 2) new_d = 1;
		else new_d = d;
		event_list.push_back({ r, c, new_d, v });
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 3) {
				wall_st = i;
				wall_sy = j;
				break;
			}
		}
	}

	wall_st -= (m - 1);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 4) {
				exit_x = i;
				exit_y = j;
				break;
			}
		}
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			if (time_map[4][i][j] == 2) {
				tmc_x = i;
				tmc_y = j;
				break;
			}
		}
	}

	make_graph();

	for (int i = 0; i <= 900; i++) dist[i] = -1;

	// 평면 장애물 처리
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 1) {
				int to_node = space_graph[i][j];
				dist[to_node] = INF;
			}
		}
	}

	// 3차원 공간 장애물 처리
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				if (time_map[k][i][j] == 1) {
					int to_node = time_graph[k][i][j];
					dist[to_node] = INF;
				}
			}
		}
	}

	// 이상 현상 초기 위치 처리
	for (int i = 0; i < f; i++) {
		int x = event_list[i].r;
		int y = event_list[i].c;
		int to_node = space_graph[x][y];
		dist[to_node] = INF;
	}
	cout << bfs();
}