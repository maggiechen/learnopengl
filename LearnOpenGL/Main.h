#pragma once

#include "IApplicationParamsProvider.h"
#include "Texturing.h"
#include "TrianglesAndShaders.h"
#include "Transforms.h"
#include "CoordinateSystems.h"
class ApplicationRunner: IApplicationParamsProvider
{
public:
	ApplicationRunner(std::string appPath);
	int RunMain();

	std::string GetAppPath();
private:
	std::string m_appPath;
};
