# =====================================================================
# 数据结构与算法 · 第 2 章 复杂度与算法分析 · 均摊分析（Python）
# 运行： python amortized.py
#
# 要回答的问题：list.append 每次都要搬家吗？
# 答案：CPython 的 list 也是按倍数扩容量，n 次 append 的总拷贝是 O(n)，
#      均摊到每次是 O(1)。
#
# 复杂度（时间 / 空间）
#   翻倍扩容：总拷贝 O(n)，均摊 O(1)，额外空间 O(1)
#   每次加 1：总拷贝 O(n^2)，均摊 O(n)，额外空间 O(1)
# =====================================================================


def simulate_doubling(n):
    """容量翻倍：容量满时扩容并拷贝旧数据，返回 n 次插入的总拷贝次数"""
    cap, size, copies = 1, 0, 0
    for _ in range(n):
        if size == cap:
            copies += size
            cap *= 2
        size += 1
    return copies


def simulate_plus_one(n):
    """每次只加 1：容量满时只多扩一个位置，返回总拷贝次数"""
    cap, size, copies = 1, 0, 0
    for _ in range(n):
        if size == cap:
            copies += size
            cap += 1
        size += 1
    return copies


def report(n):
    doubling = simulate_doubling(n)
    plus_one = simulate_plus_one(n)
    print("[均摊] n = %d" % n)
    print("[均摊]   翻倍扩容: 总拷贝 %d 次, 平均 %.2f 次/插入" % (doubling, doubling / n))
    print("[均摊]   每次加 1: 总拷贝 %d 次, 平均 %.2f 次/插入" % (plus_one, plus_one / n))


def main():
    report(16)
    report(1024)
    print("[均摊] 结论: 翻倍扩容总拷贝 O(n)（均摊 O(1)），每次加 1 是 O(n^2)（均摊 O(n)）")


if __name__ == "__main__":
    main()
