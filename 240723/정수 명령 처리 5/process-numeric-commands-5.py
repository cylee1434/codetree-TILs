n = int(input())

dynamic_array = []

for _ in range(n):
    command = input().strip().split()

    if command[0] == "push_back":
        dynamic_array.append(int(command[1]))

    elif command[0] == "pop_back":
        if dynamic_array:
            dynamic_array.pop()
    
    elif command[0] == "size":
        print(len(dynamic_array))

    elif command[0] == "get":
        k = int(command[1]) - 1
        if 0 <= k < len(dynamic_array):
            print(dynamic_array[k])