benchmark

MacOS на arm


```
mkdir build
cd build
cmake -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ ..
make
./gen ../file search_optimal_parameters
```