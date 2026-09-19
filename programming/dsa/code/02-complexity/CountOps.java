// =====================================================================
// 数据结构与算法 · 第 2 章 复杂度与算法分析 · 操作次数实测（Java）
// 编译运行： javac -encoding UTF-8 CountOps.java
//            java -Dstdout.encoding=UTF-8 CountOps
//
// 本章的代码是「测量工具」，所以四门语言用同一套手写逻辑，
// 数出来的操作次数完全一致，方便横向对照。
//
// 复杂度（时间 / 空间）
//   常数 O(1)/O(1)  二分 O(log n)/O(1)  遍历 O(n)/O(1)
//   归并排序 O(n log n)/O(n)  双重循环 O(n^2)/O(1)  递归求和 O(n)/O(n)
// =====================================================================

import java.util.Locale;

public class CountOps {

    static final int N = 1000;
    static final int HASH_SIZE = 1024;     // 2^10；故意取小，让冲突真的发生，才看得到探测次数

    static int[] data = new int[N];
    static int[] sortedArr = new int[N];
    static int[] tmp = new int[N];
    static int[] hashTable = new int[HASH_SIZE];

    static long cmpCount = 0;
    static long probeCount = 0;

    // 固定种子的伪随机数列：保证每次运行、每门语言拿到的数据完全一样
    static void lcgFill(int[] a, int n) {
        long x = 12345L;
        for (int i = 0; i < n; ++i) {
            x = (x * 1103515245L + 12345L) % 2147483648L;
            a[i] = (int) (x % 2000L);
        }
    }

    // 归并排序：顺手统计比较次数，O(n log n)，额外空间 O(n)
    static void mergeSort(int[] a, int[] buffer, int l, int r) {
        if (r - l <= 1) return;
        int mid = l + (r - l) / 2;
        mergeSort(a, buffer, l, mid);
        mergeSort(a, buffer, mid, r);
        int i = l, j = mid, k = l;
        while (i < mid && j < r) {
            ++cmpCount;
            if (a[i] <= a[j]) buffer[k++] = a[i++];
            else buffer[k++] = a[j++];
        }
        while (i < mid) buffer[k++] = a[i++];
        while (j < r) buffer[k++] = a[j++];
        for (int t = l; t < r; ++t) a[t] = buffer[t];
    }

    // 二分查找：返回比较次数，O(log n)
    static int binarySearchCompares(int[] a, int n, int target) {
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
    static void hashClear() {
        for (int i = 0; i < HASH_SIZE; ++i) hashTable[i] = -1;
    }

    static int hashInsert(int v) {
        int h = v & (HASH_SIZE - 1);
        for (;;) {
            ++probeCount;
            if (hashTable[h] == -1) { hashTable[h] = v; return 1; }
            if (hashTable[h] == v) return 0;
            h = (h + 1) & (HASH_SIZE - 1);
        }
    }

    // 递归求和：返回值一路带出递归深度，用来看「空间复杂度里的递归栈」
    static int recSumDepth(int n, int depth) {
        if (n == 0) return depth;
        return recSumDepth(n - 1, depth + 1);
    }

    public static void main(String[] args) {
        lcgFill(data, N);

        long pairs = 0;                                    // O(n^2)
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j) ++pairs;

        long sum = 0;                                      // O(n)
        for (int i = 0; i < N; ++i) sum += data[i];

        for (int i = 0; i < N; ++i) sortedArr[i] = data[i];
        cmpCount = 0;
        mergeSort(sortedArr, tmp, 0, N);                    // O(n log n)
        long sortCompares = cmpCount;

        int logn = binarySearchCompares(sortedArr, N, sortedArr[N - 1]);   // O(log n)

        long bruteCompares = 0;
        int uniqueBrute = 0;
        for (int i = 0; i < N; ++i) {
            boolean seen = false;
            for (int j = 0; j < i; ++j) {
                ++bruteCompares;
                if (data[j] == data[i]) { seen = true; break; }
            }
            if (!seen) ++uniqueBrute;
        }

        int uniqueSorted = 1;
        long adjacentCompares = 0;
        for (int i = 1; i < N; ++i) {
            ++adjacentCompares;
            if (sortedArr[i] != sortedArr[i - 1]) ++uniqueSorted;
        }

        hashClear();
        probeCount = 0;
        int uniqueHash = 0;
        for (int i = 0; i < N; ++i) uniqueHash += hashInsert(data[i]);
        long probes = probeCount;

        System.out.println("[计数] 数据规模 n = " + N);
        System.out.println("[计数] O(1)       常数操作: 1 次");
        System.out.println("[计数] O(log n)   二分查找比较: " + logn + " 次");
        System.out.println("[计数] O(n)       遍历累加: " + N + " 次, 和 = " + sum);
        System.out.println("[计数] O(n log n) 归并排序比较: " + sortCompares + " 次");
        System.out.println("[计数] O(n^2)     双重循环: " + pairs + " 次");
        System.out.println(String.format(Locale.ROOT,
                "[计数] 增长对比: O(n^2) 是 O(n log n) 的 %.1f 倍, 是 O(n) 的 %.1f 倍",
                (double) pairs / (double) sortCompares, (double) pairs / (double) N));
        System.out.println("[去重] 不重复元素: " + uniqueHash + " 个（暴力 " + uniqueBrute
                + " / 排序 " + uniqueSorted + " / 哈希 " + uniqueHash + "，三种结果一致）");
        System.out.println("[去重] 暴力双重循环: " + bruteCompares + " 次比较, 额外空间 O(1)");
        System.out.println("[去重] 排序后相邻比较: " + adjacentCompares + " 次（不含排序的 "
                + sortCompares + " 次）, 额外空间 O(n)");
        System.out.println("[去重] 哈希表: " + probes + " 次探测 = " + N + " 次插入 + "
                + (probes - N) + " 次冲突, 额外空间 O(n)");
        System.out.println("[递归] 递归求和 n=100: 栈深度 " + recSumDepth(100, 0) + " 层；迭代版: 1 层");
    }
}
