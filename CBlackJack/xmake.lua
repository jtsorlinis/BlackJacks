target("CBlackJack")
set_kind("binary")
add_files("src/*.c")
set_targetdir("./build/bin")

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

set_optimize("fastest")
set_policy("build.optimization.lto", true)
