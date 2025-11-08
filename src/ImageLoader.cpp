#include "ImageLoader.h"
#include <iostream>
#include <filesystem>

cv::Mat ImageLoader::loadImage(const std::string& filepath) {
    // TODO 2.1: Implement image loading
    // HINTS:
    // - Use cv::imread() to load the image
    // - Check if the image is empty after loading
    // - If empty, set lastError with a meaningful message
    // - Print a success message if loaded correctly
    
    cv::Mat image;
    
    // YOUR CODE HERE (5-10 lines)
    // Example structure:
    image = cv::imread(filepath);
    if (image.empty()) {
        lastError = "Failed to load image: " + filepath;
        return image;
    }
    std::cout << "Successfully loaded: " << filepath << std::endl;
    
    
    return image;
}

std::vector<cv::Mat> ImageLoader::loadImagesFromDirectory(const std::string& directory) {
    std::vector<cv::Mat> images;
    
    // TODO 2.2: Load all images from directory
    // HINTS:
    // - Use std::filesystem::directory_iterator (C++17)
    // - Filter for image extensions (.jpg, .png, .bmp)
    // - Use loadImage() for each file
    // - Only add images that loaded successfully
    
    // YOUR CODE HERE (10-15 lines)
    // Example structure:
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            if (ext == ".jpg" || ext == ".png" || ext == ".bmp") {
                cv::Mat img = loadImage(entry.path().string());
                if (!img.empty()) {
                    images.push_back(img);
                }
            }
        }
    }
    
    
    std::cout << "Loaded " << images.size() << " images from " << directory << std::endl;
    return images;
}

bool ImageLoader::isValidImage(const cv::Mat& image) const {
    // TODO 2.3: Implement validation checks
    // Check if image:
    // - Is not empty
    // - Has reasonable dimensions (e.g., width and height > 10 pixels)
    // - Has valid type (8-bit or 16-bit)
    
    // YOUR CODE HERE (5-7 lines)
    if (image.empty()) return false;
    if (image.rows < 10 || image.cols < 10) return false;
    if (image.depth() != CV_8U && image.depth() != CV_16U) return false;
    // ... add more checks
    
    
    return true; // Replace with actual validation
}