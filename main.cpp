#include <iostream>
//#include "BigInteger.cpp"
#include "BigInteger.hpp"

int main()
{
  BigInteger integer({1, 2, 5}, false);
  BigInteger one({1, 0, 0}, false);
  std::cout << (integer * one).to_string() << "\n";
  return 0;
}
