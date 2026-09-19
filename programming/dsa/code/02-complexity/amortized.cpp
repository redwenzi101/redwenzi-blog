// =====================================================================
// 数据结构与算法 · 第 2 章 复杂度与算法分析 · 均摊分析（C++）
// 编译运行： g++ -std=c++17 -Wall -Wextra -Wpedantic -o app amortized.cpp && ./app
//
// 要回答的问题：动态数组（vector）每次插入都要扩容吗？
// 答案：容量翻倍时，n 次插入的总拷贝是 O(n)，均摊到每次插入是 O(1)——
//      这就是 std::vector::push_back 号称「均摊 O(1)」的由来。
//
// 复杂度（时间 / 空间）
//   翻倍扩容：总拷贝 O(n)，均摊 O(1)，额外空间 O(1)
//   每次加 1：总拷贝 O(n^2)，均摊 O(n)，额外空间 O(1)
// =====================================================================

#include <cstdio>

// 容量翻倍：容量满时扩容并拷贝旧数据，返回 n 次插入的总拷贝次数
long simulate_doubling(int n) {
    int cap = 1, size = 0;
    long copies = 0;
    for (int i = 0; i < n; ++i) {
        if (size == cap) { copies += size; cap *= 2; }
        ++size;
    }
    return copies;
}

// 每次只加 1：容量满时只多扩一个位置，返回总拷贝次数
long simulate_plus_one(int n) {
    int cap = 1, size = 0;
    long copies = 0;
    for (int i = 0; i < n; ++i) {
        if (size == cap) { copies += size; cap += 1; }
        ++size;
    }
    return copies;
}

void report(int n) {
    long doubling = simulate_doubling(n);
    long plusone = simulate_plus_one(n);
    std::printf("[均摊] n = %d\n", n);
    std::printf("[均摊]   翻倍扩容: 总拷贝 %ld 次, 平均 %.2f 次/插入\n",
                doubling, static_cast<double>(doubling) / n);
    std::printf("[均摊]   每次加 1: 总拷贝 %ld 次, 平均 %.2f 次/插入\n",
                plusone, static_cast<double>(plusone) / n);
}

int main() {
    report(16);
    report(1024);
    std::printf("[均摊] 结论: 翻倍扩容总拷贝 O(n)（均摊 O(1)），每次加 1 是 O(n^2)（均摊 O(n)）\n");
    return 0;
}
