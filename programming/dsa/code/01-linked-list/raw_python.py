# =====================================================================
# 数据结构与算法 · 第 1 章 链表 · 手写实现（Python）
# 运行： python raw_python.py
#
# 复杂度速查（时间 / 空间）
#   头插 O(1) / O(1)          尾插 O(n) / O(1)
#   查找 O(n) / O(1)          删除 O(n) / O(1)    （单链表要重新找前驱）
#   反转 O(n) / O(1)          找中点 O(n) / O(1)
#   判环 O(n) / O(1)          合并两条有序链表 O(n+m) / O(1)
# 说明：Python 没有指针，这里用类 + 对象引用模拟结点，
#      逻辑与 C / C++ / Java 版完全一致，便于横向对照。
# =====================================================================


class Node:
    __slots__ = ("value", "next")

    def __init__(self, value, nxt=None):
        self.value = value
        self.next = nxt


def push_front(head, value):
    """头插：新结点接到表头，O(1)"""
    return Node(value, head)


def push_back(head, value):
    """尾插：先走到最后一个结点，O(n)"""
    node = Node(value)
    if head is None:
        return node
    cur = head
    while cur.next is not None:
        cur = cur.next
    cur.next = node
    return head


def print_all(prefix, head):
    """遍历打印：以 "prefix: 1 2 3" 的形式输出"""
    parts = []
    cur = head
    while cur is not None:
        parts.append(str(cur.value))
        cur = cur.next
    print(prefix + ": " + " ".join(parts) if parts else prefix + ":")


def index_of(head, value):
    """查找：返回第几个（从 1 开始），找不到返回 0，O(n)"""
    i = 1
    cur = head
    while cur is not None:
        if cur.value == value:
            return i
        cur = cur.next
        i += 1
    return 0


def erase(head, value):
    """删除第一个匹配的结点，O(n)"""
    prev = None
    cur = head
    while cur is not None:
        if cur.value == value:
            if prev is None:
                return cur.next
            prev.next = cur.next
            return head
        prev = cur
        cur = cur.next
    return head


def reverse(head):
    """反转：三指针迭代，O(n) 时间、O(1) 空间"""
    prev = None
    cur = head
    while cur is not None:
        nxt = cur.next
        cur.next = prev
        prev = cur
        cur = nxt
    return prev


def middle(head):
    """找中点：快慢指针，O(n)"""
    slow = head
    fast = head
    while fast is not None and fast.next is not None:
        slow = slow.next
        fast = fast.next.next
    return slow


def has_cycle(head):
    """判环：Floyd 快慢指针，O(n)"""
    slow = head
    fast = head
    while fast is not None and fast.next is not None:
        slow = slow.next
        fast = fast.next.next
        if slow is fast:
            return True
    return False


def length(head):
    """长度：O(n)"""
    n = 0
    cur = head
    while cur is not None:
        n += 1
        cur = cur.next
    return n


def merge_sorted(a, b):
    """合并两条有序链表：双指针穿针引线，O(n+m)"""
    dummy = Node(0)
    tail = dummy
    while a is not None and b is not None:
        if a.value <= b.value:
            tail.next = a
            a = a.next
        else:
            tail.next = b
            b = b.next
        tail = tail.next
    tail.next = a if a is not None else b
    return dummy.next


def main():
    lst = None

    lst = push_front(lst, 3)
    lst = push_front(lst, 2)
    lst = push_front(lst, 1)
    print_all("[手写] 头插 3,2,1", lst)

    lst = push_back(lst, 4)
    print_all("[手写] 尾插 4", lst)

    print("[手写] 查找 3: 找到, 第 %d 个" % index_of(lst, 3))
    print("[手写] 查找 9: %s" % ("未找到" if index_of(lst, 9) == 0 else "找到"))

    lst = erase(lst, 2)
    print_all("[手写] 删除 2", lst)

    lst = reverse(lst)
    print_all("[手写] 反转", lst)

    print("[手写] 找中点: %d" % middle(lst).value)

    print("[手写] 判环(无环): %s" % ("有环" if has_cycle(lst) else "无环"))
    last = lst
    while last.next is not None:
        last = last.next
    last.next = lst                          # 人为造一个环
    print("[手写] 判环(有环): %s" % ("有环" if has_cycle(lst) else "无环"))
    last.next = None                         # 拆掉环

    print("[手写] 长度: %d" % length(lst))

    a = None
    b = None
    a = push_back(a, 1); a = push_back(a, 3); a = push_back(a, 5)
    b = push_back(b, 2); b = push_back(b, 4); b = push_back(b, 6)
    merged = merge_sorted(a, b)
    print_all("[手写] 合并两条有序链表", merged)


if __name__ == "__main__":
    main()
