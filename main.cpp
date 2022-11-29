#include <iostream>
//#include "BigInteger.cpp"
#include "BigInteger.hpp"

int main()
{
  BigInteger integer({3, 0, 0, 0}, false);
  BigInteger one({1,0, 0,0,0,0,0}, true);
  std::cout << (integer * one).to_string() << "\n";
  return 0;
}
