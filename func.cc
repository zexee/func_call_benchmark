#include "func.h"
#include <random>

void Plus(Data* d) {
  ++d->sum;
}

void Data2::Plus() {
  ++sum;
}

void Data3Base::Plus() {
  sum += 2;
}

void Data3Base2::Plus() {
  sum += 3;
}

void Data3::Plus() {
  ++sum;
}

void Data3_2::Plus() {
  ++sum;
}

int sum = 0;
void Global() {
  ++sum;
}

