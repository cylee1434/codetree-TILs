#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;
using vvi = vector<vector<int>>;

int K, M;
vvi arr(5, vector<int>(5, 0));
vector<int> lst;
vector<int> ans;

vvi rotate(const vvi& a, int si, int sj) {
	vvi narr = a;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			narr[si + i][sj + j] = a[si + 3 - j - 1][sj + i];
		}
	}
	return narr;
}

int bfs(vvi& a, vvi& v, int si, int sj, int clr) {
	queue<pair<int, int>> q;
	set<pair<int, int>> sset;
	int cnt = 0;

	q.push({ si, sj });
	v[si][sj] = 1;
	sset.insert({ si, sj });
	cnt++;

	while (!q.empty()) {
		pair<int, int> cur = q.front();
		q.pop();

		int dx[4] = { -1, 1, 0, 0 };
		int dy[4] = { 0, 0, -1, 1 };

		for (int d = 0; d < 4; d++) {
			int nx = cur.first + dx[d];
			int ny = cur.second + dy[d];
			if (0 <= nx && nx < 5 && 0 <= ny && ny < 5 && v[nx][ny] == 0 &&
				a[cur.first][cur.second] == a[nx][ny]) {
				q.push({ nx, ny });
				v[nx][ny] = 1;
				sset.insert({ nx, ny });
				cnt++;
			}
		}
	}

	if (cnt >= 3) {
		if (clr == 1) {
			for (auto it : sset) {
				a[it.first][it.second] = 0;
			}
		}
		return cnt;
	}
	return 0;
}

int count_clear(vvi& a, int clr) {
	vvi v(5, vector<int>(5, 0));
	int cnt = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (v[i][j] == 0) {
				cnt += bfs(a, v, i, j, clr);
			}
		}
	}
	return cnt;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> K >> M;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			cin >> arr[i][j];
		}
	}

	lst.resize(M);
	for (int i = 0; i < M; i++) {
		cin >> lst[i];
	}

	for (int turn = 0; turn < K; turn++) {
		int mx_cnt = 0;
		vvi m_arr;

		for (int rot = 1; rot <= 3; rot++) {
			for (int sj = 0; sj <= 2; sj++) {
				for (int si = 0; si <= 2; si++) {
					vvi n_arr = arr;
					for (int r = 0; r < rot; r++) {
						n_arr = rotate(n_arr, si, sj);
					}

					int t = count_clear(n_arr, 0);
					if (mx_cnt < t) {
						mx_cnt = t;
						m_arr = n_arr;
					}
				}
			}
		}

		if (mx_cnt == 0) break;

		arr = m_arr;
		int cnt = 0;

		while (true) {
			int t = count_clear(arr, 1);
			if (t == 0) break;
			cnt += t;

			for (int j = 0; j < 5; j++) {
				for (int i = 4; i >= 0; i--) {
					if (arr[i][j] == 0 && !lst.empty()) {
						arr[i][j] = lst.front();
						lst.erase(lst.begin());
					}
				}
			}
		}

		ans.push_back(cnt);
	}
	for (int x : ans) cout << x << ' ';
	cout << '\n';
	return 0;

}