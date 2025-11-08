#include "ScratchDetector.h"
#include <iostream>

ScratchDetector::ScratchDetector(const Parameters& params) 
    : params(params) {
    std::cout << "Scratch Detector initialized with parameters:" << std::endl;
    std::cout << "  - Blur kernel: " << params.blurKernelSize << std::endl;
    std::cout << "  - Canny thresholds: " << params.cannyThreshold1 
              << ", " << params.cannyThreshold2 << std::endl;
    std::cout << "  - Min length: " << params.minLength << std::endl;
}

std::vector<Scratch> ScratchDetector::detect(const cv::Mat& image) {
    std::vector<Scratch> scratches;
    
    std::cout << "\n--- Starting Scratch Detection ---" << std::endl;
    
    // Step 1: Preprocess
    processedImage = preprocessImage(image);
    
    // Step 2: Edge detection
    edgeImage = detectEdges(processedImage);
    
    // TODO 3.1: Find contours in the edge image
    // HINTS:
    // - Use cv::findContours()
    // - Mode: cv::RETR_EXTERNAL (only external contours)
    // - Method: cv::CHAIN_APPROX_SIMPLE (compress contours)
    
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(edgeImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    std::cout << "Found " << contours.size() << " contours" << std::endl;
    
    // TODO 3.2: Analyze each contour
    // For each contour:
    // 1. Create a Scratch object
    // 2. Call isScratch() to check if it's actually a scratch
    // 3. If it is, add it to the scratches vector
    
    int scratchCount = 0;
    

    for (const auto& contour : contours) {
        Scratch scratch;
        if (isScratch(contour, scratch)) {
            scratches.push_back(scratch);
            scratchCount++;
        }
    }
    
    std::cout << "Detected " << scratchCount << " scratches" << std::endl;
    
    return scratches;
}

cv::Mat ScratchDetector::preprocessImage(const cv::Mat& image) {
    cv::Mat processed;
    
    std::cout << "Preprocessing image..." << std::endl;
    
    // TODO 3.3: Convert to grayscale if needed
    // HINTS:
    // - Check if image has 3 channels (color) with image.channels()
    // - Use cv::cvtColor() with cv::COLOR_BGR2GRAY
    // - If already grayscale, just clone it
    
    if (image.channels() == 3) {
        cv::cvtColor(image, processed, cv::COLOR_BGR2GRAY);
    } 
    else {
        processed = image.clone();
    }
    
    
    // TODO 3.4: Apply Gaussian blur to reduce noise
    // HINTS:
    // - Use cv::GaussianBlur()
    // - Kernel size: params.blurKernelSize (must be odd!)
    // - Sigma: 0 (auto-calculate based on kernel size)
    
    cv::GaussianBlur(processed, processed, cv::Size(params.blurKernelSize, params.blurKernelSize), 0);
    
    std::cout << "  Converted to grayscale and blurred" << std::endl;
    
    return processed;
}

cv::Mat ScratchDetector::detectEdges(const cv::Mat& image) {
    cv::Mat edges;
    
    std::cout << "Detecting edges..." << std::endl;
    
    // TODO 3.5: Apply Canny edge detection
    // HINTS:
    // - Use cv::Canny()
    // - Parameters: image, output, threshold1, threshold2
    // - Use params.cannyThreshold1 and params.cannyThreshold2
    
    cv::Canny(image, edges, params.cannyThreshold1, params.cannyThreshold2);
    
    std::cout << "  Edge detection complete" << std::endl;
    
    return edges;
}

bool ScratchDetector::isScratch(const std::vector<cv::Point>& contour, 
                                Scratch& scratch) {
    // TODO 3.6: Calculate contour properties
    
    // Step 1: Get bounding rectangle
    // YOUR CODE HERE (1 line)
    cv::Rect bbox = cv::boundingRect(contour);
    // cv::Rect bbox;
    
    
    // Step 2: Calculate dimensions
    double width = bbox.width;
    double height = bbox.height;
    double length = std::max(width, height);
    double thickness = std::min(width, height);
    
    // TODO 3.7: Check if it matches scratch criteria
    // A scratch is:
    // - Long enough (length >= params.minLength)
    // - Thin enough (thickness <= params.maxWidth)
    // - Elongated (aspect ratio >= params.minAspectRatio)
    
    // YOUR CODE HERE (5-10 lines)
    // Calculate aspect ratio
    double aspectRatio = length / (thickness + 0.1); // +0.1 to avoid division by zero
    
    // Check all criteria
    bool isValid = (length >= params.minLength) && (thickness <= params.maxWidth) 
                    && (aspectRatio >= params.minAspectRatio);
    std::cout<< isValid <<":"<< length <<","<< thickness <<std::endl;
    if (!isValid) {
        return false;
    }
    
    // TODO 3.8: Fill in the Scratch structure
    // Calculate:
    // - contour (already provided)
    // - boundingBox
    // - length
    // - angle (use cv::minAreaRect and RotatedRect::angle)
    // - centerPoint
    
    // YOUR CODE HERE (10-15 lines)
    scratch.contour = contour;
    scratch.boundingBox = bbox;
    scratch.length = length;
    
    // Get angle using minimum area rectangle
    cv::RotatedRect rotRect = cv::minAreaRect(contour);
    scratch.angle = rotRect.angle;
    
    // Get center point
    cv::Moments m = cv::moments(contour);
    scratch.centerPoint = cv::Point2f(m.m10 / m.m00, m.m01 / m.m00);
    
    return isValid;
}