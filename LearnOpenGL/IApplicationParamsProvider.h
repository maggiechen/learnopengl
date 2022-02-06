#pragma once
#include <string>
class IApplicationParamsProvider
{
public:
	virtual std::string GetAppPath() = 0;
	// a virtual destructor to prevent memory leaks should we ever need to destroy
	// subclasses of this one using this base class
	virtual ~IApplicationParamsProvider() {};
};