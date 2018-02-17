import qbs 1.0

Application
{
    Depends {name: "cpp"}
    destinationDirectory: "bin"

    cpp.cxxFlags: ["-std=c++11", "-Werror=return-type"]
    cpp.staticLibraries: ["gtest", "pthread"]

    files: [
        "test.cpp",
        "../lib/astrocoords/coords.h",
        "../lib/astrocoords/coords.cpp"
    ]

}
