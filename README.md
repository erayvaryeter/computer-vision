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

# 2.1.4 SURF (Speeded up Robust Features)
This method is inspired by SIFT, however several times faster and claimed to be more robust against different image transformations. This method provides both features and descriptors, and can be run by changing the argument to "SURF".

# 2.1.5 FAST (Features from Accelerated Segment Test)
FAST detector uses a circle of 16 pixels to classify whether a candidate point p is actually a corner. Each pixel in the circle is labeled from integer number from 1 to 16 clockwise. If a set of N contiguous pixels in the circle are all brighter than the intensity of candidate pixel p + threshold value, or all darker than the intensity of candidate pixel p - threshold value, then p is classified as corner. This method provides only keypoints. Executable can be run by changing the argument to "FAST".

# 2.1.6 BRIEF (Binary Robust Independent Elementary Features)
BRIEF is another algorithm with scale and rotation invariance. It constructs the feature descriptor of the local image through the grayscale relationship of random point pairs in the neighborhood of the local image, and obtains the binary feature descriptor. This method provides both features and descriptors, and can be run by changing the argument to "BRIEF".

# 2.1.7 ORB (Oriented FAST and Rotated BRIEF)
ORB is a fast robust local feature detector. It is based on FAST keypoint detector and modified version of the visual BRIEF descriptor, which has an aim of providing a fast and efficient alternative to SIFT. This method provides both features and descriptors, and can be run by changing the argument to "ORB".

# 2.1.8 BRISK (Binary Robust Invariant Scalable Keypoints)
BRISK has a predefined sampling pattern as compared to BRIEF or ORB. Pixels are sampled over concentric rings. For each sampling point, a small patch is considered around it. Before starting the algorithm, the patch is smoothed using gaussian smoothing. This method provides both features and descriptors, and can be run by changing the argument to "BRISK".

# 2.2 Feature Matching
Feature matching is the process of defining a distance calculation function which is then used to calculate the distance between a particular descriptor in the first image with all descriptors in the second image. If the calculated distance is lower than a certain user defined threshold, these two descriptors can be considered as a good match. OpenCV offers FLANN based and Brut Force matchers. BFMatcher is going to try all the possibilities (which is the meaning of "Brute Force" and hence it will find the best matches. FLANN, meaning "Fast Library for Approximate Nearest Neighbors", will be much faster but will find an approximate nearest neighbors. It will find a good matching, but not necessarily the best possible one. In this project BFMatcher is used to get good results.

It is possible to use different matching functionalities. This project offers regular matching, knn matching (k parameter is given as the maximum count of best matches found per descriptor), and radius matching (which gives the option to define a threshold for the distance between the matched descriptors).

This project offers a cli which you can try by the following arguments, and you should get this output where you can see matching of extracted features.
  "--image1",
  "../../../../features/feature-matcher/resource/1.jpg",
  "--image2",
  "../../../../features/feature-matcher/resource/2.jpg",
  "--matching-type",
  "Radius"
![image](https://user-images.githubusercontent.com/80865017/170466681-ddc3d17f-4464-428c-99ae-c8d194eca1aa.png)

You can change the matching type command line argument to "Regular" or "Knn" to see different results.

# 3. Homography

Homography describes the projective geometry of two cameras and a world plane. In simple terms, homography maps images of points which lie on a world plane from one camera view to another. Two images of the same planar surface in space are related by a homography (assuming a pinhole camera model). This has many practical applications, such as image rectification, image registration, or camera motion—rotation and translation—between two images

# 3.1 Homography Calculator

Homography calculator cli program finds feature points of two images which have common area of view with different perspectives. Found features are then used to calculate the homography matrix. Homography matrix is used to warp the source image perspectively. At the end, original image is displayed with 50 percent weight and the warped perspective image is displayed with 50 percent weight. As it can be seen from the output image, very good features are selected, which is why we see a greatly colored area.

First input image:

![image](https://user-images.githubusercontent.com/80865017/170826690-7f8a4228-85e3-482d-ade1-895a28c53458.png)

Second input image:

![image](https://user-images.githubusercontent.com/80865017/170826704-71dcf3a7-ff63-48f4-8bec-0e913418ec13.png)

Output image:

![image](https://user-images.githubusercontent.com/80865017/170826493-e8f6cb49-91be-4e5c-974d-bd04396e907d.png)

# 3.2 Stitching

OpenCV provides stitching functionality from images that cover common areas. Implementation is quite simple, cli can be run without any command line argument. Program uses those 3 following images as input:
![image](https://user-images.githubusercontent.com/80865017/170470898-49e99eea-4548-453f-a7a6-42aa7b91984c.png)
![image](https://user-images.githubusercontent.com/80865017/170470958-dcd97a9a-b20d-4da0-8635-b1ced5cf7c33.png)
![image](https://user-images.githubusercontent.com/80865017/170471003-9d9035d5-cffe-433e-bceb-c7df00dc75d9.png)

As the result of stitching, we get this stitched output:
![image](https://user-images.githubusercontent.com/80865017/170471121-ca859e8f-410b-4f70-9e40-eece6ebaac4f.png)

# 4. Machine Learning

This repository provides implementations for 3 algorithms: PCA, KNN and Random Forest. 

PCA (Principal component analysis is a technique for reducing the dimensionality of datasets by increasing the interpretability but at the same time minimizing the information loss. This is being done by creating new uncorrelated variables that successively maximize the variance. PCA can be considered as a feature extraction method as well.

KNN (K-Nearest Neighbor) is a classification method which has an output of a class membership. An object is classified by a plurality vote of its neighbors with the object being assigned to the class most common among it's k nearest neighbors.

Random forest is a supervised machine learning algorithm that is used widely in classification and regression problems. It builds decision trees on different samples and takes their majority vote for classification and average in case of regression. 

This repository uses MNIST dataset for classification. KNN and Random Forest methods are used and two cli programs are provided for their implementations. Results of classifications can be obtained by running the executables.

Example output:

![image](https://user-images.githubusercontent.com/80865017/170828393-20ebb84e-b363-49af-9d48-20f070bdb9e2.png)

# 5. Deep Learning

# 5.1 Face Detection and Age/Gender/Ethnicity Estimation

OpenCV supports functionality for running pre-trained neural networks. This project provides some neural networks for face detection and age/gender/ethnicity estimations. Classification networks that are used for estimation tasks are trained by me by taking ResNet architecture as the base structure. Dataset that was used for this training can be seen from https://www.kaggle.com/datasets/jangedoo/utkface-new

Age prediction is not working well as there was not enough number of images for each age group. Running face detection command line interface with proper command line arguments results in the following output:

![image](https://user-images.githubusercontent.com/80865017/170828782-c9d9b53c-3370-4cee-b66a-427934867ba6.png)

# 5.2 Face Warping

Image warping is a transformation which maps all positions in one image plane to positions in a second plane, which is very useful for correcting image distortion. In order to have a good face recognition system, it is crucial to warp detected faces to a proper reference plane so that matching will make more sense in the recognition phase. This project provides a warping library and also a command line interface as example. 

Face warping is using the 68 face landmarks as reference points. Homography matrix is found between the landmark points of the currently processed/detected face and reference landmarks points. Afterwards this matrix is used to warp the originally detected face. Example output of face warping can be seen as follows:

![image](https://user-images.githubusercontent.com/80865017/170829050-7f2031b9-dc7f-488b-9a6f-e009994f02bb.png)![image](https://user-images.githubusercontent.com/80865017/170829066-500b3c2b-a91d-4212-854b-5080c65d2a70.png)![image](https://user-images.githubusercontent.com/80865017/170829077-801c8583-c638-4849-8e9f-f91724614d8c.png)
![image](https://user-images.githubusercontent.com/80865017/170829099-9a03fc2b-f5f1-4b18-bd29-eea020f8489b.png)
![image](https://user-images.githubusercontent.com/80865017/170829112-dddd959f-aa2a-4748-954c-a1f8ce3d5bb9.png)

As you can see from the results, detected faces were warped and corrected.

# 5.3 Face Recognition

OpenCV provides face recognition functionalities. This project uses couple images of Brad Pitt, Don Cheadle, Elliott Gould, George Clooney and Matt Damon. Command line interface application demonstrates the training and testing phase of face recognition. However, since there are not many images used for training, recognition results are not good. Example output is as follows:

![image](https://user-images.githubusercontent.com/80865017/170831342-3e235c35-751c-427f-b2c7-46b7ad1b02a7.png)

# 5.4 Instance Segmentation

Instance segmentation is a computer vision task for detecting and localizing an object in an image. Instance segmentation is a natural sequence of semantic segmentation, and it is also one of the biggest challenges compared to other segmentation techniques. The goal of instance segmentation is to get a view of objects of the same class divided into different instances. In order to solve instance segmentation problem, Mask R-CNN model is used, which is a deep neural network based on a faster R-CNN architecture that in addition to classification and detection, also provides mask prediction for each bounding box. 

This repository uses a pre-trained Mask R-CNN neural network to detect many classes including faces, vehicles, etc. Running instance segmentation cli executable will result in the following output: 

![image](https://user-images.githubusercontent.com/80865017/170831638-5ea0b1ac-90d3-42db-be74-3f56fcd7153d.png)

# 6. Tracking

Why do we need tracking at all instead of detecting objects in every frame ? There are numereous reasons why:

- When there are multiple objects (say people) detected in a video frame, tracking helps establishing the identity of objects across frames.
- In some cases, object detection might fail but it may still be possible to track the object because tracking takes into account the location and appearance of the object in the previous frames.
- If we always apply object detection, this will lead to slower execution. However, tracking algorithms are very fast because they do either local/global search. 

If we perform the object detection only in every Nth frame, but just apply tracking algorithms in intermediate frames, we can obtain a very high frame rate !

OpenCV provides 8 different tracker types: BOOSTING, MIL, KCF, TLD, MEDIANFLOW, GOTURN, MOSSE and CSTR. All of them are implemented in this project.

There are 2 cli executables provided: Face Tracking and Instance Segmentation Tracking.

# 6.1 Face Tracking

Face tracking detects faces in the video every 30th frame, and just tracks the last detected faces for the other 29 frames. It also applies the available estimations. It is clear that neural networks for estimations need more work, better data and improvement in general. Example output of the face tracking is as follows:

https://user-images.githubusercontent.com/80865017/170860652-e91d75df-4987-4822-b2de-51dffb36ae15.mp4

# 6.2 Instance Segmentation Tracking

This cli executable detects vehicles in the video every 30th frame, and just tracks the last detected vehicles for the other 29 frames. It also applies instance segmentation and displays the found mask. While displaying the found mask, program stops for a second to display it well because otherwise it will show the mask on the tracked bounding box which is moving, but mask stays right where it is. In order to avoid this, program stops on purpose for a second when mask is detected.

https://user-images.githubusercontent.com/80865017/170860674-566ecfd2-6be8-4fee-87ce-b7b5eaf9d1fa.mp4
