#include "ImageLoader.h"
#include "ScratchDetector.h"
#include "ResultVisualizer.h"
#include <iostream>
#include <filesystem>

void processImage(const std::string& imagePath);
void processBatch(const std::string& directory);
void createTestImage();
void practiceMorphology();
void practiceEdgeDetection();

int main(int argc, char** argv) {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "    Scratch Detection System v1.0      \n";
    std::cout << "========================================\n\n";
    
    if (argc < 2) {
        std::cout << "Usage for Scratch Detection:\n";
        std::cout << "  Single image: " << argv[0] << " <image_path>\n";
        std::cout << "  Batch mode:   " << argv[0] << " --batch <directory>\n";
        std::cout << "Now for practice OpenCV:\n";
        //practiceMorphology();
        //practiceEdgeDetection();
        createTestImage();
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
    params.maxWidth = 15;
    params.minAspectRatio = 5.0;

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
    visualizer.saveResult(image, "output/original.jpg");
    visualizer.saveResult(detector.getEdgeImage(), "output/edges.jpg");
    visualizer.saveResult(result, "output/result.jpg");
    visualizer.generateReport(scratches, "output/report.txt");
    std::cout << "\nPress any key to close windows..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void processBatch(const std::string& directory) {
    std::cout << "Batch processing: " << directory << "\n\n";
    
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

void createTestImage() {
    cv::Mat img = cv::Mat::zeros(500, 700, CV_8UC3);
    img.setTo(cv::Scalar(220, 220, 220)); // Gray background
    
    // Draw a scratch (thin dark line)
    cv::line(img, cv::Point(100, 100), cv::Point(400, 150), 
             cv::Scalar(50, 50, 50), 2);
    cv::line(img, cv::Point(200, 450), cv::Point(340, 300), 
             cv::Scalar(50, 250, 50), 3);
    cv::line(img, cv::Point(50, 150), cv::Point(100, 250), 
             cv::Scalar(50, 50, 250), 6);
    cv::line(img, cv::Point(450, 350), cv::Point(300, 450), 
             cv::Scalar(50, 50, 0), 7);
    cv::line(img, cv::Point(100, 150), cv::Point(400, 250), 
             cv::Scalar(150, 50, 250), 6);
    cv::line(img, cv::Point(350, 50), cv::Point(600, 350), 
             cv::Scalar(50, 50, 150), 10);
   
    // Add noise
    cv::Mat noise(img.size(), img.type());
    cv::randn(noise, cv::Scalar::all(0), cv::Scalar::all(10));
    img += noise;
    
    cv::imwrite("test2.png", img);
}

void practiceMorphology() {
    std::string imagePath = "images/detect_blob.png";
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

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5), cv::Point(2, 2));
    cv::Mat derode;
    //std::string window_name = "Morphology Demo";
    cv::morphologyEx(image, derode, cv::MORPH_ERODE, element);
    cv::imwrite("output/morphology_erode.jpg", derode);
    cv::Mat ddilate;
    cv::morphologyEx(image, ddilate, cv::MORPH_DILATE, element);
    cv::imwrite("output/morphology_dilate.jpg", ddilate);
    cv::Mat dopen;
    cv::morphologyEx(image, dopen, cv::MORPH_OPEN, element);
    cv::imwrite("output/morphology_open.jpg", dopen);
    cv::Mat dclose;
    cv::morphologyEx(image, dclose, cv::MORPH_CLOSE, element);
    cv::imwrite("output/morphology_close.jpg", dclose);
    cv::Mat dgradient;
    cv::morphologyEx(image, dgradient, cv::MORPH_GRADIENT, element);
    cv::imwrite("output/morphology_gradient.jpg", dgradient);
    cv::Mat dtophat;
    cv::morphologyEx(image, dtophat, cv::MORPH_TOPHAT, element);
    cv::imwrite("output/morphology_tophat.jpg", dtophat);
    cv::Mat dblackhat;
    cv::morphologyEx(image, dblackhat, cv::MORPH_BLACKHAT, element);
    cv::imwrite("output/morphology_blackhat.jpg", dblackhat);

    //cv::imshow(window_name, dst);
    //cv::waitKey(0);
}

void practiceEdgeDetection(){
    // Step 1: Load image
    std::string imagePath = "images/detect_blob.png";
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Error: " << std::endl;
        return;
    }
    std::cout << "Image size: " << image.cols << "x" << image.rows << "\n\n";
    cv::imwrite("output/gray_scale.jpg", image);

    cv::Mat sobelx, sobely, gradient, gradient_abs;
    cv::Sobel(image, sobelx, CV_64F, 1, 0, 3);
    cv::Sobel(image, sobely, CV_64F, 0, 1, 3);
    cv::magnitude(sobelx, sobely, gradient);
    cv::convertScaleAbs(gradient, gradient_abs);
    cv::imwrite("output/soble_gradient.jpg", gradient_abs);

    cv::Mat laplacian, laplacian_abs;
    cv::Laplacian(image, laplacian, CV_64F);
    cv::convertScaleAbs(laplacian, laplacian_abs);
    cv::imwrite("output/laplacian.jpg", laplacian_abs);

    cv::Mat blur, edges;
    cv::GaussianBlur(image, blur, cv::Size(5, 5), 1.4);
    cv::Canny(blur, edges, 100, 200);
    cv::imwrite("output/blur.jpg", blur);
    cv::imwrite("output/edges.jpg", edges);
}
