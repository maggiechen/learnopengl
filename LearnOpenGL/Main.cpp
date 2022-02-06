#include "Main.h"

int main(int argc, char* argv[])
{
	// This isn't platform safe, but for some reason this VS project doesn't have proper filesystem support despite
	// being c++17, and the latest boost libs are not compiling properly for me. So I'm just going to manually
	// handle paths. This is going to be used to get the vertex and fragment shader files, which I've configured
	// to be copied into the output directory in the LearnOpenGL.vsxproj file.
	std::string fullPathToExe = argv[0];
	std::string appPath = fullPathToExe.substr(0, fullPathToExe.find_last_of("\\"));
	ApplicationRunner appRunner(appPath);
	return appRunner.RunMain();
}

ApplicationRunner::ApplicationRunner(std::string appPath)
{
	m_appPath = appPath;
}

int ApplicationRunner::RunMain()
{
	TrianglesAndShaders g(this);
	int ret = g.mainImplTriangleWithVBO();

	return ret;
}

std::string ApplicationRunner::GetAppPath()
{
	return m_appPath;
}
