from conans import ConanFile, CMake, tools

class DevelopmentConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_multi", "txt", "cmake"
    
    name = "computer-vision"
    version = "1.0.0"
    license = "proprietary"
    description = "tools"
    no_copy_source = True
    exports_sources = [ "*" ]
    
    default_options = {
        "opencv:contrib": True,
        "opencv:contrib_freetype": False,
        "opencv:contrib_sfm": False,
        "opencv:cpu_baseline": None,
        "opencv:cpu_dispatch": None,
        "opencv:dnn": True,
        "opencv:dnn_cuda": True,
        "opencv:nonfree": True,
        "opencv:parallel": False,
        "opencv:shared": False,
        "opencv:with_ade": True,
        "opencv:with_cuda": False,
        "opencv:with_eigen": True,
        "opencv:with_ffmpeg": True,
        "opencv:with_imgcodec_hdr": False,
        "opencv:with_imgcodec_pfm": False,
        "opencv:with_imgcodec_pxm": False,
        "opencv:with_imgcodec_sunraster": False,
        "opencv:with_ipp": False,
        "opencv:with_jpeg": "libjpeg",
        "opencv:with_jpeg2000": "jasper",
        "opencv:with_openexr": True,
        "opencv:with_png": True,
        "opencv:with_quirc": True,
        "opencv:with_tiff": True,
        "opencv:with_webp": True,
        "opencv:with_tesseract": False
    }

    def imports(self):
       self.copy("*.dll")
    
    def requirements(self):
        self.requires("cxxopts/[>=3.1.1]@")
        self.requires("catch2/[=3.5.1]@")
        self.requires("spdlog/[=1.12.0]@")
        self.requires("opencv/[=4.9.0]@")
        self.requires("zxing-cpp/[=1.0.8]@")

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.configure()
        return cmake
        
    def build(self):
        cmake = self._configure_cmake()
        cmake.build()
        
    def package(self):
        cmake = self._configure_cmake()
        cmake.install()
        
    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)