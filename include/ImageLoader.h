#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

/**
 * @brief Class responsible for loading and basic validation of images
 */
class ImageLoader {
public:
    /**
     * @brief Load a single image from file
     * @param filepath Path to the image file
     * @return Loaded image (empty Mat if failed)
     */
    cv::Mat loadImage(const std::string& filepath);
    
    /**
     * @brief Load multiple images from a directory
     * @param directory Path to directory containing images
     * @return Vector of loaded images
     */
    std::vector<cv::Mat> loadImagesFromDirectory(const std::string& directory);
    
    /**
     * @brief Check if image is valid for processing
     * @param image Image to validate
     * @return true if image is valid, false otherwise
     */
    bool isValidImage(const cv::Mat& image) const;
    
    /**
     * @brief Get the last error message
     */
    std::string getLastError() const { return lastError; }
    
private:
    std::string lastError;
};

#endif // IMAGE_LOADER_H