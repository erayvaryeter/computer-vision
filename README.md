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

