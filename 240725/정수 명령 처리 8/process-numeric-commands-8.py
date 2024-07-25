class Node:
    def __init__(self, data):
        self.data = data 
        self.next = None 
        self.prev = None

class DoublyLinkedList:
    def __init__(self):
        self.head = None
        self.tail = None
        self.node_num = 0
  
    def push_front(self, new_data):
        new_node = Node(new_data)
        new_node.next = self.head

        if self.head is not None:
            self.head.prev = new_node
        else:
            self.tail = new_node
        self.head = new_node
        
        self.node_num += 1

    def push_back(self, new_data):
        new_node = Node(new_data)
        new_node.prev = self.tail

        if self.tail is not None:
            self.tail.next = new_node
        else:
            self.head = new_node
        self.tail = new_node

        self.node_num += 1

    def pop_front(self):
        if self.head is None:
            return "List is empty"
        
        temp = self.head
        if self.head.next is not None:
            self.head = self.head.next
            self.head.prev = None
        else:
            self.head = None
            self.tail = None
        
        self.node_num -= 1
        return temp.data

    def pop_back(self):
        if self.tail is None:
            return "List is empty"
        
        temp = self.tail
        if self.tail.prev is not None:
            self.tail = self.tail.prev
            self.tail.next = None
        else:
            self.head = None
            self.tail = None
        
        self.node_num -= 1
        return temp.data

    def size(self):
        return self.node_num

    def empty(self):
        return 1 if self.node_num == 0 else 0

    def front(self):
        if self.head is None:
            return "List is empty"
        else:
            return self.head.data

    def back(self):
        if self.tail is None:
            return "List is empty"
        else:
            return self.tail.data

n = int(input())
dll = DoublyLinkedList()

for _ in range(n):
    command = input().strip().split()
    if command[0] == "push_back":
        dll.push_back(int(command[1]))
    elif command[0] == "push_front":
        dll.push_front(int(command[1]))
    elif command[0] == "pop_front":
        print(dll.pop_front())
    elif command[0] == "pop_back":
        print(dll.pop_back())
    elif command[0] == "front":
        print(dll.front())
    elif command[0] == "back":
        print(dll.back())
    elif command[0] == "size":
        print(dll.size())
    elif command[0] == "empty":
        print(dll.empty())