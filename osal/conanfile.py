from conans import ConanFile, CMake, tools

class OsalConan(ConanFile):
    settings = "build_type"
    generators = "cmake"
    
    name = "osal"
    version = "1.0.0"
    license = "proprietary"
    description = "operating system abstraction layer"
    exports_sources = [ "*" ]
    
    def requirements(self):
        pass
        
    def build(self):
        for build_type in ["Debug", "Release"]:
            for platform in ["windows", "rtp", "vxworks"]:
                cmake = CMake(self, generator='MinGW Makefiles', build_type=build_type)
                if (build_type == "Debug"):
                    cmake.definitions["CMAKE_DEBUG_POSTFIX"] = "d"
                if (platform == "rtp"):
                    if (build_type == "Debug"):
                        cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = "cmake/toolchains/toolchain_rtp_debug.cmake"
                    elif (build_type == "Release"):
                        cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = "cmake/toolchains/toolchain_rtp_release.cmake"
                elif (platform == "vxworks"):
                    if (build_type == "Debug"):
                        cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = "cmake/toolchains/toolchain_vxworks_debug.cmake"
                    elif (build_type == "Release"):
                        cmake.definitions["CMAKE_TOOLCHAIN_FILE"] = "cmake/toolchains/toolchain_vxworks_release.cmake"
            cmake.configure(build_folder=self.build_folder + "/" + build_type + "/" + platform)
            cmake.build()
            cmake.install()
        
    def package(self):
        self.copy("*.h", dst="include", keep_path=False)
        self.copy("*.lib", dst="build/Debug/lib", src='build/Debug/lib', keep_path=False)
        self.copy("*.dll", dst="build/Debug/bin", src='build/Debug/bin', keep_path=False)
        self.copy("*.so", dst="build/Debug/lib", src='build/Debug/lib', keep_path=False)
        self.copy("*.a", dst="build/Debug/lib", src='build/Debug/lib', keep_path=False)
        
    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)