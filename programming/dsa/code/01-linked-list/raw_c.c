/* =====================================================================
   数据结构与算法 · 第 1 章 链表 · 手写实现（C）
   编译运行： gcc -std=c17 -Wall -Wextra -o app raw_c.c && ./app

   复杂度速查（时间 / 空间）
     头插 O(1) / O(1)          尾插 O(n) / O(1)
     查找 O(n) / O(1)          删除 O(n) / O(1)    （单链表要重新找前驱）
     反转 O(n) / O(1)          找中点 O(n) / O(1)
     判环 O(n) / O(1)          合并两条有序链表 O(n+m) / O(1)
   说明：C 没有标准库链表，用结构体 + malloc/free 自己组织内存，
        这正是"指针实现"在 C 里的标准写法。
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

/* 头插：新结点接到表头，O(1) */
static Node *push_front(Node *head, int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) return head;
    node->value = value;
    node->next = head;
    return node;
}

/* 尾插：先走到最后一个结点，O(n) */
static Node *push_back(Node *head, int value) {
    Node *node = (Node *)malloc(sizeof(Node));
    if (node == NULL) return head;
    node->value = value;
    node->next = NULL;
    if (head == NULL) return node;
    Node *cur = head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = node;
    return head;
}

/* 遍历打印：以 "prefix: 1 2 3" 的形式输出 */
static void print_all(const char *prefix, Node *head) {
    printf("%s:", prefix);
    for (Node *cur = head; cur != NULL; cur = cur->next) {
        printf(" %d", cur->value);
    }
    printf("\n");
}

/* 查找：返回第几个（从 1 开始），找不到返回 0，O(n) */
static int index_of(Node *head, int value) {
    int i = 1;
    for (Node *cur = head; cur != NULL; cur = cur->next, i++) {
        if (cur->value == value) return i;
    }
    return 0;
}

/* 删除第一个匹配的结点，O(n) */
static Node *erase(Node *head, int value) {
    Node *prev = NULL;
    Node *cur = head;
    while (cur != NULL) {
        if (cur->value == value) {
            if (prev == NULL) {
                head = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur);
            return head;
        }
        prev = cur;
        cur = cur->next;
    }
    return head;
}

/* 反转：三指针迭代，O(n) 时间、O(1) 空间 */
static Node *reverse(Node *head) {
    Node *prev = NULL;
    Node *cur = head;
    while (cur != NULL) {
        Node *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    return prev;
}

/* 找中点：快慢指针，快指针走两步、慢指针走一步，O(n) */
static Node *middle(Node *head) {
    Node *slow = head;
    Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

/* 判环：Floyd 快慢指针，相遇即有环，O(n) */
static int has_cycle(Node *head) {
    Node *slow = head;
    Node *fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return 1;
    }
    return 0;
}

/* 长度：O(n) */
static int length(Node *head) {
    int n = 0;
    for (Node *cur = head; cur != NULL; cur = cur->next) n++;
    return n;
}

/* 合并两条有序链表：双指针穿针引线，O(n+m) */
static Node *merge_sorted(Node *a, Node *b) {
    Node dummy;
    dummy.next = NULL;
    Node *tail = &dummy;
    while (a != NULL && b != NULL) {
        if (a->value <= b->value) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    tail->next = (a != NULL) ? a : b;
    return dummy.next;
}

/* 整表释放：C 里必须自己做，否则内存泄漏 */
static void free_all(Node *head) {
    while (head != NULL) {
        Node *next = head->next;
        free(head);
        head = next;
    }
}

int main(void) {
    Node *list = NULL;

    list = push_front(list, 3);
    list = push_front(list, 2);
    list = push_front(list, 1);
    print_all("[手写] 头插 3,2,1", list);

    list = push_back(list, 4);
    print_all("[手写] 尾插 4", list);

    int pos = index_of(list, 3);
    printf("[手写] 查找 3: 找到, 第 %d 个\n", pos);
    printf("[手写] 查找 9: %s\n", index_of(list, 9) == 0 ? "未找到" : "找到");

    list = erase(list, 2);
    print_all("[手写] 删除 2", list);

    list = reverse(list);
    print_all("[手写] 反转", list);

    printf("[手写] 找中点: %d\n", middle(list)->value);

    printf("[手写] 判环(无环): %s\n", has_cycle(list) ? "有环" : "无环");
    Node *last = list;
    while (last->next != NULL) last = last->next;
    last->next = list;                       /* 人为造一个环 */
    printf("[手写] 判环(有环): %s\n", has_cycle(list) ? "有环" : "无环");
    last->next = NULL;                       /* 拆掉环，否则释放会出错 */

    printf("[手写] 长度: %d\n", length(list));

    Node *a = NULL;
    Node *b = NULL;
    a = push_back(a, 1); a = push_back(a, 3); a = push_back(a, 5);
    b = push_back(b, 2); b = push_back(b, 4); b = push_back(b, 6);
    Node *merged = merge_sorted(a, b);
    print_all("[手写] 合并两条有序链表", merged);

    free_all(list);
    free_all(merged);
    return 0;
}
