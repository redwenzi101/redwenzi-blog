/* =====================================================================
   数据结构与算法 · 第 2 章 复杂度与算法分析 · 均摊分析（C）
   编译运行： gcc -std=c17 -Wall -Wextra -o app amortized.c && ./app

   要回答的问题：动态数组（vector / ArrayList / list）每次插入都要扩容吗？
   答案：不。容量翻倍时，n 次插入的总拷贝次数是 O(n)，平均到每次是 O(1)，
        这个「最坏情况偶尔很贵、平均下来很便宜」的分析方法就叫均摊分析。

   复杂度（时间 / 空间）
     翻倍扩容：总拷贝 O(n)，均摊到每次插入 O(1)，额外空间 O(1)
     每次加 1：总拷贝 O(n^2)，均摊到每次插入 O(n)，额外空间 O(1)
   ===================================================================== */

#include <stdio.h>

/* 容量翻倍：容量满时扩容并拷贝旧数据，返回 n 次插入的总拷贝次数 */
static long simulate_doubling(int n) {
    int cap = 1, size = 0;
    long copies = 0;
    for (int i = 0; i < n; ++i) {
        if (size == cap) { copies += size; cap *= 2; }
        size++;
    }
    return copies;
}

/* 每次只加 1：容量满时只多扩一个位置，返回总拷贝次数 */
static long simulate_plus_one(int n) {
    int cap = 1, size = 0;
    long copies = 0;
    for (int i = 0; i < n; ++i) {
        if (size == cap) { copies += size; cap += 1; }
        size++;
    }
    return copies;
}

static void report(int n) {
    long doubling = simulate_doubling(n);
    long plusone = simulate_plus_one(n);
    printf("[均摊] n = %d\n", n);
    printf("[均摊]   翻倍扩容: 总拷贝 %ld 次, 平均 %.2f 次/插入\n",
           doubling, (double)doubling / (double)n);
    printf("[均摊]   每次加 1: 总拷贝 %ld 次, 平均 %.2f 次/插入\n",
           plusone, (double)plusone / (double)n);
}

int main(void) {
    report(16);
    report(1024);
    printf("[均摊] 结论: 翻倍扩容总拷贝 O(n)（均摊 O(1)），每次加 1 是 O(n^2)（均摊 O(n)）\n");
    return 0;
}
