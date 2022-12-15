#include <iostream>
//#include "BigInteger.cpp"
#include "BigInteger.hpp"

int main()
{
  BigInteger integer({3, 0, 7, 0}, false);
  BigInteger one({1,2}, false);
  integer = integer % one;
  //std::cout << '\n' << (BigInteger("280") -  BigInteger("279") == 1) << "\n";
  std::cout << (integer).to_string();
  return 0;
}
