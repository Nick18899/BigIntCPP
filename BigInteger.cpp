#pragma once
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include "complex"
typedef std::complex<double> base;
#include "BigInteger.hpp"

/*field:
    * std::vector<short> digits; 
    * bool sign;
 */

BigInteger::BigInteger() : digits({0}), sign(false)
{ 

}

BigInteger::BigInteger(long long other) : sign(other < 0) // rewrite for int
{
	if (sign) other *= -1;
	while (other)
  {
		digits.push_back(other % 10);
		other /= 10;
	}
}

BigInteger BigInteger::operator-() const {
  BigInteger cp (*this);
  cp.sign = !(cp.sign);
  return cp;
}

BigInteger::BigInteger(const std::string& other) : sign(other[0] == '-')
{
	std::for_each(other.begin() + (sign ? 1 : 0), other.end(), [&](char digit) //lambda foreach is used here
  {
    digits.push_back(static_cast<short>(digit) - '0');
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

std::string BigInteger::to_string() const
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

void FourierFastTransform (std::vector<base>& vec, bool inverted) { // required check out
  int n = static_cast<int>(vec.size());
  if (n == 1) {
    return;
  }
  std::vector<base> vec1(n/2);
  std::vector<base> vec2(n/2);
  for (int i = 0, j = 0; i < n; i += 2, ++j) {
    vec1[j] = vec[i];
    vec2[j] = vec[i + 1];
  }
  FourierFastTransform(vec1, inverted);
  FourierFastTransform(vec2, inverted);
  double ang = 2*M_PI/n * (inverted ? -1 : 1);
  base w(1);
  base wn(std::cos(ang), std::sin(ang));
  for (int  i = 0; i < n/2; ++i) {
    vec[i] = vec1[i] + w * vec2[i];
    vec[i + n/2] = vec1[i] - w * vec2[i];
    if (inverted) {
      vec[i] /= 2;
      vec[i + n/2] /= 2;
    }
    w *= wn;
  }
}

void multiply (const std::vector<short> & vec1, const std::vector<short>& vec2, std::vector<short>& result) {
  //reverse(vec1.begin(), vec1.end());
  //reverse(vec2.begin(), vec2.end());
  std::vector<base> four_vec1 (vec1.begin(), vec1.end());
  std::vector<base> four_vec2 (vec2.begin(), vec2.end());
  size_t n = 1;
  while (n < std::max (vec1.size(), vec2.size())) {
    n <<= 1;
  }
  n <<= 1;
  four_vec1.resize(n);
  four_vec2.resize(n);
  FourierFastTransform(four_vec1, false);
  FourierFastTransform(four_vec2, false);
  for (size_t i = 0; i < n; ++i) {
    four_vec1[i] *= four_vec2[i];
  }
  FourierFastTransform(four_vec1, true);
  result.resize(n);
  for (size_t i = 0; i < n; ++i) {
    result[i] = int(four_vec1[i].real() + 0.5);
  }
  int c = 0;
  for (size_t i = 0; i < n; ++i) {
    result[i] += c;
    c = result[i]  /10;
    result[i] %=10;
  }
  size_t i = result.size() - 1;
  while (result[i] == 0) {
    if (result.size() == 1) {
      break;
    }
    result.pop_back();
    --i;
  }
  /*for (int i =0; i < result.size(); ++i) {
    std::cout << result[i] << " ";
  }*/
}

std::size_t BigInteger::size() const
{
  return digits.size();
}

void BigInteger::operator*=(const BigInteger &other) {
  std::vector<short> result;
  multiply(this->digits, other.digits, result);
  this->digits = result;
  if (this->sign != other.sign) {
    this->sign = true;
  }
  else {
    this->sign = false;
  }
}

BigInteger BigInteger::operator*(const BigInteger &other) const {
  BigInteger cp (*this);
  cp *= other;
  return cp;
}

BigInteger BigInteger::operator/(const BigInteger &other) const { // дописать
  /*if (other == BigInteger(0)) {
    return BigInteger(0);
  }
  if (this->sign && !other.sign) {
    return BigInteger(-1)*(*this / (BigInteger(-1)*other));
  }
  if (!this->sign && !other.sign) {
    return (BigInteger(-1)*(*this)) / (BigInteger(-1)*other);
  }
  if (other > *this) {
    return BigInteger("0");
  }*/
  return BigInteger("0");
}

BigInteger BigInteger::operator%(const BigInteger &other) const {
  BigInteger res = *this - (*this / other) * (*this);
  if (res.sign) {
    res += other;
  }
  return res;
}

BigInteger BigInteger::operator+(const BigInteger &other) const {
  BigInteger cp(*this);
  cp += other;
  return cp;
}

BigInteger BigInteger::operator-(const BigInteger &other) const {
  BigInteger cp(*this);
  cp -= other;
  return cp;
}

BigInteger::operator bool() const {
  return !(*this == BigInteger("0"));
}

void BigInteger::operator-=(const BigInteger &other) {
  BigInteger cp(other);
  cp.sign = !(other.sign);
  *this += cp;
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
    if (*this == -other) {
      *this = BigInteger(0);
      return;
    }
    if (!(this->sign)) {
      if (other.size() > this->size() || *this < -other) {
        BigInteger cp(*this);
        *this = other;
        for (int i = 0; i< cp.digits.size(); ++i) {
          std::cout << cp.digits[i] << " ";
        }
        std::cout << "\n";
        for (int i = 0; i< this->digits.size(); ++i) {
          std::cout << this->digits[i] << " ";
        }
        std::cout << std::endl;
        substract_second_from_first(*this, cp);
        this->sign = true;
      }
      else {
        substract_second_from_first(*this, other);
      }
    }
    else {
      if (other.size() > this->size() || (*this) < -other) {
        BigInteger cp(*this);
        *this = other;
        substract_second_from_first(*this, cp);
        this->sign = false;
      }
      else {
        substract_second_from_first(*this, other);
      }
    }
  }
}

const BigInteger& BigInteger::operator=(const BigInteger& other)
{
  this->digits = other.digits;
  this->sign = other.sign;
  return *this;
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

bool BigInteger::operator>= (const BigInteger& other) const //maybe cringe
{
  return !(*this >other); 
}

bool BigInteger::operator<=(const BigInteger &other) const { // also
  return !(*this < other);
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
  return true;
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
  bool flag = false;
  for(int i = 0; i < second.size(); ++i)
  {
    flag = false;
    first.digits[i] -= (second.digits[i] + buff);
    if (first.digits[i] < 0){
      buff = first.digits[i] < 0 ? 1 : 0;
      first.digits[i] += 10;
      flag = true;
    }
    if (second.size() < first.size() && (i == second.size() - 1) && flag) {
      ++i;
      if (first.digits[i] != 0) {
        --first.digits[i];
        std::cout << first.digits[i] << '\n';
      }
      else {
        int j = i;
        while (first.digits[j] == 0) {
          ++j;
        }
        for (int k = i; k < j; ++k) {
          first.digits[k] = 9;
        }
        --first.digits[j];
        //std::cout << first.digits[j] << std::endl;
      }
    }
  }
  while (!first.digits.back())
  {
    first.digits.pop_back();
  }
}

BigInteger& BigInteger::operator--()
{
  *this -= 1;
  return *this;
}
const BigInteger BigInteger::operator++(int)
{
  BigInteger previous(*this);
  *this += 1;
  return previous;
}
const BigInteger BigInteger::operator--(int)
{
  BigInteger previous(*this);
  *this -= 1;
  return previous;
}
// check what is wrong with this shit
