

#include <algorithm>
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> src = {1, 2, 3};
    std::vector<int> dest(src.size());

    // 等效于 std::copy(src.rbegin(), src.rend(), dest.begin());
    std::reverse_copy(src.begin(), src.end(), dest.begin());
    std::cout << dest[0] << " " << dest[1] << " " << dest[2] << std::endl;
}