// =====================================================================
// 数据结构与算法 · 第 1 章 链表 · 手写实现（Java）
// 编译运行： javac -encoding UTF-8 LinkedListRaw.java
//            java -Dstdout.encoding=UTF-8 LinkedListRaw
//
// 复杂度速查（时间 / 空间）
//   头插 O(1) / O(1)          尾插 O(n) / O(1)
//   查找 O(n) / O(1)          删除 O(n) / O(1)    （单链表要重新找前驱）
//   反转 O(n) / O(1)          找中点 O(n) / O(1)
//   判环 O(n) / O(1)          合并两条有序链表 O(n+m) / O(1)
// 说明：Java 手写链表不需要 free —— 对象没有引用后由 GC 回收，
//      这一点和 C / C++ 的手写实现正好形成对照。
// =====================================================================

public class LinkedListRaw {

    static class Node {
        int value;
        Node next;

        Node(int value, Node next) {
            this.value = value;
            this.next = next;
        }
    }

    // 头插：新结点接到表头，O(1)
    static Node pushFront(Node head, int value) {
        return new Node(value, head);
    }

    // 尾插：先走到最后一个结点，O(n)
    static Node pushBack(Node head, int value) {
        Node node = new Node(value, null);
        if (head == null) return node;
        Node cur = head;
        while (cur.next != null) cur = cur.next;
        cur.next = node;
        return head;
    }

    // 遍历打印：以 "prefix: 1 2 3" 的形式输出
    static void printAll(String prefix, Node head) {
        StringBuilder sb = new StringBuilder(prefix).append(":");
        for (Node cur = head; cur != null; cur = cur.next) {
            sb.append(" ").append(cur.value);
        }
        System.out.println(sb);
    }

    // 查找：返回第几个（从 1 开始），找不到返回 0，O(n)
    static int indexOf(Node head, int value) {
        int i = 1;
        for (Node cur = head; cur != null; cur = cur.next, ++i) {
            if (cur.value == value) return i;
        }
        return 0;
    }

    // 删除第一个匹配的结点，O(n)
    static Node erase(Node head, int value) {
        Node prev = null;
        Node cur = head;
        while (cur != null) {
            if (cur.value == value) {
                if (prev == null) head = cur.next;
                else prev.next = cur.next;
                return head;
            }
            prev = cur;
            cur = cur.next;
        }
        return head;
    }

    // 反转：三指针迭代，O(n) 时间、O(1) 空间
    static Node reverse(Node head) {
        Node prev = null;
        Node cur = head;
        while (cur != null) {
            Node next = cur.next;
            cur.next = prev;
            prev = cur;
            cur = next;
        }
        return prev;
    }

    // 找中点：快慢指针，O(n)
    static Node middle(Node head) {
        Node slow = head;
        Node fast = head;
        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
        }
        return slow;
    }

    // 判环：Floyd 快慢指针，O(n)
    static boolean hasCycle(Node head) {
        Node slow = head;
        Node fast = head;
        while (fast != null && fast.next != null) {
            slow = slow.next;
            fast = fast.next.next;
            if (slow == fast) return true;
        }
        return false;
    }

    // 长度：O(n)
    static int length(Node head) {
        int n = 0;
        for (Node cur = head; cur != null; cur = cur.next) ++n;
        return n;
    }

    // 合并两条有序链表：双指针穿针引线，O(n+m)
    static Node mergeSorted(Node a, Node b) {
        Node dummy = new Node(0, null);
        Node tail = dummy;
        while (a != null && b != null) {
            if (a.value <= b.value) {
                tail.next = a;
                a = a.next;
            } else {
                tail.next = b;
                b = b.next;
            }
            tail = tail.next;
        }
        tail.next = (a != null) ? a : b;
        return dummy.next;
    }

    public static void main(String[] args) {
        Node list = null;

        list = pushFront(list, 3);
        list = pushFront(list, 2);
        list = pushFront(list, 1);
        printAll("[手写] 头插 3,2,1", list);

        list = pushBack(list, 4);
        printAll("[手写] 尾插 4", list);

        int pos = indexOf(list, 3);
        System.out.println("[手写] 查找 3: 找到, 第 " + pos + " 个");
        System.out.println("[手写] 查找 9: " + (indexOf(list, 9) == 0 ? "未找到" : "找到"));

        list = erase(list, 2);
        printAll("[手写] 删除 2", list);

        list = reverse(list);
        printAll("[手写] 反转", list);

        System.out.println("[手写] 找中点: " + middle(list).value);

        System.out.println("[手写] 判环(无环): " + (hasCycle(list) ? "有环" : "无环"));
        Node last = list;
        while (last.next != null) last = last.next;
        last.next = list;                       // 人为造一个环
        System.out.println("[手写] 判环(有环): " + (hasCycle(list) ? "有环" : "无环"));
        last.next = null;                       // 拆掉环

        System.out.println("[手写] 长度: " + length(list));

        Node a = null;
        Node b = null;
        a = pushBack(a, 1); a = pushBack(a, 3); a = pushBack(a, 5);
        b = pushBack(b, 2); b = pushBack(b, 4); b = pushBack(b, 6);
        Node merged = mergeSorted(a, b);
        printAll("[手写] 合并两条有序链表", merged);
    }
}
