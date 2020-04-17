#include <iostream>
#include <functional>
#include "func.h"

#include <x86intrin.h>
// optional wrapper if you don't want to just use __rdtsc() everywhere
inline uint64_t rdtsc() {
  _mm_lfence();  // optionally wait for earlier insns to retire before reading the clock
  uint64_t tsc = __rdtsc();
  _mm_lfence();  // optionally block later instructions until rdtsc retires
  return tsc;
}

// requires a Nehalem or newer CPU.  Not Core2 or earlier.  IDK when AMD added it.
inline uint64_t rdtscp() {
  unsigned dummy;
  return __rdtscp(&dummy);  // waits for earlier insns to retire, but allows later to start
}

Data3Base* MakeData3() {
  Data3Base* ret = nullptr;
  srand(time(NULL));
  if (rand() % 2 == 0) {
    ret = new Data3();
  } else {
    ret = new Data3_2();
  }
  return ret;
}

uint64_t Global(int LOOP) {
  sum = 0;
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    Global();
  uint64_t end = rdtsc();
  return sum + end - start;
}
uint64_t Direct(int LOOP) {
  Data d;
  d.sum = 0;
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    Plus(&d);
  uint64_t end = rdtsc();
  return d.sum + end - start;
}
uint64_t FuncPointer(int LOOP) {
  Data d;
  d.sum = 0;
  auto plus = Plus;
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    plus(&d);
  uint64_t end = rdtsc();
  return d.sum + end - start;
}

uint64_t BindDirect(int LOOP) {
  Data d;
  d.sum = 0;
  // auto plus = std::bind(Plus, &d);
  auto plus = std::bind(Plus, &d);
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    plus();
  uint64_t end = rdtsc();
  return d.sum + end - start;
}

uint64_t BindDirectStdFunction(int LOOP) {
  Data d;
  d.sum = 0;
  std::function<void()> plus = std::bind(Plus, &d);
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    plus();
  uint64_t end = rdtsc();
  return d.sum + end - start;
}

uint64_t Member(int LOOP) {
  Data2 d2;
  d2.sum = 0;
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    d2.Plus();
  uint64_t end = rdtsc();
  return d2.sum + end - start;
}

uint64_t BindMember(int LOOP) {
  Data2 d2;
  d2.sum = 0;
  auto plus2 = std::bind(&Data2::Plus, &d2);
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    plus2();
  uint64_t end = rdtsc();
  return d2.sum + end - start;
}

uint64_t BindMemberStdFunction(int LOOP) {
  Data2 d2;
  d2.sum = 0;
  std::function<void()> plus2 = std::bind(&Data2::Plus, &d2);
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    plus2();
  uint64_t end = rdtsc();
  return d2.sum + end - start;
}

uint64_t Virtual(int LOOP) {
  Data3Base* d3 = MakeData3();
  d3->sum = 0;
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    d3->Plus();
  uint64_t end = rdtsc();
  return d3->sum + end - start;
}

uint64_t BindVirtual(int LOOP) {
  Data3Base* d3 = MakeData3();
  d3->sum = 0;
  auto plus3 = std::bind(&Data3Base::Plus, d3);
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    plus3();
  uint64_t end = rdtsc();
  return d3->sum + end - start;
}

uint64_t Virtual2(int LOOP) {
  Data3Base* d3 = MakeData3();
  d3->sum = 0;
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    d3->Plus();
  uint64_t end = rdtsc();
  return d3->sum + end - start;
}

uint64_t BindVirtual2(int LOOP) {
  Data3Base* d3 = MakeData3();
  d3->sum = 0;
  auto plus3 = std::bind(&Data3Base::Plus, d3);
  uint64_t start = rdtsc();
  for (int i = 0; i < LOOP; ++i)
    plus3();
  uint64_t end = rdtsc();
  return d3->sum + end - start;
}

int main(int argc, char* argv[]) {
  int LOOP = 99999999;
  uint64_t t0 = Global(LOOP);
  std::cerr << "GLOBAL\t\t\t" << t0 << std::endl;
  uint64_t t1 = Direct(LOOP);
  std::cerr << "DIRECT\t\t\t" << t1 << std::endl;
  uint64_t t2 = FuncPointer(LOOP);
  std::cerr << "FUNCTION POINTER\t" << t2 << std::endl;
  uint64_t t3 = BindDirect(LOOP);
  std::cerr << "BIND DIRECT\t\t" << t3 << std::endl;
  uint64_t t3_2 = BindDirectStdFunction(LOOP);
  std::cerr << "BIND DIRECT FUNC\t" << t3_2 << std::endl;
  uint64_t t4 = Member(LOOP);
  std::cerr << "MEMBER FUNCTION\t\t" << t4 << std::endl;
  uint64_t t5 = BindMember(LOOP);
  std::cerr << "BIND MEMBER\t\t" << t5 << std::endl;
  uint64_t t5_2 = BindMemberStdFunction(LOOP);
  std::cerr << "BIND MEMBER FUNC\t" << t5_2 << std::endl;
  uint64_t t6 = Virtual(LOOP);
  std::cerr << "VIRTUAL\t\t\t" << t6 << std::endl;
  uint64_t t7 = BindVirtual(LOOP);
  std::cerr << "BIND VIRTUAL\t\t" << t7 << std::endl;
  uint64_t t8 = Virtual2(LOOP);
  std::cerr << "VIRTUAL2\t\t" << t8 << std::endl;
  uint64_t t9 = BindVirtual2(LOOP);
  std::cerr << "BIND VIRTUAL2\t\t" << t9 << std::endl;
}
