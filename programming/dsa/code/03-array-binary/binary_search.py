# =====================================================================
# 数据结构与算法 · 第 3 章 数组、字符串与二分查找 · 二分查找（Python）
# 运行： python binary_search.py
#
# 三种写法都要会，因为边界处理不同，面试最爱在这里挑错：
#   ① 闭区间 [lo, hi]    ② 左闭右开 [lo, hi)    ③ lower_bound
#
# 复杂度（时间 / 空间）
#   二分查找 O(log n) / O(1)      二分答案 O(log(答案范围) × 判定代价) / O(1)
# =====================================================================

N = 1000


def bsearch_closed(arr, n, target):
    """① 闭区间 [lo, hi]：hi 取 n-1，返回 (下标, 比较次数)"""
    lo, hi, cmp_count = 0, n - 1, 0
    while lo <= hi:
        cmp_count += 1
        mid = lo + (hi - lo) // 2
        if arr[mid] == target:
            return mid, cmp_count
        if arr[mid] < target:
            lo = mid + 1
        else:
            hi = mid - 1
    return -1, cmp_count


def bsearch_half_open(arr, n, target):
    """② 左闭右开 [lo, hi)：hi 取 n，返回 (下标, 比较次数)"""
    lo, hi, cmp_count = 0, n, 0
    while lo < hi:
        cmp_count += 1
        mid = lo + (hi - lo) // 2
        if arr[mid] == target:
            return mid, cmp_count
        if arr[mid] < target:
            lo = mid + 1
        else:
            hi = mid
    return -1, cmp_count


def lower_bound_idx(arr, n, target):
    """③ 第一个 >= target 的下标（可能返回 n），返回 (下标, 比较次数)"""
    lo, hi, cmp_count = 0, n, 0
    while lo < hi:
        cmp_count += 1
        mid = lo + (hi - lo) // 2
        if arr[mid] < target:
            lo = mid + 1
        else:
            hi = mid
    return lo, cmp_count


def cut_count(wood, k, length):
    """二分答案：假设每段长 length，能切出多少段"""
    seg = 0
    for i in range(k):
        seg += wood[i] // length
    return seg


def max_cut_length(wood, k, need):
    """二分答案：求最大的等长 L，使切出的段数 >= need，返回 (L, 判定次数)"""
    lo, hi = 1, max(wood[:k])
    ans, checks = 0, 0
    while lo <= hi:
        checks += 1
        mid = lo + (hi - lo) // 2
        if cut_count(wood, k, mid) >= need:
            ans = mid
            lo = mid + 1
        else:
            hi = mid - 1
    return ans, checks


def main():
    a = [i * 3 for i in range(N)]      # 有序：0, 3, 6, ... 2997

    i1, cmp1 = bsearch_closed(a, N, 1800)
    i2, cmp2 = bsearch_half_open(a, N, 1800)
    i3, cmp3 = lower_bound_idx(a, N, 1800)
    i4, cmp4 = lower_bound_idx(a, N, 1801)

    empty = [0]
    one = [5]
    e1, _ = bsearch_closed(empty, 0, 5)
    e2, _ = bsearch_closed(one, 1, 5)
    e3, _ = bsearch_closed(one, 1, 9)
    e4, _ = bsearch_closed(a, N, 0)
    e5, _ = bsearch_closed(a, N, 2997)
    e6, _ = bsearch_closed(a, N, 1801)

    wood = [10, 24, 16]
    best, checks = max_cut_length(wood, 3, 7)

    print("[二分] 有序数组 n = %d（a[i] = i × 3）" % N)
    print("[二分] 闭区间 [lo, hi]    找 1800: 找到, 下标 %d, 比较 %d 次" % (i1, cmp1))
    print("[二分] 左闭右开 [lo, hi)  找 1800: 找到, 下标 %d, 比较 %d 次" % (i2, cmp2))
    print("[二分] lower_bound 找 1800: 下标 %d, 比较 %d 次" % (i3, cmp3))
    print("[二分] lower_bound 找 1801: 下标 %d, 比较 %d 次" % (i4, cmp4))
    print("[二分] 边界用例:")
    print("[二分]   空数组找 5: 返回 %d" % e1)
    print("[二分]   单元素 [5] 找 5: 返回 %d" % e2)
    print("[二分]   单元素 [5] 找 9: 返回 %d" % e3)
    print("[二分]   找第一个元素 0: 返回 %d" % e4)
    print("[二分]   找最后一个元素 2997: 返回 %d" % e5)
    print("[二分]   找不存在的 1801: 返回 %d" % e6)
    print("[二分答案] 切木头: 长度 [10, 24, 16]，要切成至少 7 段等长")
    print("[二分答案] 最大等长 = %d, 判定 %d 次" % (best, checks))
    print("[二分答案] 验证: L=6 得 %d 段（够），L=7 得 %d 段（不够）"
          % (cut_count(wood, 3, 6), cut_count(wood, 3, 7)))


if __name__ == "__main__":
    main()
