#!/bin/bash

ROUNDS='1000000'

echo "Building CBlackJack"
cd CBlackjack
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ../..

echo "Building CPPBlackJack"
cd CPPBlackjack
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ../..

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

hyperfine --warmup 3 -L num_rounds $ROUNDS --export-markdown results.md --sort mean-time \
-n C "CBlackJack/build/bin/CBlackJack {num_rounds}" \
-n C++ "CPPBlackJack/build/bin/CPPBlackJack {num_rounds}" \
-n Rust "RustBlackJack/target/release/rust_black_jack {num_rounds}" \
-n C# "dotnet CSharpBlackJack/bin/Release/net10.0/CSharpBlackJack.dll {num_rounds}" \
-n "C# (AOT)" "CSharpBlackJack/bin/Release/net10.0/osx-arm64/native/CSharpBlackJack {num_rounds}" \
-n Go "GoBlackJack/GoBlackJack {num_rounds}" \
-n Node "node JSBlackJack/. {num_rounds}" \
-n Bun "bun JSBlackJack/main.js {num_rounds}" \
-n PyPy "pypy3 PyBlackJack/main.py {num_rounds}" \
-n Python "python3 PyBlackJack/main.py {num_rounds}"
exit 0