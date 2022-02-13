from conans import ConanFile

required_conan_version = ">=1.33.0"

class OpeCvCudaConan(ConanFile):
    name = "opencv-cuda"
    version = "4.5.3"
    settings = "os", "compiler", "build_type", "arch"

    def package(self):
        self.copy("*.hpp", dst="include", src="D:/opencv-4.5.3/build/install/Debug/include")
        self.copy("*.h", dst="include", src="D:/opencv-4.5.3/build/install/Debug/include")
        
        # required binaries
        self.copy("*.dll", dst="bin", src="D:/opencv-4.5.3/build/install/Debug/x64/vc16/bin")
        
        # required libraries
        self.copy("*.lib", dst="lib", src="D:/opencv-4.5.3/build/install/Debug/x64/vc16/lib")

    def package_info(self):
        self.cpp_info.libs = self.collect_libs()