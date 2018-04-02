#include <chrono>
#include <iterator>
#include <limits>
#include <iostream>
#include "Log.hpp"

using namespace std;
using namespace std::chrono;

template<typename C>
using Iterator_type = typename C::iterator;

template<typename Iter>
using Iterator_category = typename std::iterator_traits<Iter>::iterator_category;

template<typename Ran>
void sort_helper(Ran beg, Ran end, random_access_iterator_tag)
{
	sort(beg, end);
}

template<typename For>
void sort_helper(For beg, For end, forward_iterator_tag)
{
	vector<decltype(*beg)> v{beg, end};
	sort(v.begin(), v.end());
	copy(v.begin(), v.end(), beg);
}

template<typename C>
void sort(C& c)
{
	using Iter = Iterator_type<C>;
	sort_helper(c.begin(), c.end(), Iterator_category<Iter>{});
}


template<typename T>
constexpr bool Is_arithmetic()
{
	return std::is_arithmetic<T>::value;
}

void test_limits();

int main(int argc, char**argv)
{
	LOG_D("hello world");

	//test_limits();
	LOG_D("int is arithmetic = " << is_arithmetic<int>());
	LOG_D("str is arithmetic = " << is_arithmetic<string>());

	return 0;
}


void test_limits()
{
	constexpr float min = numeric_limits<float>::min();

	LOG_D("float::min = " << min);
	constexpr int szi = sizeof(int);
	LOG_D("int::sizeof = " << szi);
}