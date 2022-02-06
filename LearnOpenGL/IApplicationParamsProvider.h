#pragma once
#include <string>
class IApplicationParamsProvider
{
public:
	virtual std::string GetAppPath() = 0;
	virtual ~IApplicationParamsProvider() {};
};