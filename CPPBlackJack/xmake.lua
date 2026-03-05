target("CPPBlackJack")
set_kind("binary")
add_files("src/*.cpp")
set_targetdir("./build/bin")

-- Optimization settings
set_optimize("fastest")
set_policy("build.optimization.lto", true)