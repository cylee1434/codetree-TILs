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

int cal_mangattan(const pii& a, const pii& b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

void cal_dist(int startX, int startY, int N) {
    // 보드 초기화, 장애물 INF, 아니면 -1
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = board[i][j] ? INF : -1;
        }
    }

    queue<pii> q;
    q.push({startX, startY});
    dist[startX][startY] = 0; // 시작 지점 거리는 0

    while (!q.empty()) {
        int curX = q.front().first;
        int curY = q.front().second;
        
        q.pop();

        for (int d = 0; d < 4; d++) {
            int nx = curX + dx[d];
            int ny = curY + dy[d];

            if (nx < 0 || nx >= N || ny < 0 || ny>= N) continue;
            if (dist[nx][ny] != -1) continue;
            
            dist[nx][ny] = dist[curX][curY] + 1;
            q.push({nx, ny});
        }

    }
}

// 시야 방향에 따른 함수 선언 (위, 아래, 왼쪽, 오른쪽)
// int sightUp(int x, int y, int N, bool isTest);
// int sightDown(int x, int y, int N, bool isTest);
// int sightLeft(int x, int y, int N, bool isTest);
// int sightRight(int x, int y, int N, bool isTest);

int sightUp(int x, int y, int N, bool isTest) {
    for (int i = x-1; i >= 0; i--) {
        int left = max(0, y - (x - i));
        int right = min(N-1, y + (x - i));
        for (int j = left; j <= right; j++) {
            sightMap[i][j] = 1;
        }
    }

    // 장애물 처리 : 시야 막힘 여부 확인
    bool obstacle = false;
    for (int i = x - 1; i >= 0; i--) {
        if (obstacle) {
            sightMap[i][y] = 0; // 장애물 확인 후 시야 제거
        }
        else {
            sightMap[i][y] = 1; // 장애물 미발견시 시야 유지
        }

        if (warrior_cnt[i][y]) {
            obstacle = true;    // 전사가 있는 경우 장애물로 간주
        }
    }

    // 장애물에 따라 시야 조정
    for(int i = x - 1; i >= 1; i--) { // 시야 지우기 대상인 위쪽 셀 i-1이 배열 범위를 벗어나지 않도록
        int left = max(0, y - (x - i));           // 왼쪽 경계 계산
        int right = min(N - 1, y + (x - i));      // 오른쪽 경계 계산

        // 왼쪽 측면 조정
        for(int j = left; j < y; j++) {
            if((!sightMap[i][j]) || warrior_cnt[i][j]) {
                if(j > 0) sightMap[i-1][j-1] = 0; // 왼쪽 위 셀의 시야 제거
                sightMap[i-1][j] = 0;            // 바로 위 셀의 시야 제거
            }
        }

        // 오른쪽 측면 조정
        for(int j = y + 1; j <= right; j++) {
            if((!sightMap[i][j]) || warrior_cnt[i][j]) {
                if(j + 1 < N) sightMap[i-1][j+1] = 0; // 오른쪽 위 셀의 시야 제거
                sightMap[i-1][j] = 0;                // 바로 위 셀의 시야 제거
            }
        }
    }

    // 시야로 커버된 전사 수 계산
    int coverage = 0;
    for(int i = x - 1; i >= 0; i--) {
        int left = max(0, y - (x - i));           // 왼쪽 경계 계산
        int right = min(N - 1, y + (x - i));      // 오른쪽 경계 계산
        for(int j = left; j <= right; j++) {
            if(sightMap[i][j]) {
                coverage += warrior_cnt[i][j]; // 시야 내에 있는 전사의 수를 더함
            }
        }
    }

    // 테스트 모드인 경우 시야 맵을 원래대로 되돌림
    if(isTest) {
        for(int i = x - 1; i >= 0; i--) {
            int left = max(0, y - (x - i));
            int right = min(N - 1, y + (x - i));
            for(int j = left; j <= right; j++) {
                sightMap[i][j] = 0;                   // 시야 제거
            }
        }
    }

    return coverage;    // 커버리지 반환
}

int sightDown(int x, int y, int N, bool isTest) {
    // 다이아몬드 형태로 아래쪽 셀을 시야에 포함시킴
    for(int i = x + 1; i < N; i++) {
        int left = max(0, y - (i - x));           // 왼쪽 경계 계산
        int right = min(N - 1, y + (i - x));      // 오른쪽 경계 계산
        for(int j = left; j <= right; j++) {
            sightMap[i][j] = 1;                   // 시야 설정
        }
    }

    // 장애물 처리: 시야 막힘 여부 확인
    bool obstacle = false;
    for(int i = x + 1; i < N; i++) {
        if(obstacle) {
            sightMap[i][y] = 0;                   // 장애물이 발견된 후에는 시야 제거
        }
        else {
            sightMap[i][y] = 1;                   // 장애물이 발견되지 않으면 시야 유지
        }

        if(warrior_cnt[i][y]) {             // 전사가 있는 경우 장애물로 간주
            obstacle = true;
        }
    }

    // 장애물에 따라 시야 조정
    for(int i = x + 1; i < N - 1; i++) {
        int left = max(0, y - (i - x));           // 왼쪽 경계 계산
        int right = min(N - 1, y + (i - x));      // 오른쪽 경계 계산

        // 왼쪽 측면 조정
        for(int j = left; j < y; j++) {
            if((!sightMap[i][j]) || warrior_cnt[i][j]) {
                if(j > 0) sightMap[i+1][j-1] = 0; // 왼쪽 아래 셀의 시야 제거
                sightMap[i+1][j] = 0;            // 바로 아래 셀의 시야 제거
            }
        }

        // 오른쪽 측면 조정
        for(int j = y + 1; j <= right; j++) {
            if((!sightMap[i][j]) || warrior_cnt[i][j]) {
                if(j + 1 < N) sightMap[i+1][j+1] = 0; // 오른쪽 아래 셀의 시야 제거
                sightMap[i+1][j] = 0;                // 바로 아래 셀의 시야 제거
            }
        }
    }

    // 시야로 커버된 전사 수 계산
    int coverage = 0;
    for(int i = x + 1; i < N; i++) {
        int left = max(0, y - (i - x));           // 왼쪽 경계 계산
        int right = min(N - 1, y + (i - x));      // 오른쪽 경계 계산
        for(int j = left; j <= right; j++) {
            if(sightMap[i][j]) {
                coverage += warrior_cnt[i][j]; // 시야 내에 있는 전사의 수를 더함
            }
        }
    }

    // 테스트 모드인 경우 시야 맵을 원래대로 되돌림
    if(isTest) {
        for(int i = x + 1; i < N; i++) {
            int left = max(0, y - (i - x));
            int right = min(N - 1, y + (i - x));
            for(int j = left; j <= right; j++) {
                sightMap[i][j] = 0;                   // 시야 제거
            }
        }
    }

    return coverage;    // 커버리지 반환
}

/**
 * @brief 왼쪽 방향으로 시야를 설정하는 함수 (sightUp과 유사)
 * 
 * @param x 현재 플레이어의 x 좌표
 * @param y 현재 플레이어의 y 좌표
 * @param N 그리드의 크기
 * @param isTest 테스트 모드 여부 (true: 테스트, false: 실제 적용)
 * @return int 시야로 커버된 전사의 수
 */
int sightLeft(int x, int y, int N, bool isTest) {
    // 다이아몬드 형태로 왼쪽 셀을 시야에 포함시킴
    for(int i = y - 1; i >= 0; i--) {
        int top = max(0, x - (y - i));            // 상단 경계 계산
        int bottom = min(N - 1, x + (y - i));     // 하단 경계 계산
        for(int j = top; j <= bottom; j++) {
            sightMap[j][i] = 1;                    // 시야 설정
        }
    }

    // 장애물 처리: 시야 막힘 여부 확인
    bool obstacle = false;
    for(int i = y - 1; i >= 0; i--) {
        if(obstacle) {
            sightMap[x][i] = 0;                    // 장애물이 발견된 후에는 시야 제거
        }
        else {
            sightMap[x][i] = 1;                    // 장애물이 발견되지 않으면 시야 유지
        }

        if(warrior_cnt[x][i]) {               // 전사가 있는 경우 장애물로 간주
            obstacle = true;
        }
    }

    // 장애물에 따라 시야 조정
    for(int i = y - 1; i > 0; i--) {
        int top = max(0, x - (y - i));             // 상단 경계 계산
        int bottom = min(N - 1, x + (y - i));      // 하단 경계 계산

        // 상단 측면 조정
        for(int j = top; j < x; j++) {
            if((!sightMap[j][i]) || warrior_cnt[j][i]) {
                if(j > 0) sightMap[j-1][i-1] = 0; // 왼쪽 위 셀의 시야 제거
                sightMap[j][i-1] = 0;            // 바로 왼쪽 셀의 시야 제거
            }
        }

        // 하단 측면 조정
        for(int j = x + 1; j <= bottom; j++) {
            if((!sightMap[j][i]) || warrior_cnt[j][i]) {
                if(j + 1 < N) sightMap[j+1][i-1] = 0; // 왼쪽 아래 셀의 시야 제거
                sightMap[j][i-1] = 0;                // 바로 왼쪽 셀의 시야 제거
            }
        }
    }

    // 시야로 커버된 전사 수 계산
    int coverage = 0;
    for(int i = y - 1; i >= 0; i--) {
        int top = max(0, x - (y - i));             // 상단 경계 계산
        int bottom = min(N - 1, x + (y - i));      // 하단 경계 계산
        for(int j = top; j <= bottom; j++) {
            if(sightMap[j][i]) {
                coverage += warrior_cnt[j][i]; // 시야 내에 있는 전사의 수를 더함
            }
        }
    }

    // 테스트 모드인 경우 시야 맵을 원래대로 되돌림
    if(isTest) {
        for(int i = y - 1; i >= 0; i--) {
            int top = max(0, x - (y - i));
            int bottom = min(N - 1, x + (y - i));
            for(int j = top; j <= bottom; j++) {
                sightMap[j][i] = 0;                   // 시야 제거
            }
        }
    }

    return coverage;    // 커버리지 반환
}

/**
 * @brief 오른쪽 방향으로 시야를 설정하는 함수 (sightUp과 유사)
 * 
 * @param x 현재 플레이어의 x 좌표
 * @param y 현재 플레이어의 y 좌표
 * @param N 그리드의 크기
 * @param isTest 테스트 모드 여부 (true: 테스트, false: 실제 적용)
 * @return int 시야로 커버된 전사의 수
 */
int sightRight(int x, int y, int N, bool isTest) {
    // 다이아몬드 형태로 오른쪽 셀을 시야에 포함시킴
    for(int i = y + 1; i < N; i++) {
        int top = max(0, x - (i - y));            // 상단 경계 계산
        int bottom = min(N - 1, x + (i - y));     // 하단 경계 계산
        for(int j = top; j <= bottom; j++) {
            sightMap[j][i] = 1;                    // 시야 설정
        }
    }

    // 장애물 처리: 시야 막힘 여부 확인
    bool obstacle = false;
    for(int i = y + 1; i < N; i++) {
        if(obstacle) {
            sightMap[x][i] = 0;                    // 장애물이 발견된 후에는 시야 제거
        }
        else {
            sightMap[x][i] = 1;                    // 장애물이 발견되지 않으면 시야 유지
        }

        if(warrior_cnt[x][i]) {               // 전사가 있는 경우 장애물로 간주
            obstacle = true;
        }
    }

    // 장애물에 따라 시야 조정
    for(int i = y + 1; i < N - 1; i++) {
        int top = max(0, x - (i - y));             // 상단 경계 계산
        int bottom = min(N - 1, x + (i - y));      // 하단 경계 계산

        // 상단 측면 조정
        for(int j = top; j < x; j++) {
            if((!sightMap[j][i]) || warrior_cnt[j][i]) {
                if(j > 0) sightMap[j-1][i+1] = 0; // 오른쪽 위 셀의 시야 제거
                sightMap[j][i+1] = 0;            // 바로 오른쪽 셀의 시야 제거
            }
        }

        // 하단 측면 조정
        for(int j = x + 1; j <= bottom; j++) {
            if((!sightMap[j][i]) || warrior_cnt[j][i]) {
                if(j + 1 < N) sightMap[j+1][i+1] = 0; // 오른쪽 아래 셀의 시야 제거
                sightMap[j][i+1] = 0;                // 바로 오른쪽 셀의 시야 제거
            }
        }
    }

    // 시야로 커버된 전사 수 계산
    int coverage = 0;
    for(int i = y + 1; i < N; i++) {
        int top = max(0, x - (i - y));             // 상단 경계 계산
        int bottom = min(N - 1, x + (i - y));      // 하단 경계 계산
        for(int j = top; j <= bottom; j++) {
            if(sightMap[j][i]) {
                coverage += warrior_cnt[j][i]; // 시야 내에 있는 전사의 수를 더함
            }
        }
    }

    // 테스트 모드인 경우 시야 맵을 원래대로 되돌림
    if(isTest) {
        for(int i = y + 1; i < N; i++) {
            int top = max(0, x - (i - y));
            int bottom = min(N - 1, x + (i - y));
            for(int j = top; j <= bottom; j++) {
                sightMap[j][i] = 0;                   // 시야 제거
            }
        }
    }

    return coverage;    // 커버리지 반환
}

int bestSight(int x, int y, int N) {
    int maxCover = 0;   // 최대 커버리지 저장 변수
    int bestDir = -1;   // 최적의 시야 방향

    // 0: 위, 1: 아래, 2: 왼쪽, 3: 오른쪽
    for (int d = 0; d < 4; d++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                sightMap[i][j] = 0;
            }
        }
        int coverage = 0;

        // 방향에 따라 시야 계산 함수 호출
        if (d == 0) {
            coverage = sightUp(x, y, N, true);
        } else if (d == 1) {
            coverage = sightDown(x, y, N, true);
        } else if (d == 2) {
            coverage = sightLeft(x, y, N, true);
        } else if (d == 3) {
            coverage = sightRight(x, y, N, true);
        }
    
        if (maxCover < coverage) {
            maxCover = coverage;
            bestDir = d;
        }
    }

    // 시야 맵 다시 초기화
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            sightMap[i][j] = 0;
        }
    }

    if (bestDir == 0) {
        sightUp(x, y, N, false);
    } else if (bestDir == 1) {
        sightDown(x, y, N, false);
    } else if (bestDir == 2) {
        sightLeft(x, y, N, false);
    } else if (bestDir ==3) {
        sightRight(x, y, N, false);
    }

    return maxCover;
}


/**
 * @brief 플레이어를 향해 전사들을 이동시키는 함수
 * 
 * @param playerX 플레이어의 현재 x 좌표
 * @param playerY 플레이어의 현재 y 좌표
 * @param N 그리드의 크기
 * @param M 전사의 수
 * @return pii (총 이동한 전사 수, 플레이어에게 도달한 전사 수)
 */
pii moveWarriors(int playerX, int playerY, int N, int M) {
    int totalMoved = 0;   // 총 이동한 전사 수
    int totalHits = 0;    // 플레이어에게 도달한 전사 수

    // 전사의 이동 방향: 위, 아래, 왼쪽, 오른쪽
    // const int moveDX[4] = { -1, 1, 0, 0 };
    // const int moveDY[4] = { 0, 0, -1, 1 };

    // 모든 전사에 대해 이동 처리
    for(int i = 0; i < M; i++) {
        if(warrior[i].first == -1)
            continue; // 이미 잡힌 전사는 건너뜀

        int warriorX = warrior[i].first;
        int warriorY = warrior[i].second;

        // 시야 내에 있는 전사는 이동하지 않음
        if(sightMap[warriorX][warriorY])
            continue;

        // 현재 플레이어와의 거리 계산
        int currentDistance = cal_mangattan({playerX, playerY}, {warriorX, warriorY});
        bool hasMoved = false;   // 이동 여부 플래그

        // 첫 번째 이동: 거리를 줄이기 위해 이동
        for(int dir = 0; dir < 4; dir++) {
            int nextX = warriorX + dx[dir];
            int nextY = warriorY + dy[dir];

            // 이동할 위치가 그리드 내에 있고 시야 내에 있지 않은지 확인
            if(nextX < 0 || nextY < 0 || nextX >= N || nextY >= N)
                continue;
            if(sightMap[nextX][nextY])
                continue;

            // 새로운 위치에서의 거리 계산
            int newDistance = cal_mangattan({playerX, playerY}, {nextX, nextY});
            if(newDistance < currentDistance) {
                warriorX = nextX;
                warriorY = nextY;
                hasMoved = true;
                totalMoved++;
                break; // 첫 번째 이동 후 루프 탈출
            }
        }

        // 두 번째 이동: 추가로 거리를 줄일 수 있는지 확인
        if(hasMoved) {
            int newDistance = cal_mangattan({playerX, playerY}, {warriorX, warriorY});
            for(int dir = 0; dir < 4; dir++) {
                // 반대 방향으로 이동 시도
                int nextX = warriorX + dx[(dir + 2) % 4];
                int nextY = warriorY + dy[(dir + 2) % 4];

                // 이동할 위치가 그리드 내에 있고 시야 내에 있지 않은지 확인
                if(nextX < 0 || nextY < 0 || nextX >= N || nextY >= N)
                    continue;
                if(sightMap[nextX][nextY])
                    continue;

                // 새로운 위치에서의 거리 계산
                int furtherDistance = cal_mangattan({playerX, playerY}, {nextX, nextY});
                if(furtherDistance < newDistance) {
                    warriorX = nextX;
                    warriorY = nextY;
                    totalMoved++;
                    break; // 두 번째 이동 후 루프 탈출
                }
            }
        }

        // 전사의 위치 업데이트
        warrior[i] = {warriorX, warriorY};
    }

    // 플레이어에게 도달한 전사 수 계산
    for(int i = 0; i < M; i++) {
        if(warrior[i].first == -1)
            continue; // 이미 잡힌 전사는 건너뜀

        if(warrior[i].first == playerX && warrior[i].second == playerY) {
            totalHits++;
            warrior[i] = { -1, -1 }; // 전사를 잡힌 상태로 표시
        }
    }

    return { totalMoved, totalHits }; // 결과 반환
}

/**
 * @brief 현재 전사들의 위치를 기반으로 각 셀에 있는 전사의 수를 업데이트하는 함수
 * 
 * @param N 그리드의 크기
 * @param M 전사의 수
 */
void updateWarrior(int N, int M) {
    // 전사 수 그리드를 초기화
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            warrior_cnt[i][j] = 0;

    // 각 전사의 위치를 확인하여 전사 수 그리드에 반영
    for(int i = 0; i < M; i++) {
        if(warrior[i].first == -1)
            continue; // 이미 잡힌 전사는 건너뜀
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

    int stX, stY, endX, endY;
    cin >> stX >> stY >> endX >> endY;

    for (int i = 0; i < M; i++) {
        cin >> warrior[i].first >> warrior[i].second;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> board[i][j];
        }
    }

    // 종료 지점으로부터 모든 셀까지의 거리 계산
    cal_dist(endX, endY, N);

    // 시작 지점이 종료 지점에 도달할 수 없는 경우 종료
    if (dist[stX][stY] == -1) {
        cout << "-1\n";
        return 0;
    }

    int curX = stX;  // 현재 플레이어의 X좌표
    int curY = stY;  // 현재 플레이어의 Y좌표

    // 플레이어가 종료 지점에 도달할 때까지 
    while (true) {
        bool moved = false; // 플레이어 이동 여부
        
        for (int d = 0; d < 4; d++) {
            int nx = curX + dx[d];
            int ny = curY + dy[d];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if (dist[nx][ny] < dist[curX][curY]) {
                curX = nx;
                curY = ny;
                moved = true;
                break;  // 이동 후 루프 탈출
            }
        }

        // 종료 지점 도달 시 종료
        if (curX == endX && curY == endY) {
            cout << "0\n";
            return 0;
        }
        // 현재 위치에 있는 전사들을 잡읍
        for (int i = 0; i < M; i++) {
            if (warrior[i].first == curX && warrior[i].second == curY) {
                warrior[i] = {-1, -1};  // 전사를 잡힌 상태로 표시
            }
        } 

        // 현재 전사들의 위치를 기반으로 전사 수 그리드 업데이트
        updateWarrior(N, M);

        // 최적의 시야 방향 선택 후 시야로 커버된 전사 수 계산
        int sightCov = bestSight(curX, curY, N);

        // 전사 이동 후, 이동한 전사 수와 플레이어에게 도달한 전사 수 얻음
        pii war_result = moveWarriors(curX, curY, N, M);

        // 이동한 전사 수, 시야 커버리지, 플레이어에게 도달한 전사 수 출력
        cout << war_result.first << " " << sightCov << " " << war_result.second << '\n';
    }
    return 0;
}