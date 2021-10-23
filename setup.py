from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import sys
import setuptools
import tempfile
import os


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked."""

    def __str__(self):
        import pybind11

        return pybind11.get_include()


def get_source_files(source_directory):
    source_files = []

    for dirpath, _, filenames in os.walk(source_directory):
        for filename in [f for f in filenames if f.endswith(".cpp")]:
            source_files.append(os.path.join(dirpath, filename))

    return source_files


ext_modules = [
    Extension(
        "circularbuffer",
        # Sort input source files to ensure bit-for-bit reproducible builds
        sorted(get_source_files("src")),
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
        ],
        language="c++",
    ),
]


# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
    """Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", delete=False) as f:
        f.write("int main (int argc, char **argv) { return 0; }")
        fname = f.name
    try:
        compiler.compile([fname], extra_postargs=[flagname])
    except setuptools.distutils.errors.CompileError:
        return False
    finally:
        try:
            os.remove(fname)
        except OSError:
            pass
    return True


def cpp_flag(compiler):
    """Return the -std=c++[17/20] compiler flag.
    The newer version is prefered over c++17 (when it is available).
    """
    flags = ["-std=c++20", "-std=c++2a", "-std=c++17"]

    for flag in flags:
        if has_flag(compiler, flag):
            return flag

    raise RuntimeError("Unsupported compiler -- at least C++17 support is needed!")


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""

    c_opts = {
        "msvc": ["/EHsc", "-O3"],
        "unix": ["-O3"],
    }
    l_opts = {
        "msvc": [],
        "unix": [],
    }

    if sys.platform == "darwin":
        darwin_opts = ["-stdlib=libc++", "-mmacosx-version-min=10.7"]
        c_opts["unix"] += darwin_opts
        l_opts["unix"] += darwin_opts

    def build_extensions(self):
        self.compiler.set_executable("compiler_so", "g++")
        self.compiler.set_executable("compiler_cxx", "g++")
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        link_opts = self.l_opts.get(ct, [])
        additional_opts = ["-fPIC", "-fvisibility=hidden"]
        if ct == "unix":
            opts.append(cpp_flag(self.compiler))
            for opt in additional_opts:
                if has_flag(self.compiler, opt):
                    opts.append(opt)

        for ext in self.extensions:
            macros = [
                ("VERSION_INFO", '"{}"'.format(self.distribution.get_version()))
            ]

            ext.define_macros = macros
            ext.extra_compile_args = opts
            ext.extra_link_args = link_opts
        build_ext.build_extensions(self)

setup(
    name="circularbuffer",
    version="0.2.0",
    description="Circular Buffer",
    long_description="",
    ext_modules=ext_modules,
    setup_requires=["pybind11>=2.6.0"],
    cmdclass={"build_ext": BuildExt},
    zip_safe=False,
)

