from conans import ConanFile

required_conan_version = ">=1.33.0"

class CudaConan(ConanFile):
    name = "cuda"
    version = "11.5"
    settings = "os", "compiler", "build_type", "arch"

    def package(self):
        self.copy("*.hpp", dst="include", src="D:/CUDA/include")
        self.copy("*.h", dst="include", src="D:/CUDA/include")
        
        # required binaries
        self.copy("*.dll", dst="bin", src="D:/CUDA/bin")
        
        # required libraries
        self.copy("*.lib", dst="lib", src="D:/CUDA/lib/x64")

    def package_info(self):
        self.cpp_info.libs = self.collect_libs()