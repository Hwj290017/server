

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // 将前4个元素（1,2,3,4）从后往前拷贝到位置 [2, 3, 4, 5]
    std::copy_backward(vec.begin(), vec.begin() + 4, vec.end());

    // 输出结果：1, 2, 1, 2, 3, 4 （假设 vec 初始为 {1,2,3,4,5}，但此处需容器扩展）
    // 注意：此处示例需目标容器足够大，实际代码可能需要 resize()
    for (int num : vec)
    {
        std::cout << num << " ";
    }
}