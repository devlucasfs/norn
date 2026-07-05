cd src

g++ -std=c++17 -g -O0 -fPIC -fpermissive -fexceptions main.cpp \
    -o ../build/norn \
    -I. \
    -L./libs/x86_64-linux \
    -leva \
    -Wl,-rpath,'$ORIGIN'
