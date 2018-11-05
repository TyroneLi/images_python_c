#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include "Python.h"

#pragma comment(lib, "python36_d.lib")

struct cellPre {
	std::string labelClass;
	float score;
	int x;
	int y;
	int h;
	int w;
};

class PythonInterface {
public:
	PythonInterface();
	~PythonInterface();
	void pythonInitializer();
	void callPythonFunc(const char*imagePath, struct cellPre cells[], 
		const char*moduleName = "client", const char*pythonFuncName = "main");
};