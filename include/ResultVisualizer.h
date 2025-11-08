#ifndef RESULT_VISUALIZER_H
#define RESULT_VISUALIZER_H

#include "ScratchDetector.h"
#include <opencv2/opencv.hpp>
#include <string>

/**
 * @brief Class for visualizing detection results
 */
class ResultVisualizer {
public:
    /**
     * @brief Draw scratches on the image
     * @param image Original image
     * @param scratches Detected scratches
     * @return Image with annotations
     */
    cv::Mat drawScratches(const cv::Mat& image, 
                         const std::vector<Scratch>& scratches);
    
    /**
     * @brief Create a detailed result image with info panel
     * @param image Original image
     * @param scratches Detected scratches
     * @return Composite image with annotations and info
     */
    cv::Mat createResultImage(const cv::Mat& image,
                             const std::vector<Scratch>& scratches);
    
    /**
     * @brief Save result to file
     * @param image Result image
     * @param filepath Output file path
     */
    bool saveResult(const cv::Mat& image, const std::string& filepath);
    
    /**
     * @brief Generate a text report
     * @param scratches Detected scratches
     * @param filepath Output file path
     */
    void generateReport(const std::vector<Scratch>& scratches,
                       const std::string& filepath);
};

#endif // RESULT_VISUALIZER_H