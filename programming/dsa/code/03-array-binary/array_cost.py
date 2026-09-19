# =====================================================================
# 数据结构与算法 · 第 3 章 数组、字符串与二分查找 · 数组代价实测（Python）
# 运行： python array_cost.py
#
# 复杂度（时间 / 空间）
#   随机访问 O(1) / O(1)            尾部追加（容量够）O(1) / O(1)
#   头部插入 / 删除 O(n) / O(1)     前缀和：预处理 O(n)、查询 O(1)
#   差分：区间加 O(1)、还原 O(n)
# =====================================================================

N = 1000
L = 200
R = 800


def lcg_fill(a, n):
    """固定种子的伪随机数列：与第 2 章同一个种子，方便对照"""
    x = 12345
    for i in range(n):
        x = (x * 1103515245 + 12345) % 2147483648
        a[i] = x % 2000


def main():
    data = [0] * N
    lcg_fill(data, N)

    random_access = data[N // 2]                 # 随机访问：1 次取值

    buf = data[:] + [0, 0]                       # 留两格余量：头部插入 1 个 + 尾部插入 1 个
    size = N

    tail_appended = data[N - 1]                  # 尾部追加（容量够）：不移动元素

    head_insert_moves = 0                        # 头部插入：整体右移
    for i in range(size, 0, -1):
        buf[i] = buf[i - 1]
        head_insert_moves += 1
    buf[0] = 42
    size += 1

    tail_insert_moves = 0                        # 尾部插入触发扩容：整体拷贝
    for i in range(size):
        tail_insert_moves += 1
    buf[size] = 43
    size += 1

    head_delete_moves = 0                        # 删除头部：整体左移
    for i in range(size - 1):
        buf[i] = buf[i + 1]
        head_delete_moves += 1
    size -= 1

    tail_deleted_moves = 0                       # 删除尾部：不移动元素
    size -= 1

    prefix = [0] * (N + 1)                       # 前缀和：预处理一次
    prefix_adds = 0
    for i in range(N):
        prefix[i + 1] = prefix[i] + data[i]
        prefix_adds += 1

    range_sum_fast = prefix[R] - prefix[L]       # 1 次减法
    range_sum_brute = 0
    brute_adds = 0
    for i in range(L, R):
        range_sum_brute += data[i]
        brute_adds += 1

    diff = [0] * (N + 1)                         # 差分数组：区间加
    diff[L] += 5
    diff[R] -= 5
    diff_changes = 2

    restore_adds = 0
    cur = 0
    restored_range_sum = 0
    for i in range(N):
        cur += diff[i]
        restore_adds += 1
        if L <= i < R:
            restored_range_sum += data[i] + cur

    print("[数组] 数据规模 n = %d" % N)
    print("[数组] 随机访问第 %d 个: %d（1 次取值）, O(1)" % (N // 2, random_access))
    print("[数组] 尾部追加（容量够）: 移动 0 个元素（读到 %d）, O(1)" % tail_appended)
    print("[数组] 头部插入: 移动 %d 个元素, O(n)" % head_insert_moves)
    print("[数组] 尾部插入（触发扩容）: 拷贝 %d 个元素, O(n)" % tail_insert_moves)
    print("[数组] 删除头部: 移动 %d 个元素, O(n)" % head_delete_moves)
    print("[数组] 删除尾部: 移动 %d 个元素, O(1)" % tail_deleted_moves)
    print("[前缀和] 预处理 %d 个元素: %d 次加法, O(n)" % (N, prefix_adds))
    print("[前缀和] 区间 [%d, %d) 求和: 1 次减法 = %d, O(1)" % (L, R, range_sum_fast))
    print("[前缀和] 同区间暴力求和: %d 次加法 = %d, O(n)" % (brute_adds, range_sum_brute))
    print("[前缀和] 两者结果一致: %s" % ("是" if range_sum_fast == range_sum_brute else "否"))
    print("[差分] 区间 [%d, %d) 批量加 5: %d 次修改, O(1)" % (L, R, diff_changes))
    print("[差分] 从差分数组还原: %d 次加法, O(n)" % restore_adds)
    print("[差分] 还原后该区间和 = %d（原区间和 %d + %d×5）"
          % (restored_range_sum, range_sum_brute, R - L))


if __name__ == "__main__":
    main()
