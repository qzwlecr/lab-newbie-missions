#UniqueLab Mission 2

##介绍：

1. array
    + init ptr\_at empty size **O(1)**
2. vector
    + init ptr\_at empty size capacity reserve resize shrink\_to\_fit push\_back pop\_back **O(1)**
    + clear insert erase **O(n)**
3. list 
    + init ptr\_at empty size insert erase push\_back pop\_back push\_front pop\_front begin end Iterator\_increase Iterator\_decrease  Iterator\_equal Iterator\_notequal get\_data\_ptr **O(1)**
    + clear **O(n)**
4. set 
    + empty size init begin end Iterator\_equal Iterator\_notequal get\_data\_ptr **O(1)**
    + clear **O(n)**
    + insert erase find lower\_bound upper\_bound **O(log n)**
    + Iterator\_increase Iterator\_decrease **O(1),but O(log n) in worst case**
5. stack(by vector)
    + init empty size push pop top **O(1)**
    + clear **O(n)**
6. queue(by list)
    + init empty size push pop top **O(1)**
    + clear **O(n)**
7. priority\_queue(by vector)
    + init empty size top **O(1)**
    + push pop **O(log n)**
    + clear **O(n)**
8. unordered\_map
    + empty size init **O(1)**
    + clear **O(n)**
    + erase insert **O(xuanxue)**

##文件说明:

    *.h *的操作接口头文件
    *.c *的具体实现

##简介：

本次任务要求实现用C语言实现C++ STL库的部分功能，为下一期任务造好轮子。
本期任务参考资料为 http://www.cplusplus.com/reference/stl

##要求：

1. 实现以下容器
    1. vector, stack, queue, \(array,deque可选\)
    2. list, \(forward\_list 可选\)
    3. priority\_queue
    4. set, unordered\_map （map, unordered\_set可选)、
2. 时间复杂度有严格要求，也是检查重点。具体数值请自行Google.
3. stack和queue内置容器可以自己选择。
4. 可以选择C语言或者C++实现，C++要求实现上述容器除allocator和C++11及以上特性的其他所有功能。C语言实现要求实现头文件中列出的所有功能。
5. C实现的头文件已经给出
    1. 地址https://github.com/FluorineDog/2016-uniquestudio-assignments2
6. 由于C语言天然的限制，本来需要返回引用的地方只能返回指针，相当不优雅，请在使用ADT的时候多加注意。


