// =====================================================================
// 数据结构与算法 · 第 2 章 复杂度与算法分析 · 均摊分析（Java）
// 编译运行： javac -encoding UTF-8 Amortized.java
//            java -Dstdout.encoding=UTF-8 Amortized
//
// 要回答的问题：ArrayList 每次 add 都要扩容吗？
// 答案：容量按倍数增长时，n 次 add 的总拷贝是 O(n)，均摊到每次是 O(1)。
//
// 复杂度（时间 / 空间）
//   翻倍扩容：总拷贝 O(n)，均摊 O(1)，额外空间 O(1)
//   每次加 1：总拷贝 O(n^2)，均摊 O(n)，额外空间 O(1)
// =====================================================================

import java.util.Locale;

public class Amortized {

    // 容量翻倍：容量满时扩容并拷贝旧数据，返回 n 次插入的总拷贝次数
    static long simulateDoubling(int n) {
        int cap = 1, size = 0;
        long copies = 0;
        for (int i = 0; i < n; ++i) {
            if (size == cap) { copies += size; cap *= 2; }
            ++size;
        }
        return copies;
    }

    // 每次只加 1：容量满时只多扩一个位置，返回总拷贝次数
    static long simulatePlusOne(int n) {
        int cap = 1, size = 0;
        long copies = 0;
        for (int i = 0; i < n; ++i) {
            if (size == cap) { copies += size; cap += 1; }
            ++size;
        }
        return copies;
    }

    static void report(int n) {
        long doubling = simulateDoubling(n);
        long plusOne = simulatePlusOne(n);
        System.out.println("[均摊] n = " + n);
        System.out.println(String.format(Locale.ROOT,
                "[均摊]   翻倍扩容: 总拷贝 %d 次, 平均 %.2f 次/插入",
                doubling, (double) doubling / n));
        System.out.println(String.format(Locale.ROOT,
                "[均摊]   每次加 1: 总拷贝 %d 次, 平均 %.2f 次/插入",
                plusOne, (double) plusOne / n));
    }

    public static void main(String[] args) {
        report(16);
        report(1024);
        System.out.println("[均摊] 结论: 翻倍扩容总拷贝 O(n)（均摊 O(1)），每次加 1 是 O(n^2)（均摊 O(n)）");
    }
}
