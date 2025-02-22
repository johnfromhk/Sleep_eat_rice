#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <sstream>
#include <time.h>
using std::vector;
using std::string;
using std::cout;
using std::endl;

HWND gethandle(const std::wstring& windowTitle);
RECT getaxis(HWND hwnd);
void ClickAndMoveWindowRegion(HWND hwnd, double x1, double y1, double x2, double y2);
void ClickWindowRegion(HWND hwnd, double x, double y);
