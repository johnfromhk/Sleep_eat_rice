#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
bool CaptureWindowScreenshot(const std::wstring& windowClass,const std::wstring& windowTitle, const std::wstring& savePath) {
    // 获取目标窗口句柄

    HWND hwndm = FindWindowW(windowClass.c_str(), nullptr);
    if (!hwndm) {
        std::wcerr << L"未找到窗口: " << windowTitle << std::endl;
        return false;
    }
    else {
        std::cout << L"窗口handle: " << hwndm << std::endl;
    }
    HWND hwnd = FindWindowEx(hwndm,nullptr,L"Chrome_RenderWidgetHostHWND", nullptr);
    if (!hwnd) {
        std::wcerr << L"未找到窗口: " << windowTitle << std::endl;
        return false;
    }
    else {
        std::cout << L"窗口handle: " << hwnd << std::endl;
    }
    // 获取窗口设备上下文 (DC)
    HDC hdcWindow = GetDC(hwnd);
    HDC hdcMemDC = CreateCompatibleDC(hdcWindow);

    RECT rc;
    GetClientRect(hwnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    // 创建兼容位图
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcWindow, width, height);
    SelectObject(hdcMemDC, hbmScreen);

    // 拷贝窗口内容到内存 DC
    if (!BitBlt(hdcMemDC, 0, 0, width, height, hdcWindow, 0, 0, SRCCOPY)) {
        std::cerr << "BitBlt 失败" << std::endl;
        DeleteObject(hbmScreen);
        DeleteDC(hdcMemDC);
        ReleaseDC(hwnd, hdcWindow);
        return false;
    }

    // 保存到文件
    BITMAP bmp;
    GetObject(hbmScreen, sizeof(BITMAP), &bmp);
    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = -bmp.bmHeight;  // 负数表示正确的方向
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);

    GetDIBits(hdcWindow, hbmScreen, 0, (UINT)bmp.bmHeight, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    HANDLE hFile = CreateFileW(savePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42;  // 'BM'

    DWORD dwWritten;
    WriteFile(hFile, &bmfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, nullptr);
    WriteFile(hFile, &bi, sizeof(BITMAPINFOHEADER), &dwWritten, nullptr);
    WriteFile(hFile, lpbitmap, dwBmpSize, &dwWritten, nullptr);

    // 释放资源
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    CloseHandle(hFile);
    DeleteObject(hbmScreen);
    DeleteDC(hdcMemDC);
    ReleaseDC(hwnd, hdcWindow);

    std::wcout << L"截图已保存至: " << savePath << std::endl;
    return true;
}

bool CaptureScreen(const char* filename) {
    // 获取屏幕的宽度和高度
    int screenX = GetSystemMetrics(SM_CXSCREEN);
    int screenY = GetSystemMetrics(SM_CYSCREEN);

    // 创建屏幕的 DC 和兼容 DC
    HDC hScreenDC = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreenDC);

    // 创建兼容的位图
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, screenX, screenY);
    SelectObject(hMemDC, hBitmap);

    // 复制屏幕内容到兼容 DC
    BitBlt(hMemDC, 0, 0, screenX, screenY, hScreenDC, 0, 0, SRCCOPY);

    // 保存 BMP 文件
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = -bmp.bmHeight;  // 负值表示顶部到底部的存储方式
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);

    GetDIBits(hScreenDC, hBitmap, 0, (UINT)bmp.bmHeight, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // 创建文件
    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "无法创建文件!" << std::endl;
        return false;
    }

    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42;  // BM 头标识

    DWORD dwWritten;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwWritten, NULL);

    // 释放资源
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    CloseHandle(hFile);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);

    std::cout << "截图已保存到 " << filename << std::endl;
    return true;
}


// 查找窗口并点击指定区域
void ClickWindowRegion(const std::wstring & windowTitle,int x, int y) {
    HWND hwnd = FindWindow(NULL, windowTitle.c_str()); // 通过窗口标题查找窗口
    if (!hwnd) {
        std::cerr << "窗口未找到!" << std::endl;
        return;
    }

    // 获取窗口坐标
    RECT rect;
    GetWindowRect(hwnd, &rect);
    std::cout << rect.left << "," << rect.right << "." << rect.top << "," << rect.bottom;
    // 计算屏幕坐标
    int screenX = rect.left + x;
    int screenY = rect.top + y;

    // 设置鼠标位置
    SetCursorPos(screenX, screenY);

    // 模拟鼠标点击
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // 按下左键
    Sleep(50);  // 短暂延迟
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // 释放左键
}

int maimn() {
    Sleep(3000); // 等待 3 秒，方便切换到目标窗口
    std::wstring windowTitle = L"羊了个羊：星球";
    ClickWindowRegion(windowTitle, 100, 200); // 修改窗口标题和点击坐标
    return 0;
}

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
    std::cout <<"L"<< axis.left <<"T"<< axis.top <<"R"<< axis.right <<"B"<< axis.bottom<<std::endl;
    return axis;
}
bool CaptureAndCropScreen(const char* filename, int x1, int y1, int x2, int y2) {
    // 计算裁剪区域的宽度和高度
    int cropWidth = x2 - x1;
    int cropHeight = y2 - y1;

    if (cropWidth <= 0 || cropHeight <= 0) {
        std::cerr << "无效的裁剪区域!" << std::endl;
        return false;
    }

    // 获取屏幕 DC
    HDC hScreenDC = GetDC(NULL);
    HDC hMemDC = CreateCompatibleDC(hScreenDC);

    // 创建兼容的位图（用于裁剪区域）
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, cropWidth, cropHeight);
    SelectObject(hMemDC, hBitmap);

    // 复制屏幕的指定区域到 hMemDC
    BitBlt(hMemDC, 0, 0, cropWidth, cropHeight, hScreenDC, x1, y1, SRCCOPY);

    // 保存 BMP 文件
    BITMAP bmp;
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    BITMAPFILEHEADER bmfHeader;
    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = -bmp.bmHeight;  // 负值表示从上到下存储
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    DWORD dwBmpSize = ((bmp.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmp.bmHeight;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);

    GetDIBits(hScreenDC, hBitmap, 0, (UINT)bmp.bmHeight, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // 创建 BMP 文件
    HANDLE hFile = CreateFileA(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "无法创建文件!" << std::endl;
        return false;
    }

    DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmfHeader.bfSize = dwSizeofDIB;
    bmfHeader.bfType = 0x4D42;  // BM 头标识

    DWORD dwWritten;
    WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
    WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
    WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwWritten, NULL);
    // 释放资源
    GlobalUnlock(hDIB);
    GlobalFree(hDIB);
    CloseHandle(hFile);
    DeleteObject(hBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);
    std::cout << "截图已裁剪并保存到 " << filename << std::endl;
    return true;
}

void getprogram_Screen(std::wstring windowTitle) {
    auto hwnd = gethandle(windowTitle);
    auto axis = getaxis(hwnd);
    CaptureAndCropScreen("img.bmp", axis.left, axis.top, axis.right, axis.bottom);
    return;
}
#include <locale>
int wmain(int argc, wchar_t *argv []) {
    setlocale(LC_ALL, "");
    std::wstring windowTitle = L"";
    //转化为wstring
    std::vector<std::wstring> args;
    for (int i = 0;i < argc;i++) {
        args.push_back(argv[i]);
        //wprintf(L"%s\n", args[i]);
    }
    for (int i = 0;i < argc;i++) {
        //wprintf(L"%s\n",args[i]);
        if (args[i] == L"-windowTitle" && i + 1 < argc) {
            windowTitle = args[i + 1];
            goto scp;
        }
    }
    std::cout << "没有输入窗口名 参数为 -windowTitle 窗口名"<<std::endl;
    return -1;//没有找到参数 退出
    //if(windowTitle==L"") windowTitle = L"羊了个羊：星球";//默认值
scp:
    wprintf(L"%s\n", windowTitle);
    getprogram_Screen(windowTitle);
    return 0;
}