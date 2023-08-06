from conans import ConanFile, CMake, tools

class BaseConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    name = "base-library"
    version = "1.0.0"
    license = "proprietary"
    description = "tools"
    no_copy_source = True
    exports_sources = [ "*" ]

    def imports(self):
       self.copy("*.dll")

    def requirements(self):
        pass

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