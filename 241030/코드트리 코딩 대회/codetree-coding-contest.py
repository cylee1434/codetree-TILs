def min_team_members(N, M, difficulties):
    # DP 배열을 초기화합니다. 매우 큰 값으로 설정하여 최소값을 찾습니다.
    dp = [float('inf')] * (1 << N)
    dp[0] = 0  # 아무 문제도 풀지 않은 상태는 팀원이 필요하지 않음

    # 모든 상태를 탐색하며 dp를 업데이트합니다.
    for mask in range(1 << N):
        if dp[mask] == float('inf'):
            continue
        # 팀원이 풀 수 있는 문제들의 부분 집합을 구합니다.
        for i in range(1 << N):
            subset_sum = 0
            for j in range(N):
                if i & (1 << j) and not (mask & (1 << j)):
                    subset_sum += difficulties[j]
            if subset_sum <= M:
                dp[mask | i] = min(dp[mask | i], dp[mask] + 1)

    # 모든 문제를 풀기 위한 최소 팀원 수를 반환
    return dp[(1 << N) - 1] if dp[(1 << N) - 1] != float('inf') else -1

# 사용자 입력 받기
N, M = map(int, input().split())
difficulties = list(map(int, input().split()))

# 결과 출력
print(min_team_members(N, M, difficulties))