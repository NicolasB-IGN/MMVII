from setuptools import setup, Command
from setuptools.command import sdist
from setuptools.command.build_ext import build_ext
from setuptools.extension import Extension

import os
import shutil
import re
import subprocess
import sys
import xml.sax
import textwrap

from collections import deque
from pathlib import Path
from glob import glob

DOCSTRING_WIDTH = 70
DOCSTRING_HEADER = """/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif

"""
DOCSTRING_FOOTER = """
#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif
"""
DOCSTRING_FILES = ["xml/class*xml", "xml/struct*xml", "xml/namespaceMMVII.xml"]


class DoxygenHandler(xml.sax.ContentHandler):
    """
    @author: cmeynard
    """

    def __init__(self):
        self.allDesc = {}
        self.operators = {
            "<=": "le",
            ">=": "ge",
            "==": "eq",
            "!=": "ne",
            "[]": "array",
            "+=": "iadd",
            "-=": "isub",
            "*=": "imul",
            "/=": "idiv",
            "%=": "imod",
            "&=": "iand",
            "|=": "ior",
            "^=": "ixor",
            "<<=": "ilshift",
            ">>=": "irshift",
            "++": "inc",
            "--": "dec",
            "<<": "lshift",
            ">>": "rshift",
            "&&": "land",
            "||": "lor",
            "!": "lnot",
            "~": "bnot",
            "&": "band",
            "|": "bor",
            "^": "bxor",
            "+": "add",
            "-": "sub",
            "*": "mul",
            "/": "div",
            "%": "mod",
            "<": "lt",
            ">": "gt",
            "=": "assign",
            "()": "call",
        }

    def cleanName(self, name):
        for symb, alias in self.operators.items():
            name = name.replace(f"operator{symb}", f"operator_{alias}")
        name = re.sub("<.*>", "", name)
        name = "".join([ch if ch.isalnum() else "_" for ch in name])
        name = re.sub("_$", "", re.sub("_+", "_", name))
        return name

    def cleanDesc(self, desc):
        wrapper = textwrap.TextWrapper()
        wrapper.expand_tabs = True
        wrapper.replace_whitespace = True
        wrapper.drop_whitespace = True
        wrapper.width = DOCSTRING_WIDTH
        wrapper.initial_indent = wrapper.subsequent_indent = ""

        result = ""
        in_code_segment = False
        for x in re.split(r"(```)", desc):
            if x == "```":
                if not in_code_segment:
                    result += "\n"
                else:
                    result += "\n\n"
                in_code_segment = not in_code_segment
            elif in_code_segment:
                result += x.strip()
            else:
                for y in re.split(r"(?: *\n *){2,}", x):
                    wrapped = wrapper.fill(re.sub(r"\s+", " ", y).strip())
                    if len(wrapped) > 0 and wrapped[0] == "$":
                        result += wrapped[1:] + "\n"
                        wrapper.initial_indent = wrapper.subsequent_indent = " " * 4
                    else:
                        if len(wrapped) > 0:
                            result += wrapped + "\n\n"
                        wrapper.initial_indent = wrapper.subsequent_indent = ""
        return result.rstrip().lstrip("\n")

    def addDesc(self, name):
        if "detail" in self.desc and self.desc["detail"] != "":
            desc = self.desc["detail"]
        elif "brief" in self.desc and self.desc["brief"] != "":
            desc = self.desc["brief"]
        else:
            desc = ""
        desc = self.cleanDesc(desc)
        if "\n" in desc:
            desc = "\n" + desc
        name = self.cleanName(name)
        if name in self.allDesc:
            self.allDesc[name].append(desc)
        else:
            self.allDesc[name] = [desc]

    def startDocument(self):
        self.inDefinition = False
        self.inClass = False
        self.inMember = False
        self.inPara = False
        self.element = None
        self.elements = deque()
        self.desc = {}
        self.descType = None
        self.className = ""
        self.memberName = ""
        self.definition = ""
        self.indent = 0

    def compounddef_start(self, attributes):
        if attributes["kind"] in ("struct", "class", "namespace"):
            self.inClass = True
            self.className = ""

    def compounddef_end(self):
        if self.inClass:
            self.addDesc(self.className)
        self.inClass = False
        self.className = ""

    def memberdef_start(self, attributes):
        if attributes["kind"] in ("function", "variable"):
            self.inMember = True
            self.definition = ""
            self.memberName = ""

    def memberdef_end(self):
        if not self.inMember:
            return
        if "~" not in self.definition:
            self.addDesc(self.className + "_" + self.memberName)
        self.inMember = False
        self.definition = ""
        self.memberName = ""

    def definition_start(self, _):
        if not self.inMember:
            return
        self.inDefinition = True
        self.definition = ""

    def definition_end(self):
        if not self.inMember:
            return
        self.inDefinition = False

    def briefdescription_start(self, _):
        if not self.inClass and not self.inMember:
            return
        self.descType = "brief"
        self.desc[self.descType] = ""

    def briefdescription_end(self):
        self.descType = None

    def detaileddescription_start(self, _):
        if not self.inClass and not self.inMember:
            return
        self.descType = "detail"
        self.desc[self.descType] = ""

    def detaileddescription_end(self):
        self.descType = None

    def para_start(self, _):
        self.inPara = True

    def para_end(self):
        self.inPara = False
        if self.descType is None:
            return
        self.desc[self.descType] += "\n\n"

    def verbatim_start(self, _):
        if self.descType is None:
            return
        self.desc[self.descType] += "```"

    def verbatim_end(self):
        if self.descType is None:
            return
        self.desc[self.descType] += "```"

    def itemizedlist_start(self, _):
        if self.descType is None:
            return
        self.indent += 4

    def itemizedlist_end(self):
        if self.descType is None:
            return
        self.indent -= 4

    def listitem_start(self, _):
        if self.descType is None:
            return
        self.desc[self.descType] += " " * self.indent + "- "

    def characters(self, content):
        if self.inPara and self.descType is not None:
            self.desc[self.descType] += content
            return
        if self.inClass and self.element == "compoundname":
            self.className += content
            return
        if self.inMember and self.element == "name":
            self.memberName += content
            return
        if self.inDefinition:
            self.definition += content
            return

    def startElement(self, tag, attributes):
        self.elements.append(self.element)
        self.element = tag
        func = getattr(DoxygenHandler, tag + "_start", None)
        if func:
            func(self, attributes)

    def endElement(self, tag):
        self.element = self.elements.pop()
        func = getattr(DoxygenHandler, tag + "_end", None)
        if func:
            func(self)

    def outputDesc(self):
        result = []
        for name in sorted(self.allDesc):
            descs = self.allDesc[name]
            for count, desc in enumerate(descs):
                if count == 0:
                    result.append(
                        f'static const char *__doc_{name} = R"doc({desc})doc";\n'
                    )
                else:
                    result.append(
                        f'static const char *__doc_{name}_{count + 1} = R"doc({desc})doc";\n'
                    )
        return "\n".join(result)


# from https://github.com/pybind/cmake_example

# Convert distutils Windows platform specifiers to CMake -A arguments
PLAT_TO_CMAKE = {
    "win32": "Win32",
    "win-amd64": "x64",
    "win-arm32": "ARM",
    "win-arm64": "ARM64",
}


# A CMakeExtension needs a sourcedir instead of a file list.
# The name must be the _single_ output extension from the CMake build.
# If you need multiple extensions, see scikit-build.
class CMakeExtension(Extension):
    def __init__(self, name: str, sourcedir: str = "") -> None:
        root = os.fspath(Path(sourcedir).resolve())

        super().__init__(name, sources=[])
        self.sourcedir = root


class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        # Must be in this form due to bug in .resolve() only fixed in Python 3.10+
        ext_fullpath = Path.cwd() / self.get_ext_fullpath(ext.name)
        extdir = ext_fullpath.parent.resolve()

        # Using this requires trailing slash for auto-detection & inclusion of
        # auxiliary "native" libs

        debug = int(os.environ.get("DEBUG", 0)) if self.debug is None else self.debug
        cfg = "Debug" if debug else "Release"

        # CMake lets you override the generator - we need to check this.
        # Can be set with Conda-Build, for example.
        cmake_generator = os.environ.get("CMAKE_GENERATOR", "")

        # Set Python_EXECUTABLE instead if you use PYBIND11_FINDPYTHON
        # EXAMPLE_VERSION_INFO shows you how to pass a value into the C++ code
        # from Python.
        cmake_args = [
            f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY={extdir}{os.sep}",
            f"-DPYTHON_EXECUTABLE={sys.executable}",
            f"-DCMAKE_BUILD_TYPE={cfg}",  # not used on MSVC, but no harm
        ]
        build_args = []
        # Adding CMake arguments set as environment variable
        # (needed e.g. to build for ARM OSx on conda-forge)
        if "CMAKE_ARGS" in os.environ:
            cmake_args += [item for item in os.environ["CMAKE_ARGS"].split(" ") if item]

        # In this example, we pass in the version to C++. You might not need to.
        cmake_args += [f"-DEXAMPLE_VERSION_INFO={self.distribution.get_version()}"]

        if self.compiler.compiler_type != "msvc":
            # Using Ninja-build since it a) is available as a wheel and b)
            # multithreads automatically. MSVC would require all variables be
            # exported for Ninja to pick it up, which is a little tricky to do.
            # Users can override the generator with CMAKE_GENERATOR in CMake
            # 3.15+.
            if not cmake_generator or cmake_generator == "Ninja":
                try:
                    import ninja

                    ninja_executable_path = Path(ninja.BIN_DIR) / "ninja"
                    cmake_args += [
                        "-GNinja",
                        f"-DCMAKE_MAKE_PROGRAM:FILEPATH={ninja_executable_path}",
                    ]
                except ImportError:
                    pass

        else:
            # Single config generators are handled "normally"
            single_config = any(x in cmake_generator for x in {"NMake", "Ninja"})

            # CMake allows an arch-in-generator style for backward compatibility
            contains_arch = any(x in cmake_generator for x in {"ARM", "Win64"})

            # Specify the arch if using MSVC generator, but only if it doesn't
            # contain a backward-compatibility arch spec already in the
            # generator name.
            if not single_config and not contains_arch:
                cmake_args += ["-A", PLAT_TO_CMAKE[self.plat_name]]

            # Multi-config generators have a different way to specify configs
            if not single_config:
                cmake_args += [
                    f"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{cfg.upper()}={extdir}"
                ]
                build_args += ["--config", cfg]

        if sys.platform.startswith("darwin"):
            # Cross-compile support for macOS - respect ARCHFLAGS if set
            archs = re.findall(r"-arch (\S+)", os.environ.get("ARCHFLAGS", ""))
            if archs:
                cmake_args += ["-DCMAKE_OSX_ARCHITECTURES={}".format(";".join(archs))]

        # Set CMAKE_BUILD_PARALLEL_LEVEL to control the parallel build level
        # across all generators.
        if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
            # self.parallel is a Python 3 only way to set parallel jobs by hand
            # using -j in the build_ext call, not supported by pip or PyPA-build.
            if hasattr(self, "parallel") and self.parallel:
                # CMake 3.12+ only.
                build_args += [f"-j{self.parallel}"]

        build_temp = Path(self.build_temp) / ext.name
        if not build_temp.exists():
            build_temp.mkdir(parents=True)

        subprocess.run(
            ["cmake", ext.sourcedir + "/src/MMVII", *cmake_args],
            cwd=build_temp,
            check=True,
        )
        subprocess.run(
            ["cmake", "--build", ".", *build_args], cwd=build_temp, check=True
        )

        build_lib = Path(self.build_lib) / ext.name.replace("_", "")

        shutil.copytree(build_temp / "bin", build_lib / "bin", dirs_exist_ok=True)


class Clean(Command):
    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        python_mmvii_root = os.path.dirname(__file__) + "/src/MMVII"
        self._rm(python_mmvii_root + "/sources")
        self._rm(python_mmvii_root + "/build")
        self._rm(python_mmvii_root + "/xml")
        self._rm(python_mmvii_root + "/bin")
        self._rm(python_mmvii_root + "/MMVII-LocalParameters")

    def _rm(self, dir: str):
        if not self.dry_run:
            shutil.rmtree(dir, ignore_errors=True)


class ExtraSdist(Command):
    def initialize_options(self):
        pass

    def finalize_options(self):
        pass

    def run(self):
        self.run_command("sdist_clean")
        python_mmvii_root = os.path.dirname(__file__) + "/src/MMVII"
        project_root = os.path.dirname(os.path.dirname(__file__))
        sources_dir = python_mmvii_root + "/sources"

        whitelist = {
            "include",
            "src",
            "ExternalInclude",
            "CMakeLists.txt",
            "vcpkg.json",
        }

        def _ignore(src: str, names: list[str]) -> list[str]:
            if project_root == src:
                return [name for name in names if name not in whitelist]

            return []

        print("Copying MMVII sources")

        copytree_extra_kwargs = (
            {"copy_function": lambda a, b: print("DRY", a, "->", b)}
            if self.dry_run
            else {}
        )

        shutil.copytree(
            project_root,
            sources_dir,
            dirs_exist_ok=True,
            ignore=_ignore,
            **copytree_extra_kwargs,
        )

        print("Copying MMVII-LocalParameters")

        shutil.copytree(
            project_root + "/MMVII-LocalParameters",
            python_mmvii_root + "/MMVII-LocalParameters",
            dirs_exist_ok=True,
            **copytree_extra_kwargs,
        )

        print("Generating docstrings")

        docstrings = python_mmvii_root + "/docstrings.h"

        if not self.dry_run:
            shutil.rmtree(python_mmvii_root + "/xml", ignore_errors=True)
            subprocess.run(["doxygen"], cwd=python_mmvii_root)
            self._docstring(python_mmvii_root, docstrings)

    def _docstring(self, root: str, filename: str):
        parser = xml.sax.make_parser()
        parser.setFeature(xml.sax.handler.feature_namespaces, 0)
        doc_handler = DoxygenHandler()
        parser.setContentHandler(doc_handler)
        for file in DOCSTRING_FILES:
            for match in glob(root + "/" + file):
                parser.parse(match)

        with open(filename, "w") as f:
            print(DOCSTRING_HEADER, file=f)
            print(doc_handler.outputDesc(), file=f)
            print(DOCSTRING_FOOTER, file=f)


class BaseSdist(sdist.sdist):
    def run(self):
        self.run_command("extra_sdist")
        return super().run()


setup(
    name="MMVII",
    version="0.0.3",
    package_dir={"": "src"},
    author="IGN",
    url="https://github.com/micmac-V2/MMVII",
    description="MicMac v2 Python API",
    python_requires=">=3.12",
    cmdclass={
        "sdist": BaseSdist,
        "extra_sdist": ExtraSdist,
        "sdist_clean": Clean,
        "build_ext": CMakeBuild,
    },
    package_data={
        "MMVII": [
            "bin/*",
            "MMVII-LocalParameters/*/*",
            "sources/**/*",
            "*.cpp",
            "*.h",
            "CMakeLists.txt",
        ]
    },
    ext_modules=[CMakeExtension("_MMVII")],
)
