#include <iostream>
#include <vector>
#include <queue>
#define INF 1000000000
using namespace std;

typedef struct e{
	int r, c, d, v;
	bool alive = true;
} EVENT;

typedef pair<int, int> pii;


int n, m, f;
int space_map[20][20]; //평면 공간
int time_map[5][10][10]; //3차원 공간 : 동, 남, 서, 북, 윗면 순서
vector<EVENT> event_list;
int wall_start_x, wall_start_y, exit_x, exit_y, machine_x, machine_y;

//그래프 관련 변수
int num = 1;
int space_node_num[20][20];
int time_node_num[5][10][10];
int graph[900][4]; //[출발 노드 번호][동서남북] = 도착 노드 번호

//동남서북
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0};

//BFS 관련 변수
queue<int> q;
int dist[901];

/*
필요한 함수
- 노드 번호 부여하고 그래프 만드는 함수 : 노드 번호는 1번부터 부여
- event 확산시키는 함수
- bfs : 최소 턴 수 찾기
*/

/*
- 0 : 빈 공간
- 1 : 장애물
- 2 : 타임머신 위치
- 3 : 시간의 벽 위치
- 4 : 탈출구
*/


void make_graph() {
	//평면 공간에 노드 번호 부여하기 : 1번부터 부여
	//space_node_num[i][j] == (i, j)번째 공간의 노드 번호
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 3) continue;
			space_node_num[i][j] = num++;
		}
	}

	//3차원 공간에 노드 번호 부여하기
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				time_node_num[k][i][j] = num++;
			}
		}
	}

	num -= 1;

	for (int i = 0; i < 900; i++) {
		for (int j = 0; j < 4; j++) graph[i][j] = -1;
	}

	//그래프 만들기 : 동서남북으로 인접해있는 것들끼리 edge에 넣기
	//평면 공간부터 edge 추가
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 3) continue;

			int from_node = space_node_num[i][j];

			for (int k = 0; k < 4; k++) {
				int new_x = i + dx[k];
				int new_y = j + dy[k];

				if (new_x < 0 || new_x >= n || new_y < 0 || new_y >= n) continue;
				if (space_map[new_x][new_y] == 3) continue;

				int to_node = space_node_num[new_x][new_y];
				graph[from_node][k] = to_node;
			}
		}
	}


	//3차원 공간 동서남북 edge 추가
	for (int l = 0; l < 4; l++) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				int from_node = time_node_num[l][i][j];

				for (int k = 0; k < 4; k++) {
					int new_x = i + dx[k];
					int new_y = j + dy[k];
					int to_node;

					if (new_x < 0 || new_x >= m) {
						continue;
					}
					else if (new_y < 0) {
						int left_side = (l + 1) % 4;
						to_node = time_node_num[left_side][new_x][m - 1];
					}
					else if (new_y >= m) {
						int right_side = (l + 3) % 4;
						to_node = time_node_num[right_side][new_x][0];
					}
					else {
						to_node = time_node_num[l][new_x][new_y];
					}
					graph[from_node][k] = to_node;
				}
			}
		}
	}

	//평면(밑바닥)과 3차원 공간의 맨 아랫줄 연결
	//동
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[0][m - 1][i];
		int to_node = space_node_num[wall_start_x + m - 1 - i][wall_start_y + m];

		graph[from_node][1] = to_node;
		graph[to_node][2] = from_node;
	}

	
	//남
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[1][m - 1][i];
		int to_node = space_node_num[wall_start_x + m][wall_start_y + i];
		graph[from_node][1] = to_node;
		graph[to_node][3] = from_node;
	}

	//서
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[2][m - 1][i];
		int to_node = space_node_num[wall_start_x + i][wall_start_y - 1];
		graph[from_node][1] = to_node;
		graph[to_node][0] = from_node;
	}

	//북
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[3][m - 1][i];
		int to_node = space_node_num[wall_start_x - 1][wall_start_y + m - 1 - i];
		graph[from_node][1] = to_node;
		graph[to_node][1] = from_node;
	}


	//3차원 공간의 윗면과 동, 남, 서, 북 연결
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			int from_node = time_node_num[4][i][j];

			for (int k = 0; k < 4; k++) {
				int new_x = i + dx[k];
				int new_y = j + dy[k];
				
				if (new_x < 0 || new_x >= m || new_y < 0 || new_y >= m) continue;

				int to_node = time_node_num[4][new_x][new_y];
				graph[from_node][k] = to_node;
			}
		}
	}

	//3차원 공간의 윗면의 모서리 연결
	//동
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[4][i][m - 1];
		int to_node = time_node_num[0][0][m - 1 - i];
		graph[from_node][0] = to_node;
		graph[to_node][3] = from_node;
	}

	//서
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[4][i][0];
		int to_node = time_node_num[2][0][i];
		graph[from_node][2] = to_node;
		graph[to_node][3] = from_node;
	}

	//남
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[4][m - 1][i];
		int to_node = time_node_num[1][0][i];
		graph[from_node][1] = to_node;
		graph[to_node][3] = from_node;
	}

	//북
	for (int i = 0; i < m; i++) {
		int from_node = time_node_num[4][0][i];
		int to_node = time_node_num[3][0][m - 1 - i];
		graph[from_node][3] = to_node;
		graph[to_node][3] = from_node;
	}

}

void spread(int turn_cnt) {
	int s = event_list.size();
	for (int i = 0; i < s; i++) {
		//배수 턴이면 확산
		if (turn_cnt % event_list[i].v == 0 && event_list[i].alive == true) {
			int new_x = event_list[i].r;
			int new_y = event_list[i].c;
			
			if (event_list[i].d == 0) {
				new_y += (turn_cnt / event_list[i].v);
			}
			else if (event_list[i].d == 1) {
				new_x += (turn_cnt / event_list[i].v);
			}
			else if (event_list[i].d == 2) {
				new_y -= (turn_cnt / event_list[i].v);
			}
			else {
				new_x -= (turn_cnt / event_list[i].v);
			}

			if (new_x < 0 || new_x >= n || new_y < 0 || new_y >= n) {
				event_list[i].alive = false;
				continue;
			}

			if (space_map[new_x][new_y]) {
				event_list[i].alive = false;
				continue;
			}

			int to_node = space_node_num[new_x][new_y];
			dist[to_node] = INF;
		}
	}
}



int bfs() {
	//bfs 시작 전 사전 작업
	int start_node = time_node_num[4][machine_x][machine_y];
	dist[start_node] = 0;

	q.push(start_node);

	for (int cnt = 1; ; cnt++) {
		spread(cnt);
		
		vector<int> node_list;

		while (!q.empty()) {
			int from_node = q.front();
			q.pop();

			for (int i = 0; i < 4; i++) {
				int to_node = graph[from_node][i];
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

		if (dist[space_node_num[exit_x][exit_y]] != -1) {
			break;
		}
	}

	return dist[space_node_num[exit_x][exit_y]];
}


int main(void) {
	//freopen("input.txt", "r", stdin);

	cin >> n >> m >> f;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> space_map[i][j];
		}
	}


	//동
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[0][i][j];
		}
	}

	//서
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[2][i][j];
		}
	}

	//남
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[1][i][j];
		}
	}

	//북
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[3][i][j];
		}
	}

	//윗면
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			cin >> time_map[4][i][j];
		}
	}


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
				wall_start_x = i;
				wall_start_y = j;
				break;
			}
		}
	}

	wall_start_x -= (m - 1);

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
				machine_x = i;
				machine_y = j;
				break;
			}
		}
	}

	make_graph();

	for (int i = 0; i <= 900; i++) dist[i] = -1;

	//평면 장애물 처리
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (space_map[i][j] == 1) {
				int to_node = space_node_num[i][j];
				dist[to_node] = INF;
			}
		}
	}

	//3차원 공간 장애물 처리
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < m; j++) {
				if (time_map[k][i][j] == 1) {
					int to_node = time_node_num[k][i][j];
					dist[to_node] = INF;
				}
			}
		}
	}

	//이상현상 초기 위치 처리
	for (int i = 0; i < f; i++) {
		int x = event_list[i].r;
		int y = event_list[i].c;
		int to_node = space_node_num[x][y];
		dist[to_node] = INF;
	}

	cout << bfs();
}