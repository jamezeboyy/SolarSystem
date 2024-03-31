from conan import ConanFile
from conan.tools.cmake import cmake_layout


class SolarSystemRequirements(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("opengl/system")

    def layout(self):
        cmake_layout(self)
