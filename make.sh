/usr/local/bin/g++ -c func.cc -O3 -std=c++11
ar rvs func.a func.o
/usr/local/bin/g++ func_call_benchmark.cc func.a -o a -O3 -std=c++11
