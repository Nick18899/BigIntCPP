#include <iostream>
//#include "BigInteger.cpp"
#include "BigInteger.hpp"

int main()
{
  BigInteger integer({3, 0, 7, 0}, false);
  BigInteger one({1,0}, false);
  std::cout << (integer / one).to_string() << "\n";
  //std::cout << (BigInteger("442") + one).to_string();
  return 0;
}
