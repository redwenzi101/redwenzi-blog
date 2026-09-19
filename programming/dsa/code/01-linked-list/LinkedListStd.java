// =====================================================================
// 数据结构与算法 · 第 1 章 链表 · 标准库写法（Java / java.util.LinkedList）
// 编译运行： javac -encoding UTF-8 LinkedListStd.java
//            java -Dstdout.encoding=UTF-8 LinkedListStd
//
// 复杂度速查（时间 / 空间）
//   头插 addFirst O(1) / O(1)       尾插 addLast O(1) / O(1)
//   查找 contains O(n) / O(1)       删除 remove(Object) O(n) / O(1)
//   反转 Collections.reverse O(n) / O(1)
//   按索引取第 k 个 get(k) O(k) / O(1)   ← 链表没有随机访问
//   合并两条有序链表（双指针遍历） O(n+m) / O(n+m)
// 陷阱：l.remove(2) 删的是下标 2 的元素（List 接口的 remove(int)），
//      要按值删除必须写成 l.remove(Integer.valueOf(2))。
// =====================================================================

import java.util.Collections;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;

public class LinkedListStd {

    static void printAll(String prefix, LinkedList<Integer> l) {
        StringBuilder sb = new StringBuilder(prefix).append(":");
        for (int v : l) {
            sb.append(" ").append(v);
        }
        System.out.println(sb);
    }

    static LinkedList<Integer> mergeSorted(LinkedList<Integer> a, LinkedList<Integer> b) {
        LinkedList<Integer> out = new LinkedList<>();
        Iterator<Integer> ia = a.iterator();
        Iterator<Integer> ib = b.iterator();
        Integer x = ia.hasNext() ? ia.next() : null;
        Integer y = ib.hasNext() ? ib.next() : null;
        while (x != null && y != null) {
            if (x <= y) {
                out.addLast(x);
                x = ia.hasNext() ? ia.next() : null;
            } else {
                out.addLast(y);
                y = ib.hasNext() ? ib.next() : null;
            }
        }
        while (x != null) { out.addLast(x); x = ia.hasNext() ? ia.next() : null; }
        while (y != null) { out.addLast(y); y = ib.hasNext() ? ib.next() : null; }
        return out;
    }

    public static void main(String[] args) {
        LinkedList<Integer> l = new LinkedList<>();

        l.addFirst(3);
        l.addFirst(2);
        l.addFirst(1);
        printAll("[标准库] 头插 3,2,1", l);

        l.addLast(4);
        printAll("[标准库] 尾插 4", l);

        System.out.println("[标准库] 查找 3: " + (l.contains(3) ? "找到" : "未找到"));
        System.out.println("[标准库] 查找 9: " + (l.contains(9) ? "找到" : "未找到"));

        l.remove(Integer.valueOf(2));
        printAll("[标准库] 删除 2", l);

        Collections.reverse(l);
        printAll("[标准库] 反转", l);

        System.out.println("[标准库] 长度: " + l.size());

        LinkedList<Integer> a = new LinkedList<>(List.of(1, 3, 5));
        LinkedList<Integer> b = new LinkedList<>(List.of(2, 4, 6));
        printAll("[标准库] 合并两条有序链表", mergeSorted(a, b));

        System.out.println("[标准库] 按索引取第 2 个: " + l.get(1));
    }
}
