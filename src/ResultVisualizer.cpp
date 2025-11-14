#include "ResultVisualizer.h"
#include <fstream>
#include <iomanip>

cv::Mat ResultVisualizer::drawScratches(const cv::Mat& image, 
                                       const std::vector<Scratch>& scratches) {
    // Create a copy to draw on
    cv::Mat result = image.clone();
    
    // Convert to color if grayscale
    if (result.channels() == 1) {
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }
    
    // TODO 4.1: Draw each scratch
    // For each scratch:
    // 1. Draw the contour in red
    // 2. Draw the bounding box in yellow
    // 3. Draw a circle at the center in green
    // 4. Add a label with the scratch number
    
    int scratchNum = 1;
    
    for (const auto& scratch : scratches) {
        // Draw contour
        std::vector<std::vector<cv::Point>> contours = {scratch.contour};
        //cv::drawContours(result, contours, 0, cv::Scalar(0, 0, 255), 2);
   
        // Draw bounding box
        cv::rectangle(result, scratch.boundingBox, cv::Scalar(0, 255, 255), 2);

        //Draw rotated box
        cv::Point2f vertices[4];
        scratch.rotatedBox.points(vertices);
        for(int j = 0; j< 4 ; ++j) {
            cv::line(result, vertices[j], vertices[(j+1)%4], cv::Scalar(0, 120, 120), 2);
        }
       
        // Draw center point
        cv::circle(result, scratch.centerPoint, 3, cv::Scalar(0, 255, 0), -1);
        
        // Add label
        std::string label = "S" + std::to_string(scratchNum++);
        cv::putText(result, label, 
                    cv::Point(scratch.boundingBox.x, scratch.boundingBox.y - 5),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 255, 255), 2);
    }
    return result;
}

cv::Mat ResultVisualizer::createResultImage(const cv::Mat& image,
                                           const std::vector<Scratch>& scratches) {
    // Draw scratches on image
    cv::Mat annotated = drawScratches(image, scratches);
    
    // TODO 4.2: Create info panel
    // Create a panel showing:
    // - Number of scratches
    // - Status (PASS/FAIL based on threshold)
    // - Largest scratch info
    
    // Create a white panel
    int panelHeight = 150;
    cv::Mat panel = cv::Mat::ones(panelHeight, annotated.cols, CV_8UC3) * 255;
    
    // Example structure:
    // - Calculate statistics (count, max length, etc.)
    // - Draw text on panel using cv::putText()
    // - Add colored status indicator
    // 
    int yPos = 30;
    cv::putText(panel, "Defect Detection Results", 
               cv::Point(20, yPos), cv::FONT_HERSHEY_PLAIN, 1.0,
               cv::Scalar(0, 0, 0), 2);
    yPos += 40;
    
    std::string countText = "Scratches Found: " + std::to_string(scratches.size());
    cv::putText(panel, countText, cv::Point(20, yPos), 
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 0), 2);
    // ... add more info
    
    
    // TODO 4.3: Combine image and panel
    // Use cv::vconcat() to stack them vertically
    
    cv::Mat result;
    
    // YOUR CODE HERE (2 lines)
    cv::vconcat(annotated, panel, result);
    
    
    return result;
}

bool ResultVisualizer::saveResult(const cv::Mat& image, 
                                  const std::string& filepath) {
    // TODO 4.4: Save the image
    // HINTS:
    // - Use cv::imwrite()
    // - Check return value for success
    // - Print success/failure message
    
    bool success = cv::imwrite(filepath, image);
    if (success) {
        std::cout << "Saved result to: " << filepath << std::endl;
    } 
    else {
        std::cerr << "Failed to save: " << filepath << std::endl;
    }
    return success;
}

void ResultVisualizer::generateReport(const std::vector<Scratch>& scratches,
                                     const std::string& filepath) {
    // TODO 4.5: Generate text report
    // Create a detailed text file with:
    // - Summary statistics
    // - Detailed list of each scratch with properties
    
    // YOUR CODE HERE (20-30 lines)
    std::ofstream report(filepath);

    report << "=== Scratch Detection Report ===\n\n";
    report << "Total Scratches: " << scratches.size() << "\n";
    report << "Status: " << (scratches.size() > 5 ? "FAILED" : "PASSED") << "\n\n";

    report << "Detailed List:\n";
    report << std::setw(5) << "ID" 
           << std::setw(15) << "Center (x,y)"
           << std::setw(12) << "Length"
           << std::setw(12) << "Angle\n";
    report << std::string(50, '-') << "\n";
    for (size_t i = 0; i < scratches.size(); ++i) {
        const auto& s = scratches[i];
        report << std::setw(5) << i+1
               << std::setw(8) << static_cast<int>(s.centerPoint.x) 
               << "," << std::setw(5) << static_cast<int>(s.centerPoint.y)
               << std::setw(12) << std::fixed << std::setprecision(2) << s.length
               << std::setw(12) << s.angle << "\n";
    } 
    report.close();
    std::cout << "Report saved to: " << filepath << std::endl;
}