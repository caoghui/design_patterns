#include <iostream>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <future>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <cmath>
#include <numeric>
#include "Log.hpp"

using namespace std;
using namespace std::chrono;

void f()
{
	LOG_D("function f running...");
}

struct F
{
	void operator()()
	{
		LOG_D("F::operator() running...");
	}
};

void user();

void passed();

////////////////////////////////////////////////
class Message
{
};

queue<Message> mqueue;       //消息的队列
condition_variable mcond;    //条件变量
mutex mmutex;                //互斥量

void consumer()
{
	while (true)
	{
		unique_lock<mutex> lock{mmutex};
		//
		auto msg = mqueue.front();
		mqueue.pop();
		lock.unlock();
	}
}

void producer()
{
	while (true)
	{
		Message msg;
		unique_lock<mutex> lock{mmutex};
		mqueue.push(msg);
		mcond.notify_one();
	}
}

////////////////////////////////////////////////
double accum(double* beg, double* end, double init)
{
	return accumulate(beg, end, init);
}

double comp2(vector<double>& v)
{
	using Task_type = double(double*, double*, double);  //任务的类型

	packaged_task<Task_type> pt0{ accum };  //打包任务
	packaged_task<Task_type> pt1{ accum };

	future<double> f0{ pt0.get_future() };
	future<double> f1{ pt1.get_future() };

	double* first = &v[0];

	thread t1{ move(pt0), first, first + v.size() / 2, 0 };
	thread t2{ move(pt1), first + v.size() / 2, first + v.size(), 0 };

	return f0.get() + f1.get();
}

////////////////////////////////////////////////
double comp4(vector<double>& v)
{
	if (v.size() < 10)
		return accumulate(v.begin(), v.end(), 0);

	auto v0 = &v[0];
	auto sz = v.size();

	auto f0 = async(accum, v0, v0 + sz / 4, 0.0);
	auto f1 = async(accum, v0 + sz / 4, v0 + sz / 2, 0.0);
	auto f2 = async(accum, v0 + sz / 2, v0 + sz * 3 / 4, 0.0);
	auto f3 = async(accum, v0 + sz * 3 / 4, v0 + sz, 0.0);
	return f0.get() + f1.get() + f2.get() + f3.get();
}

void test_future()
{
	vector<double> v;
	for (int i = 0; i < 10; ++i)
	{
		v.push_back(i);
	}
	cout << "size = " << v.size() << endl;
	cout << comp2(v) << endl;
	//cout << comp4(v) << endl;
}

////////////////////////////////////////////////

int main(int argc, char**argv)
{
	LOG_D("smart_ptr ....");
	//user();
	//passed();
	test_future();
	return 0;
}


void user()
{
	thread t1{ f };
	thread t2{ F() };

	t1.join();
	t2.join();
}

void passed()
{
	auto t0 = high_resolution_clock::now();
	this_thread::sleep_for(milliseconds{20});
	auto t1 = high_resolution_clock::now();
	cout << duration_cast<nanoseconds>(t1 - t0).count() << " nanoseconds passed" << endl;
}