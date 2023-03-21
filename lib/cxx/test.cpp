#include <iostream>
#include <tuple>

using namespace std;

std::tuple<std::string, int> make_a_tuple()
{
    return {};
}

int main()
{
    auto a = make_a_tuple();
    cout << "a: " << get<0>(a) << ", " << get<1>(a) << endl;
    return 0;
}
