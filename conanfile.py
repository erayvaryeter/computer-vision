from conans import ConanFile, CMake, tools

class DevelopmentConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_multi", "txt", "cmake"
    
    name = "development"
    version = "1.0.0"
    license = "proprietary"
    description = "tools"
    no_copy_source = True
    exports_sources = [ "*" ]
    
    def config_options(self):
        pass

    def imports(self):
       self.copy("*.dll")
    
    def requirements(self):
        self.requires("cxxopts/[>=3.0.0]@")
        self.requires("catch2/[>=2.13.7]@")
        self.requires("spdlog/[>=1.9.2]@")
        self.requires("opencv/[=4.5.3]@")
        self.requires("cuda/[=11.5]@")

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