/* =====================================================================
   数据结构与算法 · 第 3 章 数组、字符串与二分查找 · 数组代价实测（C）
   编译运行： gcc -std=c17 -Wall -Wextra -o app array_cost.c && ./app

   本章要回答三个问题：
     ① 数组的「随机访问快、中间插入慢」到底体现在哪？——数元素移动次数；
     ② 区间求和为什么能做到 O(1)？——前缀和，预处理 O(n)、查询 O(1)；
     ③ 区间批量加为什么也能做到 O(1)？——差分数组，修改 O(1)、还原 O(n)。

   复杂度（时间 / 空间）
     随机访问 O(1) / O(1)
     尾部追加（容量够）O(1) / O(1)      头部插入 / 删除 O(n) / O(1)
     前缀和：预处理 O(n) / O(n)          区间查询 O(1) / O(1)
     差分：区间加 O(1) / O(n)            还原 O(n) / O(1)
   ===================================================================== */

#include <stdio.h>

#define N 1000
#define L 200
#define R 800

static int data[N];
static int buf[N + 2];          /* 留两格余量：头部插入 1 个 + 尾部插入 1 个 */
static long prefix[N + 1];      /* prefix[i] = data[0..i) 的和 */
static long diff[N + 1];

/* 固定种子的伪随机数列：与第 2 章同一个种子，方便对照 */
static void lcg_fill(int *a, int n) {
    unsigned long long x = 12345ULL;
    for (int i = 0; i < n; ++i) {
        x = (x * 1103515245ULL + 12345ULL) % 2147483648ULL;
        a[i] = (int)(x % 2000ULL);
    }
}

int main(void) {
    lcg_fill(data, N);

    /* ---- ① 数组的四种基本操作的代价 ---- */
    int random_access = data[N / 2];                 /* 随机访问：1 次取值 */

    for (int i = 0; i < N; ++i) buf[i] = data[i];    /* 复制一份来演示插入删除 */
    int size = N;

    int tail_appended = data[N - 1];                 /* 尾部追加（容量够）：不移动元素 */

    /* 头部插入：先把 1000 个元素整体右移一格 */
    long head_insert_moves = 0;
    for (int i = size; i > 0; --i) { buf[i] = buf[i - 1]; head_insert_moves++; }
    buf[0] = 42;
    size++;

    /* 尾部插入（需要扩容）：先把已有元素整体搬到新数组 */
    long tail_insert_moves = 0;
    for (int i = 0; i < size; ++i) tail_insert_moves++;   /* 扩容时要拷贝的元素个数 */
    buf[size] = 43;
    size++;

    /* 删除头部：把后面 999 个元素整体左移一格 */
    long head_delete_moves = 0;
    for (int i = 0; i + 1 < size; ++i) { buf[i] = buf[i + 1]; head_delete_moves++; }
    size--;

    int tail_deleted_moves = 0;                      /* 删除尾部：不移动元素 */
    size--;

    /* ---- ② 前缀和：预处理一次，之后每次区间求和都是 O(1) ---- */
    long prefix_adds = 0;
    prefix[0] = 0;
    for (int i = 0; i < N; ++i) { prefix[i + 1] = prefix[i] + data[i]; prefix_adds++; }

    long range_sum_fast = prefix[R] - prefix[L];      /* 1 次减法 */
    long range_sum_brute = 0;
    long brute_adds = 0;
    for (int i = L; i < R; ++i) { range_sum_brute += data[i]; brute_adds++; }

    /* ---- ③ 差分数组：区间批量加只需改两个位置 ---- */
    for (int i = 0; i <= N; ++i) diff[i] = 0;
    diff[L] += 5;
    diff[R] -= 5;
    long diff_changes = 2;

    long restore_adds = 0;
    long cur = 0;
    long restored_range_sum = 0;
    for (int i = 0; i < N; ++i) {
        cur += diff[i];
        restore_adds++;
        if (i >= L && i < R) restored_range_sum += data[i] + cur;
    }

    printf("[数组] 数据规模 n = %d\n", N);
    printf("[数组] 随机访问第 %d 个: %d（1 次取值）, O(1)\n", N / 2, random_access);
    printf("[数组] 尾部追加（容量够）: 移动 0 个元素（读到 %d）, O(1)\n", tail_appended);
    printf("[数组] 头部插入: 移动 %ld 个元素, O(n)\n", head_insert_moves);
    printf("[数组] 尾部插入（触发扩容）: 拷贝 %ld 个元素, O(n)\n", tail_insert_moves);
    printf("[数组] 删除头部: 移动 %ld 个元素, O(n)\n", head_delete_moves);
    printf("[数组] 删除尾部: 移动 %d 个元素, O(1)\n", tail_deleted_moves);
    printf("[前缀和] 预处理 %d 个元素: %ld 次加法, O(n)\n", N, prefix_adds);
    printf("[前缀和] 区间 [%d, %d) 求和: 1 次减法 = %ld, O(1)\n", L, R, range_sum_fast);
    printf("[前缀和] 同区间暴力求和: %ld 次加法 = %ld, O(n)\n", brute_adds, range_sum_brute);
    printf("[前缀和] 两者结果一致: %s\n", range_sum_fast == range_sum_brute ? "是" : "否");
    printf("[差分] 区间 [%d, %d) 批量加 5: %ld 次修改, O(1)\n", L, R, diff_changes);
    printf("[差分] 从差分数组还原: %ld 次加法, O(n)\n", restore_adds);
    printf("[差分] 还原后该区间和 = %ld（原区间和 %ld + %d×5）\n",
           restored_range_sum, range_sum_brute, R - L);
    return 0;
}
