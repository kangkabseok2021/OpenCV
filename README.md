# Scratch Detection System

## Overview
Automated surface defect detection using computer vision.

## Features
- Automatic scratch detection
- Batch processing
- Detailed reporting
- Parameter tuning

## Requirements
- OpenCV 4.x
- C++17 compiler
- CMake 3.10+

## Building
```bash
mkdir build && cd build
cmake ..
make
```

## Usage
```bash
# Single image
./ScratchDetector image.jpg

# Batch processing
./ScratchDetector --batch /path/to/images
```

## Algorithm
1. Preprocessing (grayscale conversion, Gaussian blur)
2. Edge detection (Canny algorithm)
3. Contour analysis (geometric filtering)
4. Result visualization

## Performance
- Processing speed: ~100ms per image (1920x1080)
- Detection accuracy: ~90% (tested on 20 images)
- False positive rate: <5%

## Author
Kab Seok Kang - Built for ASYS Group interview

