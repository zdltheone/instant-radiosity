#include "app.h"
#include <fstream>
#include <iostream>
using namespace std;

/*
* Test code (will be deleted later) by Zhihang
*/
#include "testUnit.h"

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	simpleTest();

	App app;
	app.RunApp();
}