#include <iostream>
//#include "BigInteger.cpp"
#include "BigInteger.hpp"

int main()
{
  BigInteger integer({3, 0}, true);
  BigInteger one({1, 0}, true);
  integer = integer % one;
  std::cout << (integer).to_string();
  return 0;
}
