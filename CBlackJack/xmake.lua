add_requires("mimalloc v3.3.2")

target("CBlackJack")
set_kind("binary")
add_files("src/*.c")
set_targetdir("./build/bin")
add_packages("mimalloc")
add_forceincludes("mimalloc-override.h")

-- Optimization settings
set_optimize("fastest")
set_policy("build.optimization.lto", true)

target("CBlackJack-gcc")
set_kind("binary")
set_default(false)
add_files("src/*.c")
set_targetdir("./build/bin")
set_toolchains("gcc-16")
set_toolset("ld", "gcc-16")
add_packages("mimalloc")
add_forceincludes("mimalloc-override.h")

set_optimize("fastest")
set_policy("build.optimization.lto", true)
