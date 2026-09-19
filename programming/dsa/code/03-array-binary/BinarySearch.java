// =====================================================================
// 数据结构与算法 · 第 3 章 数组、字符串与二分查找 · 二分查找（Java）
// 编译运行： javac -encoding UTF-8 BinarySearch.java
//            java -Dstdout.encoding=UTF-8 BinarySearch
//
// 三种写法都要会，因为边界处理不同，面试最爱在这里挑错：
//   ① 闭区间 [lo, hi]    ② 左闭右开 [lo, hi)    ③ lower_bound
//
// 复杂度（时间 / 空间）
//   二分查找 O(log n) / O(1)      二分答案 O(log(答案范围) × 判定代价) / O(1)
// =====================================================================

public class BinarySearch {

    static final int N = 1000;
    static int[] a = new int[N];
    static long cmp = 0;      // 用一个字段累计比较次数，方便在 main 里清零复用

    // ① 闭区间 [lo, hi]：hi 取 n-1
    static int bsearchClosed(int[] arr, int n, int target) {
        int lo = 0, hi = n - 1;
        while (lo <= hi) {
            ++cmp;
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] == target) return mid;
            if (arr[mid] < target) lo = mid + 1;
            else hi = mid - 1;
        }
        return -1;
    }

    // ② 左闭右开 [lo, hi)：hi 取 n
    static int bsearchHalfOpen(int[] arr, int n, int target) {
        int lo = 0, hi = n;
        while (lo < hi) {
            ++cmp;
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] == target) return mid;
            if (arr[mid] < target) lo = mid + 1;
            else hi = mid;
        }
        return -1;
    }

    // ③ lower_bound：第一个 >= target 的下标（可能返回 n）
    static int lowerBoundIdx(int[] arr, int n, int target) {
        int lo = 0, hi = n;
        while (lo < hi) {
            ++cmp;
            int mid = lo + (hi - lo) / 2;
            if (arr[mid] < target) lo = mid + 1;
            else hi = mid;
        }
        return lo;
    }

    // 二分答案：假设每段长 len，能切出多少段
    static long cutCount(int[] wood, int k, int len) {
        long seg = 0;
        for (int i = 0; i < k; ++i) seg += wood[i] / len;
        return seg;
    }

    // 二分答案：求最大的等长 L，使切出的段数 >= need
    static int maxCutLength(int[] wood, int k, long need, long[] checks) {
        int lo = 1, hi = 0;
        for (int i = 0; i < k; ++i) if (wood[i] > hi) hi = wood[i];
        int ans = 0;
        while (lo <= hi) {
            ++checks[0];
            int mid = lo + (hi - lo) / 2;
            if (cutCount(wood, k, mid) >= need) { ans = mid; lo = mid + 1; }
            else hi = mid - 1;
        }
        return ans;
    }

    public static void main(String[] args) {
        for (int i = 0; i < N; ++i) a[i] = i * 3;      // 有序：0, 3, 6, ... 2997

        cmp = 0;
        int i1 = bsearchClosed(a, N, 1800);
        long cmp1 = cmp;
        cmp = 0;
        int i2 = bsearchHalfOpen(a, N, 1800);
        long cmp2 = cmp;
        cmp = 0;
        int i3 = lowerBoundIdx(a, N, 1800);
        long cmp3 = cmp;
        cmp = 0;
        int i4 = lowerBoundIdx(a, N, 1801);
        long cmp4 = cmp;

        int[] empty = {0};
        int[] one = {5};
        int e1 = bsearchClosed(empty, 0, 5);
        int e2 = bsearchClosed(one, 1, 5);
        int e3 = bsearchClosed(one, 1, 9);
        int e4 = bsearchClosed(a, N, 0);
        int e5 = bsearchClosed(a, N, 2997);
        int e6 = bsearchClosed(a, N, 1801);

        int[] wood = {10, 24, 16};
        long[] checks = {0};
        int best = maxCutLength(wood, 3, 7, checks);

        System.out.println("[二分] 有序数组 n = " + N + "（a[i] = i × 3）");
        System.out.println("[二分] 闭区间 [lo, hi]    找 1800: 找到, 下标 " + i1 + ", 比较 " + cmp1 + " 次");
        System.out.println("[二分] 左闭右开 [lo, hi)  找 1800: 找到, 下标 " + i2 + ", 比较 " + cmp2 + " 次");
        System.out.println("[二分] lower_bound 找 1800: 下标 " + i3 + ", 比较 " + cmp3 + " 次");
        System.out.println("[二分] lower_bound 找 1801: 下标 " + i4 + ", 比较 " + cmp4 + " 次");
        System.out.println("[二分] 边界用例:");
        System.out.println("[二分]   空数组找 5: 返回 " + e1);
        System.out.println("[二分]   单元素 [5] 找 5: 返回 " + e2);
        System.out.println("[二分]   单元素 [5] 找 9: 返回 " + e3);
        System.out.println("[二分]   找第一个元素 0: 返回 " + e4);
        System.out.println("[二分]   找最后一个元素 2997: 返回 " + e5);
        System.out.println("[二分]   找不存在的 1801: 返回 " + e6);
        System.out.println("[二分答案] 切木头: 长度 [10, 24, 16]，要切成至少 7 段等长");
        System.out.println("[二分答案] 最大等长 = " + best + ", 判定 " + checks[0] + " 次");
        System.out.println("[二分答案] 验证: L=6 得 " + cutCount(wood, 3, 6)
                + " 段（够），L=7 得 " + cutCount(wood, 3, 7) + " 段（不够）");
    }
}
