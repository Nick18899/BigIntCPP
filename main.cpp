#include <iostream>
#include "BigInteger.cpp"
int main( int argc, const char* argv[] )
{
  BigInteger integer({1,2,3}, 1);
  std::cout << integer.to_string() << "\n";
  return 0;
}
