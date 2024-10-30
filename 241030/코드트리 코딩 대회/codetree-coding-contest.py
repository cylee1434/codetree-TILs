from collections import defaultdict as dd 

n, m = map(int, input().split())
infos = list(map(int, input().split()))

# 최대 집중력보다 크면x
if max(infos) > m:
    print(-1)
else:
    # 누가 푸는지는 상관x
    # 상태를 결정짓는 요소는 {집중1:남은, 집중력2:인원} 형태
    answer = 13
    dp = set()

    def dfs(focus, infos):
        global answer

        # 탈출 조건
        if tuple(focus.items()) in dp:
            return
        dp.add(tuple(focus.items()))

        # 다 풀었으면 최소 인원 갱신
        if not infos:
            answer = min(answer, n-focus[m])
            return
        # 백트래킹 형태로
        x = infos.pop()
        for f in list(focus.keys()):
            if focus[f] and f >= x: # 문제 하나 풀고
                focus[f-x] += 1 # 집중력 f를 가진 사람 -> f-x로
                focus[f] -= 1
                if tuple(focus.items()) not in dp:
                    dfs(focus, infos)
                focus[f-x] -= 1
                focus[f] += 1
        infos.append(x)
    
    focus = dd(int)
    focus[m] = n
    dfs(focus, infos)
    print(answer)