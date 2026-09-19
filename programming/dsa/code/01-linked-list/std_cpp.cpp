// =====================================================================
// 数据结构与算法 · 第 1 章 链表 · 标准库写法（C++ / std::list）
// 编译运行： g++ -std=c++17 -Wall -Wextra -Wpedantic -o app std_cpp.cpp && ./app
//
// 复杂度速查（时间 / 空间）
//   头插 push_front O(1) / O(1)     尾插 push_back O(1) / O(1)
//   查找 std::find O(n) / O(1)      删除 remove O(n) / O(1)
//   反转 reverse O(n) / O(1)        合并 merge O(n+m) / O(1)
//   按索引取第 k 个 O(k) / O(1)     ← 链表没有随机访问，这是它和数组最大的区别
// 说明：C++ 标准库的单链表是 std::forward_list（没有 size()、没有 push_back），
//      这里用 std::list（双向链表）演示同样的操作，接口更完整。
// =====================================================================

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

void print_all(const std::string &prefix, const std::list<int> &l) {
    std::cout << prefix << ":";
    for (int v : l) {
        std::cout << " " << v;
    }
    std::cout << "\n";
}

int main() {
    std::list<int> l;

    l.push_front(3);
    l.push_front(2);
    l.push_front(1);
    print_all("[标准库] 头插 3,2,1", l);

    l.push_back(4);
    print_all("[标准库] 尾插 4", l);

    std::cout << "[标准库] 查找 3: "
              << (std::find(l.begin(), l.end(), 3) != l.end() ? "找到" : "未找到") << "\n";
    std::cout << "[标准库] 查找 9: "
              << (std::find(l.begin(), l.end(), 9) != l.end() ? "找到" : "未找到") << "\n";

    l.remove(2);
    print_all("[标准库] 删除 2", l);

    l.reverse();
    print_all("[标准库] 反转", l);

    std::cout << "[标准库] 长度: " << l.size() << "\n";

    std::list<int> a{1, 3, 5};
    std::list<int> b{2, 4, 6};
    a.merge(b);                              // 两条有序链表合并，O(n+m)
    print_all("[标准库] 合并两条有序链表", a);

    std::list<int>::iterator p = l.begin();
    std::advance(p, 1);                      // 链表要一个一个走，O(k)
    std::cout << "[标准库] 按索引取第 2 个: " << *p << "\n";
    return 0;
}
