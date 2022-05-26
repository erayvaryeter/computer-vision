# About The Repository
This repository contains my self developed computer vision related libraries and their command line interface applications showing the usage and results.

# Requirements for Building
This is a complete CMake project, which means it is cross platform therefore possible to build on linux as well, however I developed and tested only on Windows 10 with Visual Studio 2019.
Naturally, CMake needs to be installed on your machine.
This repository is using conan package manager. Most of the used libraries are already existing in conan-center remote, which means installing conan on your machine should be sufficient for gathering most of the libraries.
However, I am using OpenCV/4.5.0 with a different configuration other than the existing packages, therefore it is built locally.

Building OpenCV/4.5.0 with correct configuration:
  - You must checkout https://github.com/conan-io/conan-center-index repository to your local.
  - Go to conan-center-index\recipes\opencv\4.x, open the terminal here (make sure conan executable is in your PATH environment
  - Run the command: conan create . opencv/4.5.0@ -s build_type=Debug -o opencv:contrib=True -o opencv:nonfree=True
  - Run the command: conan create . opencv/4.5.0@ -s build_type=Release -o opencv:contrib=True -o opencv:nonfree=True

If you have NVIDIA CUDA enabled GPU, you can create OpenCV package with CUDA support, which will result in a lot faster execution of the applications:
  - Run the command: conan create . opencv/4.5.0@ -s build_type=Debug -o opencv:contrib=True -o opencv:nonfree=True -o opencv:with_cuda=True -o opencv:with_cudnn=True -o opencv:dnn_cuda=True -o opencv:with_cublas=True
  - Run the command: conan create . opencv/4.5.0@ -s build_type=Release -o opencv:contrib=True -o opencv:nonfree=True -o opencv:with_cuda=True -o opencv:with_cudnn=True -o opencv:dnn_cuda=True -o opencv:with_cublas=True

After you successfuly build the complete project, you can run each executable.

# Explanation

# 1. Base

base directory contains helper libraries for assertion, date & time, file & folder handling, logger and string helpers.
You can see the usage of each library in their corresponding unit tests.

# 2. Features

Feature is a piece of information about the content of an image, typically about whether a certain region of the image has certain properties. Features might be specific structures in the image such as points, edges or objects. Correctly extracted features can be used for computer vision tasks like object detection/classification/segmentation. Some commonly used feature extraction methods are implemented in this project.

# 2.1 Feature Detector

Feature detection contains two steps: keypoint extraction and descriptor extraction. Keypoint usually contains the patch 2D position and other related information such as scale and orientation of the image feature. Descriptor contains the visual description of the patch and it is used to compare the similarity between image features. Not every method that will be mentioned here provides both functionalities. For the methods that only provide feature extraction, brisk method is used to extract the descriptor of the found feature in the code implementation in this project.

# 2.1.1 Harris Corner Detector
Harris corner detector takes the differential of the corner score into account with reference to direction directly, and proved to be more accurate in distinguishing between edges and corners. This method only provides the keypoints, therefore brisk is used to calculate the descriptor. If you run feature-detector-cli executable with the following command line arguments, you will get the following chessboard image containing detected points:
  "--image",
  "../../../../features/feature-detector/resource/Chessboard.jpg",
  "--detector-type",
  "HarrisCorner"
  Example Output:
  ![image](https://user-images.githubusercontent.com/80865017/170388434-449d013d-7805-46cf-b73f-ffec3105dbff.png)

# 2.1.2 Shi Tomasi Corner Detector
This is similar to Harris Corner detector with slight changes in score calculation formulate, which in the end provides slightly better results overall. Same executable can be run by changing the command line argument from "HarrisCorner" to "ShiTomasi". This method also only provides the keypoints, and descriptors are extracted with brisk method.

# 2.1.3 SIFT (Scale Invariant Feature Transform)
This method transforms an image into a large collection of feature vectors, each of which is invariant to image translation, scaling and rotation, and partially invariant to illumination changes and robust to local geometric distortion. SIFT method provides both features and descriptors. Same executable can be run by changing the argument to "SIFT".

# 2.1.4 SURF
This method is inspired by SIFT, however several times faster and claimed to be more robust against different image transformations. This method provides both features and descriptors, and can be run by changing the argument to "SURF".

