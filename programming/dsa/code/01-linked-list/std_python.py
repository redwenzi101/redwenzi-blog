# =====================================================================
# 数据结构与算法 · 第 1 章 链表 · 标准库写法（Python / collections.deque）
# 运行： python std_python.py
#
# 复杂度速查（时间 / 空间）
#   头插 appendleft O(1) / O(1)     尾插 append O(1) / O(1)
#   查找 in O(n) / O(1)             删除 remove O(n) / O(1)
#   反转 reverse O(n) / O(1)        合并两条有序链表 O(n+m) / O(n+m)
#   按索引取第 k 个 d[k] O(k) / O(1)   ← 链表没有随机访问
# 说明：Python 没有内置链表。list 是动态数组，索引访问是 O(1)；
#      要按链表方式用，应该用 collections.deque（两端插入删除都是 O(1)），
#      它的索引访问是 O(n)，正好能体现链表"查得慢、插得快"的特点。
# =====================================================================

from collections import deque


def print_all(prefix, d):
    print(prefix + ": " + " ".join(str(v) for v in d))


def merge_sorted(a, b):
    """两条有序链表合并：每次取更小的那个队头，O(n+m)"""
    a = deque(a)
    b = deque(b)
    out = deque()
    while a and b:
        if a[0] <= b[0]:
            out.append(a.popleft())
        else:
            out.append(b.popleft())
    out.extend(a)
    out.extend(b)
    return out


def main():
    d = deque()

    d.appendleft(3)
    d.appendleft(2)
    d.appendleft(1)
    print_all("[标准库] 头插 3,2,1", d)

    d.append(4)
    print_all("[标准库] 尾插 4", d)

    print("[标准库] 查找 3: %s" % ("找到" if 3 in d else "未找到"))
    print("[标准库] 查找 9: %s" % ("找到" if 9 in d else "未找到"))

    d.remove(2)
    print_all("[标准库] 删除 2", d)

    d.reverse()
    print_all("[标准库] 反转", d)

    print("[标准库] 长度: %d" % len(d))

    print_all("[标准库] 合并两条有序链表", merge_sorted(deque([1, 3, 5]), deque([2, 4, 6])))

    print("[标准库] 按索引取第 2 个: %d" % d[1])


if __name__ == "__main__":
    main()
