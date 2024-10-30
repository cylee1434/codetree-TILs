def min_team_members(N, M, difficulties):
    # DP 배열을 초기화합니다. 매우 큰 값으로 설정하여 최소값을 찾습니다.
    dp = [float('inf')] * (1 << N)
    dp[0] = 0  # 아무 문제도 풀지 않은 상태는 팀원이 필요하지 않음

    # 모든 문제의 난이도 조합을 탐색합니다.
    for mask in range(1 << N):
        # 현재 상태에서 각 문제를 풀어 가능한 다음 상태를 만듭니다.
        for i in range(N):
            if mask & (1 << i) == 0:  # 아직 풀지 않은 문제라면
                next_mask = mask | (1 << i)
                sum_difficulty = difficulties[i]

                # 남은 문제를 팀원이 풀 수 있는지 체크
                for j in range(i + 1, N):
                    if next_mask & (1 << j) == 0 and sum_difficulty + difficulties[j] <= M:
                        next_mask |= (1 << j)
                        sum_difficulty += difficulties[j]

                # DP 업데이트
                dp[next_mask] = min(dp[next_mask], dp[mask] + 1)

    # 모든 문제를 풀기 위한 최소 팀원 수를 반환
    return dp[(1 << N) - 1] if dp[(1 << N) - 1] != float('inf') else -1

# 사용자 입력 받기
N, M = map(int, input().split())
difficulties = list(map(int, input().split()))

# 결과 출력
print(min_team_members(N, M, difficulties))