# =====================================================================
# 数据结构与算法 · 第 2 章 复杂度与算法分析 · 操作次数实测（Python）
# 运行： python count_ops.py
#
# 本章的代码是「测量工具」，所以四门语言用同一套手写逻辑，
# 数出来的操作次数完全一致，方便横向对照。
#
# 复杂度（时间 / 空间）
#   常数 O(1)/O(1)  二分 O(log n)/O(1)  遍历 O(n)/O(1)
#   归并排序 O(n log n)/O(n)  双重循环 O(n^2)/O(1)  递归求和 O(n)/O(n)
# =====================================================================

import sys

N = 1000
HASH_SIZE = 1024            # 2^10；故意取小，让冲突真的发生，才看得到探测次数

data = [0] * N
sorted_arr = [0] * N
tmp = [0] * N
hash_table = [-1] * HASH_SIZE

cmp_count = 0
probe_count = 0


def lcg_fill(a, n):
    """固定种子的伪随机数列：保证每次运行、每门语言拿到的数据完全一样"""
    x = 12345
    for i in range(n):
        x = (x * 1103515245 + 12345) % 2147483648
        a[i] = x % 2000


def merge_sort(a, buffer, l, r):
    """归并排序：顺手统计比较次数，O(n log n)，额外空间 O(n)"""
    global cmp_count
    if r - l <= 1:
        return
    mid = l + (r - l) // 2
    merge_sort(a, buffer, l, mid)
    merge_sort(a, buffer, mid, r)
    i, j, k = l, mid, l
    while i < mid and j < r:
        cmp_count += 1
        if a[i] <= a[j]:
            buffer[k] = a[i]
            i += 1
        else:
            buffer[k] = a[j]
            j += 1
        k += 1
    while i < mid:
        buffer[k] = a[i]
        i += 1
        k += 1
    while j < r:
        buffer[k] = a[j]
        j += 1
        k += 1
    for t in range(l, r):
        a[t] = buffer[t]


def binary_search_compares(a, n, target):
    """二分查找：返回比较次数，O(log n)"""
    lo, hi, count = 0, n - 1, 0
    while lo <= hi:
        count += 1
        mid = lo + (hi - lo) // 2
        if a[mid] == target:
            break
        if a[mid] < target:
            lo = mid + 1
        else:
            hi = mid - 1
    return count


def hash_clear():
    for i in range(HASH_SIZE):
        hash_table[i] = -1


def hash_insert(v):
    """简单开放寻址哈希表：插入成功返回 1（新元素），已存在返回 0"""
    global probe_count
    h = v & (HASH_SIZE - 1)
    while True:
        probe_count += 1
        if hash_table[h] == -1:
            hash_table[h] = v
            return 1
        if hash_table[h] == v:
            return 0
        h = (h + 1) & (HASH_SIZE - 1)


def rec_sum_depth(n, depth):
    """递归求和：返回值一路带出递归深度，用来看「空间复杂度里的递归栈」"""
    if n == 0:
        return depth
    return rec_sum_depth(n - 1, depth + 1)


def main():
    global cmp_count, probe_count

    lcg_fill(data, N)

    pairs = 0                                   # O(n^2)
    for i in range(N):
        for j in range(i + 1, N):
            pairs += 1

    total = 0                                   # O(n)
    for i in range(N):
        total += data[i]

    for i in range(N):
        sorted_arr[i] = data[i]
    cmp_count = 0
    merge_sort(sorted_arr, tmp, 0, N)            # O(n log n)
    sort_compares = cmp_count

    logn = binary_search_compares(sorted_arr, N, sorted_arr[N - 1])   # O(log n)

    brute_compares = 0
    unique_brute = 0
    for i in range(N):
        seen = False
        for j in range(i):
            brute_compares += 1
            if data[j] == data[i]:
                seen = True
                break
        if not seen:
            unique_brute += 1

    unique_sorted = 1
    adjacent_compares = 0
    for i in range(1, N):
        adjacent_compares += 1
        if sorted_arr[i] != sorted_arr[i - 1]:
            unique_sorted += 1

    hash_clear()
    probe_count = 0
    unique_hash = 0
    for i in range(N):
        unique_hash += hash_insert(data[i])
    probes = probe_count

    print("[计数] 数据规模 n = %d" % N)
    print("[计数] O(1)       常数操作: 1 次")
    print("[计数] O(log n)   二分查找比较: %d 次" % logn)
    print("[计数] O(n)       遍历累加: %d 次, 和 = %d" % (N, total))
    print("[计数] O(n log n) 归并排序比较: %d 次" % sort_compares)
    print("[计数] O(n^2)     双重循环: %d 次" % pairs)
    print("[计数] 增长对比: O(n^2) 是 O(n log n) 的 %.1f 倍, 是 O(n) 的 %.1f 倍"
          % (pairs / sort_compares, pairs / N))
    print("[去重] 不重复元素: %d 个（暴力 %d / 排序 %d / 哈希 %d，三种结果一致）"
          % (unique_hash, unique_brute, unique_sorted, unique_hash))
    print("[去重] 暴力双重循环: %d 次比较, 额外空间 O(1)" % brute_compares)
    print("[去重] 排序后相邻比较: %d 次（不含排序的 %d 次）, 额外空间 O(n)"
          % (adjacent_compares, sort_compares))
    print("[去重] 哈希表: %d 次探测 = %d 次插入 + %d 次冲突, 额外空间 O(n)"
          % (probes, N, probes - N))
    print("[递归] 递归求和 n=100: 栈深度 %d 层；迭代版: 1 层" % rec_sum_depth(100, 0))


if __name__ == "__main__":
    sys.setrecursionlimit(10000)
    main()
