#pragma once

#include <functional>

class CScopeGuard
{
private:
	std::function<void()> on_exit_scope_;
	bool dismissed_;	
public:
	explicit CScopeGuard(std::function<void()> on_exit_scope) : on_exit_scope_(on_exit_scope), dismissed_(false)
	{
	}
	~CScopeGuard()
	{
		if (!dismissed_)
		{
			on_exit_scope_();
		}
	}

	void dismiss() { dismissed_ = true; }
};

#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

#define ON_SCOPE_EXIT(callback) CScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

