/**
 * c++11 lambda and std::for_each
 */
#include <vector>
#include <algorithm>
using namespace std;
int main(int argc, char *argv[])
{
    auto x = vector<int>{1,2,3};
    auto print_vecotr = [&x]() {
        for_each(x.begin(), x.end(), [](int i) { printf("%d\n", i);});
    };
    print_vecotr();
    for_each(x.begin(), x.end(), [] (int i) {i = 2*i;});
    print_vecotr();
    for_each(x.begin(), x.end(), [] (int& i) {i = 2*i;});
    print_vecotr();
    return 0;
}
