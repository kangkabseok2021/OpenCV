#include "ImageLoader.h"
#include "ScratchDetector.h"
#include "ResultVisualizer.h"
#include <iostream>
#include <filesystem>

void processImage(const std::string& imagePath);
void processBatch(const std::string& directory);
cv::Mat createTestImage();

int main(int argc, char** argv) {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "    Scratch Detection System v1.0      \n";
    std::cout << "========================================\n\n";
    
    // TODO 5.1: Handle command-line arguments
    // Support two modes:
    // 1. Single image: ./ScratchDetector image.jpg
    // 2. Batch processing: ./ScratchDetector --batch /path/to/folder
    
    // YOUR CODE HERE (10-15 lines)
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  Single image: " << argv[0] << " <image_path>\n";
        std::cout << "  Batch mode:   " << argv[0] << " --batch <directory>\n";
        cv::Mat image = createTestImage();
        cv::imwrite("test_image.jpg", image);
        cv::waitKey(0);
        return 1;
    }
    
    std::string arg1 = argv[1];
    if (arg1 == "--batch" && argc == 3) {
        processBatch(argv[2]);
    } 
    else {
        processImage(arg1);
    }
    return 0;
}

void processImage(const std::string& imagePath) {
    std::cout << "Processing: " << imagePath << "\n\n";
    
    // TODO 5.2: Complete image processing pipeline
    // 1. Load image
    // 2. Detect scratches
    // 3. Visualize results
    // 4. Save output
    // 5. Generate report
    
    // Step 1: Load image
    ImageLoader loader;
    cv::Mat image = loader.loadImage(imagePath);
    if (image.empty()) {
        std::cerr << "Error: " << loader.getLastError() << std::endl;
        return;
    }

    if (!loader.isValidImage(image)) {
        std::cerr << "Error: Invalid image" << std::endl;
        return;
    }

    std::cout << "Image size: " << image.cols << "x" << image.rows << "\n\n";

    // Step 2: Detect scratches
    ScratchDetector::Parameters params;
    // Tune these parameters!
    params.cannyThreshold1 = 50;
    params.cannyThreshold2 = 150;
    params.minLength = 20;
    params.maxWidth = 8;
    params.minAspectRatio = 3.0;

    ScratchDetector detector(params);
    std::vector<Scratch> scratches = detector.detect(image);

    // Step 3: Visualize
    ResultVisualizer visualizer;
    cv::Mat result = visualizer.createResultImage(image, scratches);

    // Step 4: Display
    cv::imshow("Original", image);
    cv::imshow("Edges", detector.getEdgeImage());
    cv::imshow("Results", result);

    // Step 5: Save
    std::filesystem::create_directories("output");
    visualizer.saveResult(result, "output/result.jpg");
    visualizer.generateReport(scratches, "output/report.txt");
    std::cout << "\nPress any key to close windows..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void processBatch(const std::string& directory) {
    std::cout << "Batch processing: " << directory << "\n\n";
    
    // TODO 5.3: Process multiple images
    // 1. Load all images from directory
    // 2. Process each one
    // 3. Save all results to output folder
    // 4. Generate summary report
    
    ImageLoader loader;
    std::vector<cv::Mat> images = loader.loadImagesFromDirectory(directory);

    if (images.empty()) {
        std::cerr << "No images found in directory" << std::endl;
        return;
    }
    
    ScratchDetector::Parameters params;
    ScratchDetector detector(params);
    ResultVisualizer visualizer;

    std::filesystem::create_directories("output/batch");

    int totalScratches = 0;
    for (size_t i = 0; i < images.size(); ++i) {
        std::cout << "\nProcessing image " << (i+1) << "/" << images.size() << std::endl;
        
        std::vector<Scratch> scratches = detector.detect(images[i]);
        totalScratches += scratches.size();
        
        cv::Mat result = visualizer.createResultImage(images[i], scratches);
        
        std::string outputPath = "output/batch/result_" + std::to_string(i) + ".jpg";
        visualizer.saveResult(result, outputPath);
    }
    
    std::cout << "\n=== Batch Processing Complete ===\n";
    std::cout << "Images processed: " << images.size() << "\n";
    std::cout << "Total scratches: " << totalScratches << "\n";
    std::cout << "Average per image: " << (totalScratches / images.size()) << "\n";
}

cv::Mat createTestImage() {
    cv::Mat img = cv::Mat::zeros(500, 700, CV_8UC3);
    img.setTo(cv::Scalar(180, 180, 180)); // Gray background
    
    // Draw a scratch (thin dark line)
    cv::line(img, cv::Point(100, 100), cv::Point(400, 102), 
             cv::Scalar(50, 50, 50), 2);
    
    // Add noise
    cv::Mat noise(img.size(), img.type());
    cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(10));
    img += noise;
    
    return img;
}