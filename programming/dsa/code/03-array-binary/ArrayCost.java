// =====================================================================
// 数据结构与算法 · 第 3 章 数组、字符串与二分查找 · 数组代价实测（Java）
// 编译运行： javac -encoding UTF-8 ArrayCost.java
//            java -Dstdout.encoding=UTF-8 ArrayCost
//
// 复杂度（时间 / 空间）
//   随机访问 O(1) / O(1)            尾部追加（容量够）O(1) / O(1)
//   头部插入 / 删除 O(n) / O(1)     前缀和：预处理 O(n)、查询 O(1)
//   差分：区间加 O(1)、还原 O(n)
// =====================================================================

public class ArrayCost {

    static final int N = 1000;
    static final int L = 200;
    static final int R = 800;

    static int[] data = new int[N];
    static int[] buf = new int[N + 2];      // 留两格余量：头部插入 1 个 + 尾部插入 1 个
    static long[] prefix = new long[N + 1]; // prefix[i] = data[0..i) 的和
    static long[] diff = new long[N + 1];

    // 固定种子的伪随机数列：与第 2 章同一个种子，方便对照
    static void lcgFill(int[] a, int n) {
        long x = 12345L;
        for (int i = 0; i < n; ++i) {
            x = (x * 1103515245L + 12345L) % 2147483648L;
            a[i] = (int) (x % 2000L);
        }
    }

    public static void main(String[] args) {
        lcgFill(data, N);

        int randomAccess = data[N / 2];                 // 随机访问：1 次取值

        for (int i = 0; i < N; ++i) buf[i] = data[i];
        int size = N;

        int tailAppended = data[N - 1];                 // 尾部追加（容量够）

        long headInsertMoves = 0;                       // 头部插入：整体右移
        for (int i = size; i > 0; --i) { buf[i] = buf[i - 1]; ++headInsertMoves; }
        buf[0] = 42;
        ++size;

        long tailInsertMoves = 0;                       // 尾部插入触发扩容：整体拷贝
        for (int i = 0; i < size; ++i) ++tailInsertMoves;
        buf[size] = 43;
        ++size;

        long headDeleteMoves = 0;                       // 删除头部：整体左移
        for (int i = 0; i + 1 < size; ++i) { buf[i] = buf[i + 1]; ++headDeleteMoves; }
        --size;

        int tailDeletedMoves = 0;                       // 删除尾部
        --size;

        long prefixAdds = 0;
        prefix[0] = 0;
        for (int i = 0; i < N; ++i) { prefix[i + 1] = prefix[i] + data[i]; ++prefixAdds; }

        long rangeSumFast = prefix[R] - prefix[L];
        long rangeSumBrute = 0;
        long bruteAdds = 0;
        for (int i = L; i < R; ++i) { rangeSumBrute += data[i]; ++bruteAdds; }

        for (int i = 0; i <= N; ++i) diff[i] = 0;
        diff[L] += 5;
        diff[R] -= 5;
        long diffChanges = 2;

        long restoreAdds = 0;
        long cur = 0;
        long restoredRangeSum = 0;
        for (int i = 0; i < N; ++i) {
            cur += diff[i];
            ++restoreAdds;
            if (i >= L && i < R) restoredRangeSum += data[i] + cur;
        }

        System.out.println("[数组] 数据规模 n = " + N);
        System.out.println("[数组] 随机访问第 " + (N / 2) + " 个: " + randomAccess + "（1 次取值）, O(1)");
        System.out.println("[数组] 尾部追加（容量够）: 移动 0 个元素（读到 " + tailAppended + "）, O(1)");
        System.out.println("[数组] 头部插入: 移动 " + headInsertMoves + " 个元素, O(n)");
        System.out.println("[数组] 尾部插入（触发扩容）: 拷贝 " + tailInsertMoves + " 个元素, O(n)");
        System.out.println("[数组] 删除头部: 移动 " + headDeleteMoves + " 个元素, O(n)");
        System.out.println("[数组] 删除尾部: 移动 " + tailDeletedMoves + " 个元素, O(1)");
        System.out.println("[前缀和] 预处理 " + N + " 个元素: " + prefixAdds + " 次加法, O(n)");
        System.out.println("[前缀和] 区间 [" + L + ", " + R + ") 求和: 1 次减法 = " + rangeSumFast + ", O(1)");
        System.out.println("[前缀和] 同区间暴力求和: " + bruteAdds + " 次加法 = " + rangeSumBrute + ", O(n)");
        System.out.println("[前缀和] 两者结果一致: " + (rangeSumFast == rangeSumBrute ? "是" : "否"));
        System.out.println("[差分] 区间 [" + L + ", " + R + ") 批量加 5: " + diffChanges + " 次修改, O(1)");
        System.out.println("[差分] 从差分数组还原: " + restoreAdds + " 次加法, O(n)");
        System.out.println("[差分] 还原后该区间和 = " + restoredRangeSum
                + "（原区间和 " + rangeSumBrute + " + " + (R - L) + "×5）");
    }
}
