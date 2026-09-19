// =====================================================================
// 数据结构与算法 · 第 2 章 复杂度与算法分析 · 操作次数实测（C++）
// 编译运行： g++ -std=c++17 -Wall -Wextra -Wpedantic -o app count_ops.cpp && ./app
//
// 本章的代码是「测量工具」，所以四门语言用同一套手写逻辑，
// 数出来的操作次数完全一致，方便横向对照。
//
// 复杂度（时间 / 空间）
//   常数 O(1)/O(1)  二分 O(log n)/O(1)  遍历 O(n)/O(1)
//   归并排序 O(n log n)/O(n)  双重循环 O(n^2)/O(1)  递归求和 O(n)/O(n)
// =====================================================================

#include <cstdio>

constexpr int N = 1000;
constexpr int HASH_SIZE = 1024;     // 2^10；故意取小，让冲突真的发生，才看得到探测次数

int data_[N];
int sorted_arr[N];
int tmp[N];
int hash_table[HASH_SIZE];

long cmp_count = 0;
long probe_count = 0;

// 固定种子的伪随机数列：保证每次运行、每门语言拿到的数据完全一样
void lcg_fill(int *a, int n) {
    unsigned long long x = 12345ULL;
    for (int i = 0; i < n; ++i) {
        x = (x * 1103515245ULL + 12345ULL) % 2147483648ULL;
        a[i] = static_cast<int>(x % 2000ULL);
    }
}

// 归并排序：顺手统计比较次数，O(n log n)，额外空间 O(n)
void merge_sort(int *a, int *buffer, int l, int r) {
    if (r - l <= 1) return;
    int mid = l + (r - l) / 2;
    merge_sort(a, buffer, l, mid);
    merge_sort(a, buffer, mid, r);
    int i = l, j = mid, k = l;
    while (i < mid && j < r) {
        ++cmp_count;
        if (a[i] <= a[j]) buffer[k++] = a[i++];
        else buffer[k++] = a[j++];
    }
    while (i < mid) buffer[k++] = a[i++];
    while (j < r) buffer[k++] = a[j++];
    for (int t = l; t < r; ++t) a[t] = buffer[t];
}

// 二分查找：返回比较次数，O(log n)
int binary_search_compares(const int *a, int n, int target) {
    int lo = 0, hi = n - 1, count = 0;
    while (lo <= hi) {
        ++count;
        int mid = lo + (hi - lo) / 2;
        if (a[mid] == target) break;
        if (a[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return count;
}

// 简单开放寻址哈希表：插入成功返回 1（新元素），已存在返回 0
void hash_clear() {
    for (int i = 0; i < HASH_SIZE; ++i) hash_table[i] = -1;
}

int hash_insert(int v) {
    unsigned h = static_cast<unsigned>(v) & (HASH_SIZE - 1);
    for (;;) {
        ++probe_count;
        if (hash_table[h] == -1) { hash_table[h] = v; return 1; }
        if (hash_table[h] == v) return 0;
        h = (h + 1) & (HASH_SIZE - 1);
    }
}

// 递归求和：返回值一路带出递归深度，用来看「空间复杂度里的递归栈」
int rec_sum_depth(int n, int depth) {
    if (n == 0) return depth;
    return rec_sum_depth(n - 1, depth + 1);
}

int main() {
    lcg_fill(data_, N);

    long pairs = 0;                                    // O(n^2)
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) ++pairs;

    long sum = 0;                                      // O(n)
    for (int i = 0; i < N; ++i) sum += data_[i];

    for (int i = 0; i < N; ++i) sorted_arr[i] = data_[i];
    cmp_count = 0;
    merge_sort(sorted_arr, tmp, 0, N);                  // O(n log n)
    long sort_compares = cmp_count;

    int logn = binary_search_compares(sorted_arr, N, sorted_arr[N - 1]);   // O(log n)

    long brute_compares = 0;
    int unique_brute = 0;
    for (int i = 0; i < N; ++i) {
        int seen = 0;
        for (int j = 0; j < i; ++j) {
            ++brute_compares;
            if (data_[j] == data_[i]) { seen = 1; break; }
        }
        if (!seen) ++unique_brute;
    }

    int unique_sorted = 1;
    long adjacent_compares = 0;
    for (int i = 1; i < N; ++i) {
        ++adjacent_compares;
        if (sorted_arr[i] != sorted_arr[i - 1]) ++unique_sorted;
    }

    hash_clear();
    probe_count = 0;
    int unique_hash = 0;
    for (int i = 0; i < N; ++i) unique_hash += hash_insert(data_[i]);
    long probes = probe_count;

    std::printf("[计数] 数据规模 n = %d\n", N);
    std::printf("[计数] O(1)       常数操作: 1 次\n");
    std::printf("[计数] O(log n)   二分查找比较: %d 次\n", logn);
    std::printf("[计数] O(n)       遍历累加: %d 次, 和 = %ld\n", N, sum);
    std::printf("[计数] O(n log n) 归并排序比较: %ld 次\n", sort_compares);
    std::printf("[计数] O(n^2)     双重循环: %ld 次\n", pairs);
    std::printf("[计数] 增长对比: O(n^2) 是 O(n log n) 的 %.1f 倍, 是 O(n) 的 %.1f 倍\n",
                static_cast<double>(pairs) / static_cast<double>(sort_compares),
                static_cast<double>(pairs) / static_cast<double>(N));
    std::printf("[去重] 不重复元素: %d 个（暴力 %d / 排序 %d / 哈希 %d，三种结果一致）\n",
                unique_hash, unique_brute, unique_sorted, unique_hash);
    std::printf("[去重] 暴力双重循环: %ld 次比较, 额外空间 O(1)\n", brute_compares);
    std::printf("[去重] 排序后相邻比较: %ld 次（不含排序的 %ld 次）, 额外空间 O(n)\n",
                adjacent_compares, sort_compares);
    std::printf("[去重] 哈希表: %ld 次探测 = %d 次插入 + %ld 次冲突, 额外空间 O(n)\n",
                probes, N, probes - N);
    std::printf("[递归] 递归求和 n=100: 栈深度 %d 层；迭代版: 1 层\n", rec_sum_depth(100, 0));
    return 0;
}
