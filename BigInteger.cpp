#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>
#include "BigInteger.hpp"

BigInteger::BigInteger() : digits(), sign(false){

}
BigInteger::BigInteger(long long other) : sign(other < 0){
	if (sign) other *= -1;
	while (other){
		digits.push_back(other % 10);
		other /= 10;
	}
	std::reverse(digits.begin(), digits.end());
}
BigInteger::BigInteger(const std::string& other) : sign(other[0] == '-'){
	std::for_each(other.begin() + sign, other.end(), [&](char digit){digits.push_back(digit - '0');});
}

BigInteger::BigInteger(const BigInteger& other): digits(other.digits), sign(other.sign){

}

BigInteger::BigInteger(const std::vector<short>& other_digits, bool other_sign) : digits(other_digits), sign(other_sign){

}

BigInteger::~BigInteger(){

}


std::string BigInteger::to_string() const 
{
	std::ostringstream result;
	if (sign){
		result << '-';
	}
	if(!digits.empty())
	{
		std::copy(digits.begin(), digits.end() - 1, std::ostream_iterator<short>(result, ""));
	}
	result << digits.back();
	return result.str();
}

BigInteger& BigInteger::operator++()
{
  *this += 1;
  return *this;
}
BigInteger& BigInteger::operator--()
{
  *this -= 1;
  return *this;
}
BigInteger BigInteger::operator++(int)
{
  BigInteger previous(*this);
  *this += 1;
  return previous;
}
BigInteger BigInteger::operator--(int)
{
  BigInteger previous(*this);
  *this -= 1;
  return previous;
}
void BigInteger::operator+=(const BigInteger& other)
{

}

static BigInteger* sum()
