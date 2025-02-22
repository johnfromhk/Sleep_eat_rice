#include"mouse.h"

typedef vector<vector<int>> vec_t2;//2维数组
typedef vector<vector<double>> vec_td2;
vec_t2 readVec(string filepath) {
    std::ifstream file(filepath);  // 打开文件
    if (!file) {
        std::cerr << "无法打开文件！" << std::endl;
        exit(1);
    }

    vec_t2 data;
    std::string line;

    while (std::getline(file, line)) {  // 按行读取
        std::vector<int> row;
        std::stringstream ss(line);
        std::string value;

        while (std::getline(ss, value, ',')) {  // 按逗号分割
            row.push_back(std::stoi(value));  // 转换为 int
        }
        data.push_back(row);
    }

    file.close();  // 关闭文件
    return data;
}
double kx = 0.087362, bx = 0.107088, ky = 0.047693, by = 0.193443;
double coord_tx(int x) {
    return kx * x + bx;
}
double coord_ty(int y) {
    return ky * y + by;
}
vec_td2 coord_transfer(const vec_t2&Vec) {
    vec_td2 result;
    for (auto row : Vec) {
        vector<double> drow = {
            coord_tx(row[1]),
            coord_ty(row[0]),
            coord_tx(row[3]),
            coord_ty(row[2])
        };
        result.push_back(drow);
    }
    return result;
}

int wmain(int argc, wchar_t* argv[]) {
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
        }
        if (args[i] == L"-kx" && i + 1 < argc) {
            try {
                kx = std::stod(args[i + 1]);
            }
            catch (const std::invalid_argument& e) {
                std::cout << "无效的输入,将使用默认kx\n";
            }
        }
        if (args[i] == L"-bx" && i + 1 < argc) {
            try {
                bx = std::stod(args[i + 1]);
            }
            catch (const std::invalid_argument& e) {
                std::cout << "无效的输入,将使用默认bx\n";
            }
        }
        if (args[i] == L"-ky" && i + 1 < argc) {
            try {
                ky = std::stod(args[i + 1]);
            }
            catch (const std::invalid_argument& e) {
                std::cout << "无效的输入,将使用默认ky\n";
            }
        }
        if (args[i] == L"-by" && i + 1 < argc) {
            try {
                by = std::stod(args[i + 1]);
            }
            catch (const std::invalid_argument& e) {
                std::cout << "无效的输入,将使用默认by\n";
            }
        }
    }
    if (windowTitle == L"") {
        std::cout << "没有输入窗口名 参数为 -windowTitle 窗口名" << std::endl;
        return -1;//没有找到参数 退出
    }
    //if(windowTitle==L"") windowTitle = L"羊了个羊：星球";//默认值
    srand(time(0));
    auto result = readVec("result.txt");
    auto tresult = coord_transfer(result);
    Sleep(1000);
    //std::wstring windowTitle = L"羊了个羊：星球";
    auto hwnd = gethandle(windowTitle);
    //ClickAndMoveWindowRegion(hwnd, 0.11136, 0.1451, 0.3, 0.4);
    //ClickWindowRegion(hwnd, 0.1, 0.8);
    for (auto row : tresult) {
        std::cout << "click region" << row[0] << "," << row[1] << "," << row[2] << "," << row[3] << std::endl;
        ClickAndMoveWindowRegion(hwnd, row[0], row[1], row[2], row[3]);
        Sleep(100);
    }

    return 0;
}