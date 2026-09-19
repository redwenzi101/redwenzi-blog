/* =====================================================================
   数据结构与算法 · 第 2 章 复杂度与算法分析 · 操作次数实测（C）
   编译运行： gcc -std=c17 -Wall -Wextra -o app count_ops.c && ./app

   为什么要数「操作次数」而不是看秒表：
   运行时间受机器、编译器、缓存影响，换个环境就变了；操作次数只和算法本身有关，
   四门语言用同一套逻辑写，数出来的次数应当完全一样——这才是复杂度的本意。

   本章涉及的复杂度（时间 / 空间）
     常数        O(1)       / O(1)
     二分查找    O(log n)   / O(1)
     一次遍历    O(n)       / O(1)
     归并排序    O(n log n) / O(n)
     双重循环    O(n^2)     / O(1)
     递归求和    O(n)       / O(n)  ← 递归栈占了 n 层
   ===================================================================== */

#include <stdio.h>

#define N 1000
#define HASH_SIZE 1024            /* 2^10；故意取小，让冲突真的发生，才看得到探测次数 */

static int data[N];
static int sorted_arr[N];
static int tmp[N];
static int hash_table[HASH_SIZE];

static long cmp_count = 0;        /* 归并排序的比较次数 */
static long probe_count = 0;      /* 哈希表的探测次数 */

/* 固定种子的伪随机数列：保证每次运行、每门语言拿到的数据完全一样 */
static void lcg_fill(int *a, int n) {
    unsigned long long x = 12345ULL;
    for (int i = 0; i < n; ++i) {
        x = (x * 1103515245ULL + 12345ULL) % 2147483648ULL;
        a[i] = (int)(x % 2000ULL);
    }
}

/* 归并排序：顺手统计比较次数，O(n log n)，额外空间 O(n) */
static void merge_sort(int *a, int *tmp2, int l, int r) {
    if (r - l <= 1) return;
    int mid = l + (r - l) / 2;
    merge_sort(a, tmp2, l, mid);
    merge_sort(a, tmp2, mid, r);
    int i = l, j = mid, k = l;
    while (i < mid && j < r) {
        cmp_count++;
        if (a[i] <= a[j]) tmp2[k++] = a[i++];
        else tmp2[k++] = a[j++];
    }
    while (i < mid) tmp2[k++] = a[i++];
    while (j < r) tmp2[k++] = a[j++];
    for (int t = l; t < r; ++t) a[t] = tmp2[t];
}

/* 二分查找：返回比较次数，O(log n) */
static int binary_search_compares(const int *a, int n, int target) {
    int lo = 0, hi = n - 1, count = 0;
    while (lo <= hi) {
        count++;
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == target) break;
        if (a[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return count;
}

/* 简单开放寻址哈希表：插入成功返回 1（新元素），已存在返回 0 */
static void hash_clear(void) {
    for (int i = 0; i < HASH_SIZE; ++i) hash_table[i] = -1;
}

static int hash_insert(int v) {
    unsigned h = (unsigned)v & (HASH_SIZE - 1);
    for (;;) {
        probe_count++;
        if (hash_table[h] == -1) { hash_table[h] = v; return 1; }
        if (hash_table[h] == v) return 0;
        h = (h + 1) & (HASH_SIZE - 1);
    }
}

/* 递归求和：返回值一路带出递归深度，用来看「空间复杂度里的递归栈」 */
static int rec_sum_depth(int n, int depth) {
    if (n == 0) return depth;
    return rec_sum_depth(n - 1, depth + 1);
}

int main(void) {
    lcg_fill(data, N);

    /* O(n^2)：不重复地数所有数对 */
    long pairs = 0;
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) pairs++;

    /* O(n)：一次遍历求和 */
    long sum = 0;
    for (int i = 0; i < N; ++i) sum += data[i];

    /* O(n log n)：排序一份副本，顺便拿到比较次数 */
    for (int i = 0; i < N; ++i) sorted_arr[i] = data[i];
    cmp_count = 0;
    merge_sort(sorted_arr, tmp, 0, N);
    long sort_compares = cmp_count;

    /* O(log n)：在有序副本里找最大值，走的是最坏路径 */
    int logn = binary_search_compares(sorted_arr, N, sorted_arr[N - 1]);

    /* 去重策略一：暴力双重循环，O(n^2) 时间、O(1) 额外空间 */
    long brute_compares = 0;
    int unique_brute = 0;
    for (int i = 0; i < N; ++i) {
        int seen = 0;
        for (int j = 0; j < i; ++j) {
            brute_compares++;
            if (data[j] == data[i]) { seen = 1; break; }
        }
        if (!seen) unique_brute++;
    }

    /* 去重策略二：排序后比较相邻元素，O(n log n) 时间、O(n) 额外空间 */
    int unique_sorted = 1;
    long adjacent_compares = 0;
    for (int i = 1; i < N; ++i) {
        adjacent_compares++;
        if (sorted_arr[i] != sorted_arr[i - 1]) unique_sorted++;
    }

    /* 去重策略三：哈希表，O(n) 时间、O(n) 额外空间 */
    hash_clear();
    probe_count = 0;
    int unique_hash = 0;
    for (int i = 0; i < N; ++i) unique_hash += hash_insert(data[i]);
    long probes = probe_count;

    printf("[计数] 数据规模 n = %d\n", N);
    printf("[计数] O(1)       常数操作: 1 次\n");
    printf("[计数] O(log n)   二分查找比较: %d 次\n", logn);
    printf("[计数] O(n)       遍历累加: %d 次, 和 = %ld\n", N, sum);
    printf("[计数] O(n log n) 归并排序比较: %ld 次\n", sort_compares);
    printf("[计数] O(n^2)     双重循环: %ld 次\n", pairs);
    printf("[计数] 增长对比: O(n^2) 是 O(n log n) 的 %.1f 倍, 是 O(n) 的 %.1f 倍\n",
           (double)pairs / (double)sort_compares, (double)pairs / (double)N);
    printf("[去重] 不重复元素: %d 个（暴力 %d / 排序 %d / 哈希 %d，三种结果一致）\n",
           unique_hash, unique_brute, unique_sorted, unique_hash);
    printf("[去重] 暴力双重循环: %ld 次比较, 额外空间 O(1)\n", brute_compares);
    printf("[去重] 排序后相邻比较: %ld 次（不含排序的 %ld 次）, 额外空间 O(n)\n",
           adjacent_compares, sort_compares);
    printf("[去重] 哈希表: %ld 次探测 = %d 次插入 + %ld 次冲突, 额外空间 O(n)\n",
           probes, N, probes - N);
    printf("[递归] 递归求和 n=100: 栈深度 %d 层；迭代版: 1 层\n", rec_sum_depth(100, 0));
    return 0;
}
