#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>
#include "BigInteger.hpp"

/*field:
    * std::vector<short> digits; 
    * bool sign;
 */

BigInteger::BigInteger() : digits({0}), sign(false)
{ 

}

BigInteger::BigInteger(long long other) : sign(other < 0)
{
	if (sign) other *= -1;
	while (other)
  {
		digits.push_back(other % 10);
		other /= 10;
	}
}

BigInteger::BigInteger(const std::string& other) : sign(other[0] == '-')
{
	std::for_each(other.begin() + (sign ? 1 : 0), other.end(), [&](char digit) //lambda foreach is used here
  {
    digits.push_back(digit - '0');
  });
  std::reverse(digits.begin(), digits.end());
}

BigInteger::BigInteger(const BigInteger& other): digits(other.digits), sign(other.sign)
{

}

BigInteger::BigInteger(const std::vector<short>& other_digits, bool other_sign) : digits(other_digits), sign(other_sign)
{
  std::reverse(digits.begin(), digits.end());
}

BigInteger::~BigInteger()
{

}

const std::string BigInteger::to_string() const 
{
	std::ostringstream result;
	if(!digits.empty())
	{
		std::copy(digits.begin(), digits.end() - 1, std::ostream_iterator<short>(result, ""));
	}

	result << digits.back();
  if(sign) result << '-';
  std::string result_string = result.str();
  std::reverse(result_string.begin(), result_string.end());
  return result_string;
}

const std::size_t BigInteger::size() const
{
  return digits.size();
}

void BigInteger::operator+=(const BigInteger& other)
{
  if (this->sign == other.sign)
  {
    if(other.size() > this->size())
    {
      BigInteger cp(*this);
      *this = other;
      add_second_to_first(*this, cp);
    }
    else
    {
      add_second_to_first(*this, other);
    }   
  } 
  else
  {
    
  }
}

const BigInteger& BigInteger::operator=(const BigInteger& other)
{
  digits = other.digits;
  sign = other.sign;
}

bool BigInteger::compare_two_numbers(const std::vector<short>& first, const std::vector<short>& second)
{
  if (first.size()<second.size())
  {
    return true;
  }
  else if (first.size()>second.size())
  {
    return false;
  }
  else 
  {
    for (int i=0; i<first.size(); ++i)
    {
      if (first[i] != second[i])
      {
        return (first[i] < second[i]);
      } 
    }
    return false;
  }
}

const bool BigInteger::operator>= (const BigInteger& other) const
{
  return !(*this >other); 
}



const bool BigInteger::operator== (const BigInteger& other) const
{
  if (!(other < *this)&&!(other > *this))
  {
    return true;
  }
  else
  {
    return false;
  }
}

const bool BigInteger::operator> (const BigInteger& other) const
{
  return (other < *this);
}
bool BigInteger::compare_signs(const BigInteger& first, const BigInteger& second)
{
  if (first.sign < second.sign)
  {
    return true;
  }
  else if (first.sign > second.sign)
  {
   return false; 
  }
}
const bool BigInteger::operator<(const BigInteger& other) const //false == 1; true == -1
{
  if (sign != other.sign)
  {
    return compare_signs(*this, other);
  }
  else
  {
    if (sign==false)
    {
      return compare_two_numbers(digits, other.digits);
    }
    else 
    {
      return !compare_two_numbers(digits, other.digits);
    }
  }
}

BigInteger& BigInteger::operator++()
{
  *this += 1;
  return *this;
}

void BigInteger::add_second_to_first(BigInteger& first, const BigInteger& second) //first < second
{
  short buff = 0;
  for (int i = 0;  i < second.size(); ++i)
  {
    first.digits[i] += second.digits[i] + buff;
    buff = first.digits[i] / 10;
    first.digits[i] = first.digits[i] % 10;
  }
  if (buff)
  {
    if (first.size() == second.size())
    {
      first.digits.push_back(0);
    }
    ++first.digits[second.size()];
  }
}

void BigInteger::substract_second_from_first(BigInteger& first, const BigInteger& second)
{
  short buff = 0;
  for(int i = 0; i < second.size(); ++i)
  {
    first.digits[i] -= second.digits[i] + buff;
    if (first.digits[i] < 0){
      buff = first.digits[i] < 0 ? 1 : 0;
      first.digits[i] += 10;
    }
  }
  while (!first.digits.back())
  {
    first.digits.pop_back();
  }
}
/* 
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
*/
