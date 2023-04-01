#!/bin/bash

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

hyperfine --warmup 3 --export-markdown results.md \
"CBlackJack/build/bin/CBlackJack" \
"CPPBlackJack/build/bin/CPPBlackJack" \
"RustBlackJack/target/release/rust_black_jack" \
"dotnet CSharpBlackJack/bin/release/net7.0/CSharpBlackJack.dll" \
"GoBlackJack/GoBlackJack" \
"NimBlackJack/NimBlackJack" \
"node JSBlackJack/." \
"python3 PyBlackJack/main.py" 

exit 0