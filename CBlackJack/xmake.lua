target("CBlackJack")
set_kind("binary")
add_files("src/*.c")
set_targetdir("./build/bin")

-- Optimization settings
set_optimize("fastest")
set_policy("build.optimization.lto", true)