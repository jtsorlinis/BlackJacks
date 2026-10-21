#!/bin/bash

ROUNDS='1000000'

echo "Building CBlackJack"
cd CBlackJack
xmake
xmake build CBlackJack-gcc
cd ..

echo "Building CPPBlackJack"
cd CPPBlackJack
xmake
xmake build CPPBlackJack-gcc
cd ..

echo "Building CSharpBlackJack"
cd CSharpBlackJack
dotnet build -c release
dotnet publish
cd ..

echo "Building GoBlackJack"
cd GoBlackJack
go build
cd ..

echo "Building RustBlackJack"
cd RustBlackJack
cargo build --release
cd ..

hyperfine --warmup 3 -L num_rounds $ROUNDS --export-json results.json \
-n C "CBlackJack/build/bin/CBlackJack {num_rounds}" \
-n "C (GCC)" "CBlackJack/build/bin/CBlackJack-gcc {num_rounds}" \
-n C++ "CPPBlackJack/build/bin/CPPBlackJack {num_rounds}" \
-n "C++ (GCC)" "CPPBlackJack/build/bin/CPPBlackJack-gcc {num_rounds}" \
-n Rust "RustBlackJack/target/release/rust_black_jack {num_rounds}" \
-n C# "dotnet CSharpBlackJack/bin/Release/net10.0/CSharpBlackJack.dll {num_rounds}" \
-n "C# (AOT)" "CSharpBlackJack/bin/Release/net10.0/osx-arm64/native/CSharpBlackJack {num_rounds}" \
-n Go "GoBlackJack/GoBlackJack {num_rounds}" \
-n Node "node JSBlackJack/. {num_rounds}" \
-n Bun "bun JSBlackJack/main.js {num_rounds}" \
-n PyPy "pypy3 PyBlackJack/main.py {num_rounds}" \
-n Python "python3 PyBlackJack/main.py {num_rounds}" \
|| exit 1

# hyperfine 2.0 dropped --sort, so build the results table from the JSON export, fastest first
jq -r '.results | sort_by(.summary.time_wall_clock.mean)[]
  | [.name, (.summary.time_wall_clock | .mean, .stddev), .summary.memory_peak_resident.mean]
  | @tsv' results.json |
awk -F'\t' '
BEGIN {
  printf "| %-10s | %17s | %8s | %12s |\n", "Language", "Wall time [ms]", "Relative", "Memory [MiB]"
  print "|:-----------|------------------:|---------:|-------------:|"
}
NR == 1 { best = $2 }
{ printf "| %-10s | %8.1f ± %6.1f | %7.2fx | %12.1f |\n", $1, $2 * 1000, $3 * 1000, $2 / best, $4 / 1048576 }
' > results.md

echo
cat results.md
exit 0