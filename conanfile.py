import os

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration, ConanException
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.env import VirtualRunEnv, VirtualBuildEnv, Environment
from conan.tools.files import copy, update_conandata
from conan.tools.microsoft import check_min_vs, is_msvc_static_runtime, is_msvc
from conan.tools.scm import Version

required_conan_version = ">=2.7"

class CuraFormulaeEngineConan(ConanFile):
    name = "cura-formulae-engine"
    author = "UltiMaker"
    url = "https://github.com/Ultimaker/CuraFormulaeEngine"
    homepage = "https://ultimaker.com"
    description = "Formulae parser and evaluator engine used in Ultimaker Cura"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "enable_extensive_warnings": [True, False],
        "with_apps": [True, False],
    }
    default_options = {
        "enable_extensive_warnings": False,
        "with_apps": False,
    }

    def set_version(self):
        if not self.version:
            self.version = self.conan_data["version"]

    @property
    def _min_cppstd(self):
        return 20

    @property
    def _compilers_minimum_version(self):
        return {
            "gcc": "11",
            "clang": "14",
            "apple-clang": "13",
            "msvc": "192",
            "visual_studio": "17",
        }

    def layout(self):
        cmake_layout(self)
        self.cpp.build.bin = []
        self.cpp.package.bin = []
        self.cpp.package.libs = ["cura-formulae-engine"]
        self.cpp.build.bindirs = []
        self.cpp.package.bindirs = ["bin"]
        self.cpp.package.includedirs = ["include"]

    def requirements(self):
        self.requires("range-v3/[>=0.12.0]", transitive_headers=True)
        self.requires("spdlog/[>=1.14.1]", transitive_headers=True)
        self.requires("fmt/[>=11.0.2]", transitive_headers=True)
        self.requires("foonathan-lexy/[>=2022.12.1]@ultimaker/stable", transitive_headers=True)
        self.requires("zeus_expected/[>=1.1.1]", transitive_headers=True)

    def build_requirements(self):
        self.test_requires("standardprojectsettings/[>=0.2.0]")
        if not self.conf.get("tools.build:skip_test", False, check_type=bool):
            self.test_requires("catch2/[>=3.4.0]")

    def export(self):
        update_conandata(self, {"version": self.version})

    def export_sources(self):
        copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)
        copy(self, "*", os.path.join(self.recipe_folder, "src"), os.path.join(self.export_sources_folder, "src"))
        copy(self, "*", os.path.join(self.recipe_folder, "include"), os.path.join(self.export_sources_folder, "include"))
        copy(self, "*", os.path.join(self.recipe_folder, "tests"), os.path.join(self.export_sources_folder, "tests"))

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["ENABLE_APPS"] = self.options.with_apps
        tc.variables["EXTENSIVE_WARNINGS"] = self.options.enable_extensive_warnings
        tc.variables["CURA_FORMULA_VERSION_VERSION"] = f'{self.version}'
        tc.variables["ENABLE_TESTS"] = not self.conf.get("tools.build:skip_test", False, check_type=bool)

        if is_msvc(self):
            tc.variables["USE_MSVC_RUNTIME_LIBRARY_DLL"] = not is_msvc_static_runtime(self)

        tc.generate()

        tc = CMakeDeps(self)
        tc.generate()

        tc = VirtualBuildEnv(self)
        tc.generate()

        tc = VirtualRunEnv(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, pattern="LICENSE", dst=os.path.join(self.package_folder, "licenses"), src=self.source_folder)
        copy(self, pattern="*.h", src=os.path.join(self.source_folder, "include"), dst=os.path.join(self.package_folder, "include"))
        copy(self, pattern="*.a", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.so", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.lib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
        copy(self, pattern="*.dll", src=self.build_folder, dst=os.path.join(self.package_folder, "bin"), keep_path=False)
        copy(self, pattern="*.dylib", src=self.build_folder, dst=os.path.join(self.package_folder, "lib"), keep_path=False)
