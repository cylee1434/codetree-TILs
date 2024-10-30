from collections import defaultdict as dd

n, m = map(int, input().split())
infos = list(map(int, input().split()))

# 최대 집중력보다 큰 값이 infos에 있다면 -1 출력
if max(infos) > m:
    print(-1)
else:
    answer = 13  # 초기 최소 인원 값
    dp = set()   # 이미 방문한 상태 저장

    def dfs(focus, infos):
        global answer

        # 이미 방문한 상태면 탈출
        if tuple(focus.items()) in dp:
            return
        dp.add(tuple(focus.items()))

        # 문제를 모두 풀었다면 최소 인원 갱신
        if not infos:
            answer = min(answer, n - focus[m])
            return

        # 백트래킹으로 탐색
        x = infos.pop()
        for f in list(focus.keys()):
            if focus[f] and f >= x:  # 해당 집중력으로 문제를 풀 수 있는 경우
                focus[f - x] += 1  # 집중력 감소
                focus[f] -= 1
                dfs(focus, infos[:])  # infos 복사본 사용
                focus[f - x] -= 1
                focus[f] += 1
        infos.append(x)

    focus = dd(int)
    focus[m] = n
    dfs(focus, infos)
    print(answer)