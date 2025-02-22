
#include"mouse.h"

HWND gethandle(const std::wstring& windowTitle) {
    HWND hwnd = FindWindow(NULL, windowTitle.c_str()); // 通过窗口标题查找窗口
    if (!hwnd) {
        std::cerr << "窗口未找到!" << std::endl;
        exit(1);
    }
    return hwnd;
}
RECT getaxis(HWND hwnd) {
    // 获取窗口坐标
    RECT axis;
    GetWindowRect(hwnd, &axis);
    //GetClientRect(hwnd, &axis);
    std::cout << "L" << axis.left << "T" << axis.top << "R" << axis.right << "B" << axis.bottom << std::endl;
    return axis;
}

// 查找窗口并点击指定区域
void mousesmoothmoveto(int x, int y) {
    //获取当前坐标
    POINT cursovrPos;
    GetCursorPos(&cursovrPos);

    int step = max(abs(x - cursovrPos.x), abs(y - cursovrPos.y)) / 10;

    for (int i = 0;i < step;++i) {
        SetCursorPos(
            (int)(cursovrPos.x +
                (x - cursovrPos.x) / (double)step * i + rand() % 30)
            , (int)(cursovrPos.y +
                (y - cursovrPos.y) / (double)step * i + rand() % 30)

        );
        Sleep(5 + rand() % 10);
    }
    SetCursorPos(x, y);
}

void ClickWindowRegion(HWND hwnd, double x, double y) {
    //获取屏幕坐标
    RECT rect = getaxis(hwnd);
    GetWindowRect(hwnd, &rect);
    //std::cout << rect.left << "," << rect.right << "." << rect.top << "," << rect.bottom;
    //计算屏幕宽度
    int h = rect.bottom - rect.top;
    int w = rect.right - rect.left;
    // 计算屏幕坐标
    int screenX = rect.left + x * w;
    int screenY = rect.top + y * h;

    // 设置鼠标位置
    mousesmoothmoveto(screenX, screenY);

    // 模拟鼠标点击
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // 按下左键

    Sleep(100 + rand() % 25);;  // 短暂延迟
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // 释放左键

}
void ClickAndMoveWindowRegion(HWND hwnd, double x1, double y1, double x2, double y2) {
    //获取屏幕坐标
    RECT rect = getaxis(hwnd);
    GetWindowRect(hwnd, &rect);
    //std::cout << rect.left << "," << rect.right << "." << rect.top << "," << rect.bottom;
    //计算屏幕宽度
    int h = rect.bottom - rect.top;
    int w = rect.right - rect.left;
    // 计算屏幕坐标
    int screenX1 = rect.left + x1 * w;
    int screenY1 = rect.top + y1 * h;
    int screenX2 = rect.left + x2 * w;
    int screenY2 = rect.top + y2 * h;
    // 设置鼠标位置
    mousesmoothmoveto(screenX1, screenY1);
    // 模拟鼠标点击
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // 按下左键
    Sleep(100 + rand() % 25);;  // 短暂延迟
    mousesmoothmoveto(screenX2, screenY2);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // 释放左键

}
