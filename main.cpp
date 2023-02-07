#include <iostream>
#include <concepts>
#include <ranges>


template <unsigned N = sizeof(unsigned long) * 8>
struct StaticString
{
    char data[N];
    constexpr StaticString(): data() {}
    constexpr StaticString(std::ranges::input_range auto&& str) : data()
    {
        auto i = 0;
        for (const auto c : str)
            data[i++] = c;
    }
    constexpr StaticString(unsigned long n) : data()
    {
        unsigned long div = 10;
        const int size = sizeof(decltype(n)) * 8;
        int i = 0;
        for (; i < size; ++i)
        {
            const unsigned long r = n % div;
            data[N - 1 - i] = static_cast<char>((r*10/div)+char('0'));
            n -= r;
            div *= 10;
            if (n == 0)
                break;
        }
        auto j = 0;
        for (; i > - 1; --i)
            data[j++] = data[N - 1 - i];
    }
    constexpr auto& operator[](std::size_t idx) {
        return data[idx];
    }
    constexpr auto operator[](std::size_t idx) const {
        return data[idx];
    }
};


template <int N>
constexpr auto to_const_char(unsigned long n)
{
    unsigned long div = 10;
    auto str = StaticString<20>{};
    for (auto i = N - 1; i > -1; --i)
    {
        const unsigned long r = n % div;
        str[i] = static_cast<char>((r*10/div)+char('0'));
        n -= r;
        div *= 10;
    }
    return str;
}


template <int NDigits, unsigned N>
constexpr bool isPalindrome(const StaticString<N>& str)
{
    for (auto i = 0; i < NDigits/2; ++i)
        if (str[i] != str[NDigits - 1 - i])
            return false;
    return true;
}


template <unsigned long N, unsigned long M>
constexpr auto pow()
{
    unsigned long result = 1;
    for (auto i = 0; i < M; ++i)
        result *= N;
    return result;
}


template <int NDigits2>
constexpr unsigned long largestPalindrome()
{
    constexpr unsigned long min = 1 * ::pow<10, NDigits2 - 1>() - 1;
    constexpr auto max = (min + 1) * 10 - 1;
    unsigned long result = 0;
    auto curr_min = min;
    for (auto i = max; i > curr_min; --i)
        for (auto j = i; j > curr_min; --j)
        {
            const unsigned long mult = i * j;
            if (isPalindrome<NDigits2*2>(StaticString{mult}))
            {
                result = std::max(result, mult);
                curr_min = j + 1;
            }
        }
    return result;
}


int main() {

    constexpr auto str_11011 = to_const_char<5>(12345);
    static_assert(str_11011.data[0] == '1');
    static_assert(isPalindrome<7>(StaticString<20>{"1234321"}));
    static_assert(largestPalindrome<2>() == 9009);
    static_assert(largestPalindrome<3>() == 906609);
    static_assert(largestPalindrome<4>() == 99000099);
    std::cout << largestPalindrome<5>() << '\n';
    std::cout << largestPalindrome<8>() << '\n';

    return 0;
}
