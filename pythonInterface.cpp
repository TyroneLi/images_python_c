#include "stdafx.h"
#include "pythonInterface.H"

PythonInterface::PythonInterface() {}
PythonInterface::~PythonInterface() {}

void PythonInterface::pythonInitializer()
{
	Py_Initialize();
	if (!Py_IsInitialized()) {
		throw "can not initialize python environment!";
		exit(1);
	}
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
}

void PythonInterface::callPythonFunc(const char*imagePath, struct cellPre cells[], const char*moduleName, const char*pythonFuncName)
{
	pythonInitializer();

	PyObject* pMod = NULL;
	PyObject* pFunc = NULL;
	PyObject* pParm = NULL;
	PyObject* pRetVal = NULL;
	int iRetVal = -99;

	// 这个是被调用的py文件模块名字
	const char* modulName = moduleName;
	pMod = PyImport_ImportModule(modulName);
	if (!pMod)
	{
		throw "error!!";
		exit(1);
	}
	// 这是此py文件模块中被调用的函数名字
	const char* funcName = pythonFuncName;
	pFunc = PyObject_GetAttrString(pMod, funcName);
	if (!pFunc)
	{
		throw "error!";
		exit(1);
	}
	pParm = PyTuple_New(1);
	// 传入的参数，是图片的路径
	PyTuple_SetItem(pParm, 0, Py_BuildValue("s", imagePath));
	// 这里开始执行py脚本
	pRetVal = PyEval_CallObject(pFunc, pParm);

	int check = 0;
	PyObject*errorMessage = PyList_GetItem(pRetVal, 0);
	PyArg_Parse(errorMessage, "i", &check);
	
	if (check == 1)
	{
		// 用一维数组接收labels
		PyObject*item_label = PyList_GetItem(pRetVal, 1);
		int subListLen = PyList_Size(item_label);
		int nValue;
		std::string labels_to_names[12] = { "trbc","krbc","srbc","frbc","wbc","sp","scsp","coc","nz","gx","xs","gj" };

		for (int i = 0; i < subListLen; i++)
		{
			PyObject*subitem_label = PyList_GetItem(item_label, i);
			PyArg_Parse(subitem_label, "i", &nValue);

			cells[i].labelClass = labels_to_names[nValue];
		}

		// 用一维数组接收scores
		PyObject*item_score = PyList_GetItem(pRetVal, 2);
		subListLen = PyList_Size(item_score);
		float score;
		// float*ptr_score = new float[subListLen];
		for (int i = 0; i < subListLen; i++)
		{
			PyObject*subitem_score = PyList_GetItem(item_score, i);
			PyArg_Parse(subitem_score, "f", &score);

			cells[i].score = score;
		}

		int x, y, h, w;
		PyObject*item_boxes = PyList_GetItem(pRetVal, 3);
		subListLen = PyList_Size(item_boxes);

		PyObject*subitem_boxes = NULL;
		PyObject*subitem_box = NULL;
		for (int i = 0; i < subListLen; i++)
		{
			subitem_boxes = PyList_GetItem(item_boxes, i);
			subitem_box = PyList_GetItem(subitem_boxes, 0);
			PyArg_Parse(subitem_box, "i", &x);
			cells[i].x = x;
			subitem_box = PyList_GetItem(subitem_boxes, 1);
			PyArg_Parse(subitem_box, "i", &y);
			cells[i].y = y;
			subitem_box = PyList_GetItem(subitem_boxes, 2);
			PyArg_Parse(subitem_box, "i", &h);
			cells[i].h = h;
			subitem_box = PyList_GetItem(subitem_boxes, 3);
			PyArg_Parse(subitem_box, "i", &w);
			cells[i].w = w;
		}
	}
	else if (check == -1)
	{
		std::cout << "Please check your image path!" << std::endl;
	}
	else
	{
		std::cout << "Please check your operations!" << std::endl;
	}
}