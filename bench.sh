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
cd ..

echo "Building GoBlackJack"
cd GoBlackJack
go build
cd ..

echo "Building NimBlackJack"
cd NimBlackJack
nimble build
cd ..

echo "Building RustBlackJack"
cd RustBlackJack
cargo build --release
cd ..

hyperfine --warmup 3 -L num_rounds $ROUNDS --export-markdown results.md \
"CBlackJack/build/bin/CBlackJack {num_rounds}" \
"CPPBlackJack/build/bin/CPPBlackJack {num_rounds}" \
"RustBlackJack/target/release/rust_black_jack {num_rounds}" \
"dotnet CSharpBlackJack/bin/release/net7.0/CSharpBlackJack.dll {num_rounds}" \
"GoBlackJack/GoBlackJack {num_rounds}" \
"NimBlackJack/NimBlackJack {num_rounds}" \
"node JSBlackJack/. {num_rounds}" \
"bun JSBlackJack/main.js {num_rounds}" \
"python3 PyBlackJack/main.py {num_rounds}" 
exit 0