#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
using namespace std;

int K, M;
vector<vector<int>> arr(5, vector<int>(5));
vector<int> lst;
vector<int> ans;

vector<vector<int>> rotate(const vector<vector<int>> &a, int si, int sj) {
    vector<vector<int>> narr = a;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            narr[si + i][sj + j] = a[si + 3 - j - 1][sj + i];
    return narr;
}

int bfs(vector<vector<int>> &a, vector<vector<int>> &v, int si, int sj, int clr) {
    queue<pair<int, int>> q;
    set<pair<int, int>> sset;
    int cnt = 0;

    q.push({si, sj});
    v[si][sj] = 1;
    sset.insert({si, sj});
    cnt++;

    while (!q.empty()) {
        int ci = q.front().first;
        int cj = q.front().second;
        q.pop();

        int di[4] = {-1, 1, 0, 0};
        int dj[4] = {0, 0, -1, 1};

        for (int d = 0; d < 4; ++d) {
            int ni = ci + di[d];
            int nj = cj + dj[d];
            if (0 <= ni && ni < 5 && 0 <= nj && nj < 5 && v[ni][nj] == 0 && a[ci][cj] == a[ni][nj]) {
                q.push({ni, nj});
                v[ni][nj] = 1;
                sset.insert({ni, nj});
                cnt++;
            }
        }
    }

    if (cnt >= 3) {
        if (clr == 1) {
            for (auto it : sset)
                a[it.first][it.second] = 0;
        }
        return cnt;
    }
    return 0;
}

int count_clear(vector<vector<int>> &a, int clr) {
    vector<vector<int>> v(5, vector<int>(5, 0));
    int cnt = 0;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            if (v[i][j] == 0)
                cnt += bfs(a, v, i, j, clr);
    return cnt;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> K >> M;
    for (int i = 0; i < 5; ++i)
        for (int j = 0; j < 5; ++j)
            cin >> arr[i][j];

    lst.resize(M);
    for (int i = 0; i < M; ++i)
        cin >> lst[i];

    for (int turn = 0; turn < K; ++turn) {
        int mx_cnt = 0;
        vector<vector<int>> marr;

        for (int rot = 1; rot <= 3; ++rot) {
            for (int sj = 0; sj <= 2; ++sj) {
                for (int si = 0; si <= 2; ++si) {
                    vector<vector<int>> narr = arr;
                    for (int r = 0; r < rot; ++r)
                        narr = rotate(narr, si, sj);

                    int t = count_clear(narr, 0);
                    if (mx_cnt < t) {
                        mx_cnt = t;
                        marr = narr;
                    }
                }
            }
        }

        if (mx_cnt == 0)
            break;

        arr = marr;
        int cnt = 0;

        while (true) {
            int t = count_clear(arr, 1);
            if (t == 0) break;
            cnt += t;

            for (int j = 0; j < 5; ++j) {
                for (int i = 4; i >= 0; --i) {
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
