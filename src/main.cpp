#include <opencv2/opencv.hpp>
#include <iostream>
#include <windows.h>
#include <commdlg.h>

#pragma comment(lib, "Comdlg32.lib")

std::string openFileDialog() {
    OPENFILENAMEW ofn;
    wchar_t fileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"Image Files\0*.jpg;*.jpeg;*.png;*.bmp\0All Files\0*.*\0";
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle = L"Select an Image";

    if (GetOpenFileNameW(&ofn)) {
        std::wstring ws(fileName);
        return std::string(ws.begin(), ws.end());
    }

    return "";
}

void show_image(const cv::Mat& image, const std::string& windowName) {
    cv::imshow(windowName, image);
}

int main() {
    cv::setNumThreads(0);

    std::string path = openFileDialog();

    if (path.empty()) {
        std::cout << "No file selected." << std::endl;
        return 0;
    }

    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);

    if (img.empty()) {
        std::cout << "Failed to load image." << std::endl;
        return -1;
    }
    
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    
    cv::Mat rot;
    cv::rotate(img, rot, cv::ROTATE_90_CLOCKWISE);
    
    cv::Mat resizable;
    cv::resize(img, resizable, cv::Size(), 0.5, 0.5);
    
    cv::Mat blur;
    cv::GaussianBlur(img, blur, cv::Size(5, 5), 1.7);
    
    cv::Mat edge;
    cv::Canny(img, edge, 50, 150);
    
    show_image(img, "Original image");
    show_image(gray, "Grayscale");
    show_image(rot, "Rotated");
    show_image(resizable, "Resized");
    show_image(blur, "Blurred");
    show_image(edge, "Edges");
    
    std::cout << "All windows are open. Close windows to exit..." << std::endl;
    
    while (true) {
        int key = cv::waitKey(100);
        
        bool anyWindowVisible = false;
        std::vector<std::string> windows = {"Original image", "Grayscale", "Rotated", "Resized", "Blurred", "Edges"};
        
        for (const auto& window : windows) {
            if (cv::getWindowProperty(window, cv::WND_PROP_VISIBLE) >= 1) {
                anyWindowVisible = true;
                break;
            }
        }
        
        if (!anyWindowVisible) {
            break;
        }
    }
    
    cv::imwrite("original.jpg", img);
    cv::imwrite("grayscale.jpg", gray);
    cv::imwrite("rotated.jpg", rot);
    cv::imwrite("resized.jpg", resizable);
    cv::imwrite("blurred.jpg", blur);
    cv::imwrite("edges.jpg", edge);
    
    std::cout << "Program finished. Processed images saved to disk." << std::endl;
    
    return 0;
}