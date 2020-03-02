To compile with clang, for fastest speed

On Mac(OSX):
```
clang++ -O3 -std=c++17 -flto *.cpp
```

On Windows if using clang:
```
clang++ -O3 -flto -fuse-ld=lld *.cpp
```
