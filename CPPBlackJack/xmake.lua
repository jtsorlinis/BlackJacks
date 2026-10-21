target("CPPBlackJack")
set_kind("binary")
add_files("src/*.cpp")
set_targetdir("./build/bin")

-- Optimization settings
set_optimize("fastest")
set_policy("build.optimization.lto", true)

target("CPPBlackJack-gcc")
set_kind("binary")
set_default(false)
add_files("src/*.cpp")
set_targetdir("./build/bin")
set_toolchains("gcc-16")

set_optimize("fastest")
set_policy("build.optimization.lto", true)
