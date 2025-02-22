
#include"mouse.h"

HWND gethandle(const std::wstring& windowTitle) {
    HWND hwnd = FindWindow(NULL, windowTitle.c_str()); // ͨ�����ڱ�����Ҵ���
    if (!hwnd) {
        std::cerr << "����δ�ҵ�!" << std::endl;
        exit(1);
    }
    return hwnd;
}
RECT getaxis(HWND hwnd) {
    // ��ȡ��������
    RECT axis;
    GetWindowRect(hwnd, &axis);
    //GetClientRect(hwnd, &axis);
    std::cout << "L" << axis.left << "T" << axis.top << "R" << axis.right << "B" << axis.bottom << std::endl;
    return axis;
}

// ���Ҵ��ڲ����ָ������
void mousesmoothmoveto(int x, int y) {
    //��ȡ��ǰ����
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
    //��ȡ��Ļ����
    RECT rect = getaxis(hwnd);
    GetWindowRect(hwnd, &rect);
    //std::cout << rect.left << "," << rect.right << "." << rect.top << "," << rect.bottom;
    //������Ļ���
    int h = rect.bottom - rect.top;
    int w = rect.right - rect.left;
    // ������Ļ����
    int screenX = rect.left + x * w;
    int screenY = rect.top + y * h;

    // �������λ��
    mousesmoothmoveto(screenX, screenY);

    // ģ�������
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // �������

    Sleep(100 + rand() % 25);;  // �����ӳ�
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // �ͷ����

}
void ClickAndMoveWindowRegion(HWND hwnd, double x1, double y1, double x2, double y2) {
    //��ȡ��Ļ����
    RECT rect = getaxis(hwnd);
    GetWindowRect(hwnd, &rect);
    //std::cout << rect.left << "," << rect.right << "." << rect.top << "," << rect.bottom;
    //������Ļ���
    int h = rect.bottom - rect.top;
    int w = rect.right - rect.left;
    // ������Ļ����
    int screenX1 = rect.left + x1 * w;
    int screenY1 = rect.top + y1 * h;
    int screenX2 = rect.left + x2 * w;
    int screenY2 = rect.top + y2 * h;
    // �������λ��
    mousesmoothmoveto(screenX1, screenY1);
    // ģ�������
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // �������
    Sleep(100 + rand() % 25);;  // �����ӳ�
    mousesmoothmoveto(screenX2, screenY2);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // �ͷ����

}
