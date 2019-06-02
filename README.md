# ConcurrentMake

# Requirements

For using this utility you need to have next third-pary libraries
- Libssh
- Boost (System and Filesystem)

# Installing
```sh
mkdir build
cd build
cmake ..
make
```
# Using 
 ## Starting node on your computer
 TODO: write how to run daemon
 ## Compiling
 In your project create ccmake.txt file with next options
 ```
 EXE=<name of executable>
 SOURCE=<list of c/cpp files divided by whitespace>
 HEADERS=<list of h files divided by whitespace>
 FLAGS=<parameters for compiler>
```

Then to build project run 
```sh
concurrent_make <path to ccmake.txt>
```

# Contributors
Roman Milishchuk [@RomanMilishchuk](https://github.com/RomanMilishchuk)
Yurii Yelisiesiev [@YuraYelisiesiev](https://github.com/YuraYelisieiev)
Danylo Kolinko [@Kolinko-Danylo](https://github.com/Kolinko-Danylo)
