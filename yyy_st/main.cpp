#include <opencv2/opencv.hpp>

#include <vector>
#include <iostream>
#include <fstream>

// Function to calculate row and column axes
std::pair<std::vector<int>, std::vector<int>> get_opr_axis() {
    std::ifstream file("opr_axis.txt");  // 打开文件
    if (!file) {
        std::cerr << "无法打开文件！" << std::endl;
        exit(1);
    }

    std::vector<int> row_axis, col_axis;
    std::string line;
    std::getline(file, line);  // 第一行是row_axis数据
    std::stringstream ssrow(line);
    std::string value;
    while (std::getline(ssrow, value, ',')) {  // 按逗号分割
        row_axis.push_back(std::stoi(value));  // 转换为 int
    }
    std::getline(file, line);//第二行是col_axis数据
    std::stringstream sscol(line);
    while (std::getline(sscol, value, ',')) {  // 按逗号分割
        col_axis.push_back(std::stoi(value));  // 转换为 int
    }

    file.close();  // 关闭文件

    return { row_axis, col_axis };
}

// Function to remove edges from an image
cv::Mat remove_edge(const cv::Mat& img) {
    std::vector<cv::Point> coords;
    cv::findNonZero(img, coords);
    if (!coords.empty()) {
        cv::Rect bounding_rect = cv::boundingRect(coords);
        return img(bounding_rect);
    }
    return img;
}

// Function to perform the entire image processing and template matching
void getvec(const std::string& img_path) {
    // Read the original image in grayscale
    cv::Mat srcimg = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
    if (srcimg.empty()) {
        std::cerr << "Could not read the image!" << std::endl;
        return;
    }

    // Threshold the image
    cv::Mat binary_srcimg;
    cv::threshold(srcimg, binary_srcimg, 110, 255, cv::THRESH_BINARY_INV);

    // Get image size and define grid dimensions
    int h_src = binary_srcimg.rows, w_src = binary_srcimg.cols;
    int rows = 16, cols = 10;

    // Get row and column axes
    auto row_axis = get_opr_axis().first;
    auto col_axis = get_opr_axis().second;
    for (int i = 0; i < row_axis.size(); ++i) {
        row_axis[i] = row_axis[i] * h_src / 847; // Normalize
    }
    for (int i = 0; i < col_axis.size(); ++i) {
        col_axis[i] = col_axis[i] * w_src / 463; // Normalize
    }

    // Create blocks
    std::vector<cv::Mat> blocks;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // Crop block from image
            cv::Mat block = binary_srcimg(cv::Rect(col_axis[j], row_axis[i], col_axis[j + 1] - col_axis[j], row_axis[i + 1] - row_axis[i]));
            block = remove_edge(block);
            cv::resize(block, block, cv::Size(25, 25), 0, 0, cv::INTER_LANCZOS4);
            blocks.push_back(block);
        }
    }

    // Read target subimages
    std::vector<cv::Mat> binary_subimgs;
    for (int i = 1; i <= 9; ++i) {
        std::string img_name = "imgs/" + std::to_string(i) + ".jpg";
        cv::Mat subimg = cv::imread(img_name, cv::IMREAD_GRAYSCALE);
        if (subimg.empty()) {
            std::cerr << "Could not read subimage " << img_name << std::endl;
            return;
        }
        cv::Mat binary_subimg;
        cv::threshold(subimg, binary_subimg, 110, 255, cv::THRESH_BINARY_INV);
        binary_subimg = remove_edge(binary_subimg);
        cv::resize(binary_subimg, binary_subimg, cv::Size(25, 25), 0, 0, cv::INTER_LANCZOS4);
        binary_subimgs.push_back(binary_subimg);
    }

    // Prepare result vectors
    std::vector<std::vector<int>> vec(rows, std::vector<int>(cols, 0));
    std::vector<std::vector<float>> vec_prob(rows, std::vector<float>(cols, 0.0f));

    // Perform template matching and store results
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cv::Mat block = blocks[i * cols + j];
            float best_simi = 0;
            int best_index = -1;

            for (size_t idx = 0; idx < binary_subimgs.size(); ++idx) {
                cv::Mat subimg = binary_subimgs[idx];
                cv::Mat result;
                cv::matchTemplate(subimg, block, result, cv::TM_CCOEFF_NORMED);
                double min_val, max_val;
                cv::minMaxLoc(result, &min_val, &max_val);

                if (max_val > best_simi) {
                    best_simi = max_val;
                    best_index = static_cast<int>(idx) + 1;  // Store subimage index
                }
            }

            vec[i][j] = best_index;
            vec_prob[i][j] = best_simi;
        }
    }

    // Save the result matrix to a file
    std::ofstream outfile("input.txt");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            outfile << vec[i][j];
            if (j != cols - 1) {
                outfile << ",";
            }
        }
        outfile << std::endl;
    }

    // Optionally, print the result for verification
    std::cout << "Recognition Matrix (vec):" << std::endl;
    for (const auto& row : vec) {
        for (const auto& value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Probability Matrix (vec_prob):" << std::endl;
    for (const auto& row : vec_prob) {
        for (const auto& value : row) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::string img_path = "img.bmp";  // Replace with your image path
    getvec(img_path);
    return 0;
}