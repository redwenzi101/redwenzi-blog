// =====================================================================
// 数据结构与算法 · 第 3 章 数组、字符串与二分查找 · 二分查找（C++）
// 编译运行： g++ -std=c++17 -Wall -Wextra -Wpedantic -o app binary_search.cpp && ./app
//
// 三种写法都要会，因为边界处理不同，面试最爱在这里挑错：
//   ① 闭区间 [lo, hi]    ② 左闭右开 [lo, hi)    ③ lower_bound
//
// 复杂度（时间 / 空间）
//   二分查找 O(log n) / O(1)      二分答案 O(log(答案范围) × 判定代价) / O(1)
// =====================================================================

#include <cstdio>

constexpr int N = 1000;

int a[N];

// ① 闭区间 [lo, hi]：hi 取 n-1
int bsearch_closed(const int *arr, int n, int target, long *cmp) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        ++(*cmp);
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

// ② 左闭右开 [lo, hi)：hi 取 n
int bsearch_half_open(const int *arr, int n, int target, long *cmp) {
    int lo = 0, hi = n;
    while (lo < hi) {
        ++(*cmp);
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return -1;
}

// ③ lower_bound：第一个 >= target 的下标（可能返回 n）
int lower_bound_idx(const int *arr, int n, int target, long *cmp) {
    int lo = 0, hi = n;
    while (lo < hi) {
        ++(*cmp);
        int mid = lo + (hi - lo) / 2;
        if (arr[mid] < target) lo = mid + 1;
        else hi = mid;
    }
    return lo;
}

// 二分答案：假设每段长 len，能切出多少段
long cut_count(const int *wood, int k, int len) {
    long seg = 0;
    for (int i = 0; i < k; ++i) seg += wood[i] / len;
    return seg;
}

// 二分答案：求最大的等长 L，使切出的段数 >= need
int max_cut_length(const int *wood, int k, long need, long *checks) {
    int lo = 1, hi = 0;
    for (int i = 0; i < k; ++i) if (wood[i] > hi) hi = wood[i];
    int ans = 0;
    while (lo <= hi) {
        ++(*checks);
        int mid = lo + (hi - lo) / 2;
        if (cut_count(wood, k, mid) >= need) { ans = mid; lo = mid + 1; }
        else hi = mid - 1;
    }
    return ans;
}

int main() {
    for (int i = 0; i < N; ++i) a[i] = i * 3;      // 有序：0, 3, 6, ... 2997

    long cmp1 = 0, cmp2 = 0, cmp3 = 0, cmp4 = 0;
    int i1 = bsearch_closed(a, N, 1800, &cmp1);
    int i2 = bsearch_half_open(a, N, 1800, &cmp2);
    int i3 = lower_bound_idx(a, N, 1800, &cmp3);
    int i4 = lower_bound_idx(a, N, 1801, &cmp4);

    int empty[1] = {0};
    int one[1] = {5};
    long tmp = 0;
    int e1 = bsearch_closed(empty, 0, 5, &tmp);
    int e2 = bsearch_closed(one, 1, 5, &tmp);
    int e3 = bsearch_closed(one, 1, 9, &tmp);
    int e4 = bsearch_closed(a, N, 0, &tmp);
    int e5 = bsearch_closed(a, N, 2997, &tmp);
    int e6 = bsearch_closed(a, N, 1801, &tmp);

    int wood[3] = {10, 24, 16};
    long checks = 0;
    int best = max_cut_length(wood, 3, 7, &checks);

    std::printf("[二分] 有序数组 n = %d（a[i] = i × 3）\n", N);
    std::printf("[二分] 闭区间 [lo, hi]    找 1800: 找到, 下标 %d, 比较 %ld 次\n", i1, cmp1);
    std::printf("[二分] 左闭右开 [lo, hi)  找 1800: 找到, 下标 %d, 比较 %ld 次\n", i2, cmp2);
    std::printf("[二分] lower_bound 找 1800: 下标 %d, 比较 %ld 次\n", i3, cmp3);
    std::printf("[二分] lower_bound 找 1801: 下标 %d, 比较 %ld 次\n", i4, cmp4);
    std::printf("[二分] 边界用例:\n");
    std::printf("[二分]   空数组找 5: 返回 %d\n", e1);
    std::printf("[二分]   单元素 [5] 找 5: 返回 %d\n", e2);
    std::printf("[二分]   单元素 [5] 找 9: 返回 %d\n", e3);
    std::printf("[二分]   找第一个元素 0: 返回 %d\n", e4);
    std::printf("[二分]   找最后一个元素 2997: 返回 %d\n", e5);
    std::printf("[二分]   找不存在的 1801: 返回 %d\n", e6);
    std::printf("[二分答案] 切木头: 长度 [10, 24, 16]，要切成至少 7 段等长\n");
    std::printf("[二分答案] 最大等长 = %d, 判定 %ld 次\n", best, checks);
    std::printf("[二分答案] 验证: L=6 得 %ld 段（够），L=7 得 %ld 段（不够）\n",
                cut_count(wood, 3, 6), cut_count(wood, 3, 7));
    return 0;
}
