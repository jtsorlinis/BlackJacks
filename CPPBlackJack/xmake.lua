add_requires("mimalloc v3.3.2")

target("CPPBlackJack")
set_kind("binary")
add_files("src/*.cpp")
set_targetdir("./build/bin")
add_packages("mimalloc")

-- Optimization settings
set_optimize("fastest")
set_policy("build.optimization.lto", true)

target("CPPBlackJack-gcc")
set_kind("binary")
set_default(false)
add_files("src/*.cpp")
set_targetdir("./build/bin")
set_toolchains("gcc-16")
add_packages("mimalloc")
add_ldflags("-static-libstdc++", "-Wl,-no_exported_symbols")

set_optimize("fastest")
set_policy("build.optimization.lto", true)
