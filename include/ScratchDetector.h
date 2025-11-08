#ifndef SCRATCH_DETECTOR_H
#define SCRATCH_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>

/**
 * @brief Structure to represent a detected scratch
 */
struct Scratch {
    std::vector<cv::Point> contour;  // The actual scratch shape
    cv::Rect boundingBox;             // Rectangle around the scratch
    double length;                    // Approximate length of scratch
    double angle;                     // Angle of scratch in degrees
    cv::Point2f centerPoint;          // Center point of the scratch
    
    Scratch() : length(0), angle(0) {}
};

/**
 * @brief Class that detects scratches in images
 */
class ScratchDetector {
public:
    /**
     * @brief Configure detection parameters
     */
    struct Parameters {
        // Preprocessing
        int blurKernelSize;           // Size of Gaussian blur kernel (must be odd)
        
        // Edge detection
        double cannyThreshold1;      // Lower threshold for Canny
        double cannyThreshold2;     // Upper threshold for Canny
        
        // Scratch filtering
        double minLength;          // Minimum length to be considered a scratch
        double maxWidth;           // Maximum width (scratches are thin)
        double minAspectRatio;      // Length/width ratio (scratches are elongated)

        Parameters()
            : blurKernelSize(5),
              cannyThreshold1(50),
              cannyThreshold2(150),
              minLength(20.0),
              maxWidth(10.0),
              minAspectRatio(3.0) {} 
    };
    
    /**
     * @brief Constructor with parameters
     */
    ScratchDetector(const Parameters& params = Parameters());
    
    /**
     * @brief Detect scratches in an image
     * @param image Input image (grayscale or color)
     * @return Vector of detected scratches
     */
    std::vector<Scratch> detect(const cv::Mat& image);
    
    /**
     * @brief Get the processed image (for debugging)
     */
    cv::Mat getProcessedImage() const { return processedImage; }
    cv::Mat getEdgeImage() const { return edgeImage; }
    
private:
    Parameters params;
    cv::Mat processedImage;  // Image after preprocessing
    cv::Mat edgeImage;        // Image after edge detection
    
    /**
     * @brief Preprocess the image (convert to grayscale, denoise)
     */
    cv::Mat preprocessImage(const cv::Mat& image);
    
    /**
     * @brief Detect edges in the image
     */
    cv::Mat detectEdges(const cv::Mat& image);
    
    /**
     * @brief Analyze contour to determine if it's a scratch
     */
    bool isScratch(const std::vector<cv::Point>& contour, Scratch& scratch);
};

#endif // SCRATCH_DETECTOR_H