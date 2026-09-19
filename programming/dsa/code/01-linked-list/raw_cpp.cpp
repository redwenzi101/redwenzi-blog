// =====================================================================
// 数据结构与算法 · 第 1 章 链表 · 手写实现（C++）
// 编译运行： g++ -std=c++17 -Wall -Wextra -Wpedantic -o app raw_cpp.cpp && ./app
//
// 复杂度速查（时间 / 空间）
//   头插 O(1) / O(1)          尾插 O(n) / O(1)
//   查找 O(n) / O(1)          删除 O(n) / O(1)    （单链表要重新找前驱）
//   反转 O(n) / O(1)          找中点 O(n) / O(1)
//   判环 O(n) / O(1)          合并两条有序链表 O(n+m) / O(1)
// 说明：这里用 new/delete 手写结点，和 C 版逻辑完全一致，便于横向对照；
//      用标准库的写法见同目录 std_cpp.cpp。
// =====================================================================

#include <iostream>

struct Node {
    int value;
    Node *next;
};

// 头插：新结点接到表头，O(1)
Node *push_front(Node *head, int value) {
    return new Node{value, head};
}

// 尾插：先走到最后一个结点，O(n)
Node *push_back(Node *head, int value) {
    Node *node = new Node{value, nullptr};
    if (head == nullptr) return node;
    Node *cur = head;
    while (cur->next != nullptr) cur = cur->next;
    cur->next = node;
    return head;
}

// 遍历打印：以 "prefix: 1 2 3" 的形式输出
void print_all(const std::string &prefix, Node *head) {
    std::cout << prefix << ":";
    for (Node *cur = head; cur != nullptr; cur = cur->next) {
        std::cout << " " << cur->value;
    }
    std::cout << "\n";
}

// 查找：返回第几个（从 1 开始），找不到返回 0，O(n)
int index_of(Node *head, int value) {
    int i = 1;
    for (Node *cur = head; cur != nullptr; cur = cur->next, ++i) {
        if (cur->value == value) return i;
    }
    return 0;
}

// 删除第一个匹配的结点，O(n)
Node *erase(Node *head, int value) {
    Node *prev = nullptr;
    Node *cur = head;
    while (cur != nullptr) {
        if (cur->value == value) {
            if (prev == nullptr) head = cur->next;
            else prev->next = cur->next;
            delete cur;
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

// 反转：三指针迭代，O(n) 时间、O(1) 空间
Node *reverse(Node *head) {
    Node *prev = nullptr;
    Node *cur = head;
    while (cur != nullptr) {
        Node *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

// 找中点：快慢指针，O(n)
Node *middle(Node *head) {
    Node *slow = head;
    Node *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// 判环：Floyd 快慢指针，O(n)
bool has_cycle(Node *head) {
    Node *slow = head;
    Node *fast = head;
    while (fast != nullptr && fast->next != nullptr) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// 长度：O(n)
int length(Node *head) {
    int n = 0;
    for (Node *cur = head; cur != nullptr; cur = cur->next) ++n;
    return n;
}

// 合并两条有序链表：双指针穿针引线，O(n+m)
Node *merge_sorted(Node *a, Node *b) {
    Node dummy{0, nullptr};
    Node *tail = &dummy;
    while (a != nullptr && b != nullptr) {
        if (a->value <= b->value) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = (a != nullptr) ? a : b;
    return dummy.next;
}

// 整表释放：手写 new 就要手写 delete
void free_all(Node *head) {
    while (head != nullptr) {
        Node *next = head->next;
        delete head;
        head = next;
    }
}

int main() {
    Node *list = nullptr;

    list = push_front(list, 3);
    list = push_front(list, 2);
    list = push_front(list, 1);
    print_all("[手写] 头插 3,2,1", list);

    list = push_back(list, 4);
    print_all("[手写] 尾插 4", list);

    int pos = index_of(list, 3);
    std::cout << "[手写] 查找 3: 找到, 第 " << pos << " 个\n";
    std::cout << "[手写] 查找 9: " << (index_of(list, 9) == 0 ? "未找到" : "找到") << "\n";

    list = erase(list, 2);
    print_all("[手写] 删除 2", list);

    list = reverse(list);
    print_all("[手写] 反转", list);

    std::cout << "[手写] 找中点: " << middle(list)->value << "\n";

    std::cout << "[手写] 判环(无环): " << (has_cycle(list) ? "有环" : "无环") << "\n";
    Node *last = list;
    while (last->next != nullptr) last = last->next;
    last->next = list;                        // 人为造一个环
    std::cout << "[手写] 判环(有环): " << (has_cycle(list) ? "有环" : "无环") << "\n";
    last->next = nullptr;                     // 拆掉环，否则释放会出错

    std::cout << "[手写] 长度: " << length(list) << "\n";

    Node *a = nullptr;
    Node *b = nullptr;
    a = push_back(a, 1); a = push_back(a, 3); a = push_back(a, 5);
    b = push_back(b, 2); b = push_back(b, 4); b = push_back(b, 6);
    Node *merged = merge_sorted(a, b);
    print_all("[手写] 合并两条有序链表", merged);

    free_all(list);
    free_all(merged);
    return 0;
}
