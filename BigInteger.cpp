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

BigInteger BigInteger::operator/(const BigInteger &other) const {
  BigInteger cp(*this);
  cp /= other;
  return cp;
}

void BigInteger::operator/=(const BigInteger &other) {
  BigInteger cp(other);
  cp.sign = false;
  if (cp > *this) {
    *this =  BigInteger("0");
    return;
  }
  if (cp == BigInteger("0")) {
    *this = BigInteger("0");
    return;
  }
  bool sgn;
  if (this->sign != other.sign) {
    sgn = true;
  } else {
    sgn = false;
  }
  BigInteger left("0");
  this->sign = false;
  BigInteger right(*this);
  right += 1;
  while (right - left > BigInteger(1)) {
    BigInteger mid = left + right;
    division_by_two(mid);
    //std::cout << mid.to_string() << "strange" << std::endl;
    if (mid * cp == *this) {
      left = mid;
      break;
    }
    if (mid * cp > *this) {
      right = mid;
    } else {
      if (right - left == BigInteger("1")) {
        break;
      }
      //std::cout << ((right - left) == BigInteger("1")) << " " ;
      left = mid;
    }
  }
  //std::cout << left.to_string() << "    ";
  *this = left;
  this->sign = sgn;
}

BigInteger BigInteger::operator%(const BigInteger &other) const {
  if (other.sign) {
    if (this->sign) {
      BigInteger res = -(*this) + (*this / other) * (other);
      if (res == BigInteger("0")) {
        return BigInteger("0");
      }
      return -other - res;
    } else {
      BigInteger res = (*this) - (*this / other) * (other);
      if (res == BigInteger("0")) {
        return BigInteger("0");
      }
      return res;
    }
  }
  BigInteger res = *this - (*this / other) * (other);
  if (res == BigInteger("0")) {
    return BigInteger("0");
  }
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

BigInteger::operator double() const {
  size_t num = 0;
  for (size_t i = this->digits.size() - 1; i + 1 >= 1; --i) {
    num += this->digits[this->digits.size() - i - 1];
  }
  return num;
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
      *this = BigInteger("0");
      return;
    }
    if (!(this->sign)) {
      if (other.size() > this->size() || *this < -other) {
        BigInteger cp(*this);
        *this = other;
        substract_second_from_first(*this, cp);
        this->sign = true;
      }
      else {
        substract_second_from_first(*this, other);
      }
    }
    else {
      if (other.size() > this->size() || (*this) > -other) {
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
    /*for (int i = 0; i < first.size(); ++i) {
      std::cout << first[i] << " ";
    }*/
    //std::cout << std::endl;
    for (int i=first.size() - 1; i>=0; --i)
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
    if (!sign)
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
    buff = 0;
    if (first.digits[i] < 0){
      //buff = first.digits[i] < 0 ? 1 : 0;
      buff = 1;
      first.digits[i] += 10;
      flag = true;
    }
    if (second.size() < first.size() && (i == second.size() - 1) && flag) {
      ++i;
      if (first.digits[i] != 0) {
        --first.digits[i];
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

void BigInteger::division_by_two(BigInteger &first) {
  if (first == BigInteger("0")) {
    return;
  }
  for (int i = 0; i < first.size(); ++i) {
    if (first.digits[i] % 2 == 0) {
      first.digits[i] /= 2;
    }
    else {
      if (i != 0) {
        first.digits[i - 1] += 5;
        first.digits[i] /= 2;
      }
      else {
        first.digits[i] /= 2;
      }
    }
  }
  size_t i = first.digits.size() - 1;
  while (first.digits[i] == 0) {
    if (first.digits.size() == 1) {
      break;
    }
    first.digits.pop_back();
    --i;
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
BigInteger operator "" _bi(const char* s, size_t size) {
  std::string s1 = s;
  return BigInteger(s1);
}

BigInteger BigInteger::BinaryPow (BigInteger number, size_t n) {
  BigInteger result = BigInteger("1");
  while(n) {
    if (n & 1) {
      result *= number;
      --n;
    } else {
      number *= number;
      n >>= 1;
    }
  }
  return result;
}
// check what is wrong with this shit

class Rational {
  Rational(const BigInteger& bi) {
    numerator = bi;
    denominator = BigInteger("1");
  }

  Rational(int num) {
    numerator = BigInteger(num);
    denominator = BigInteger("1");
  }

  Rational(const BigInteger& bi, const BigInteger& bi2) {
    numerator = bi;
    denominator = bi2;
  }

  Rational(int num, int num2) {
    numerator = BigInteger(num);
    denominator = BigInteger(num2);
  }

  Rational() = default;

  std::string toString() {
    if (denominator == BigInteger("1")) {
      return numerator.to_string();
    } else {
      return numerator.to_string() + "/" + denominator.to_string();
    }
  }

  std::string asDecimal(size_t precision = 0) const {
    std::string result = (numerator / denominator).to_string();
    BigInteger ost = (numerator % denominator) * (BigInteger::BinaryPow(BigInteger("10"), precision));
    ost /= denominator;
    result += ("." + ost.to_string());
    return result;
  }

  Rational operator - () const {
    return -1 * *this;
  }

  Rational operator + () const {
    return *this;
  }

  Rational& operator += (const Rational& a) {
    *this = *this + a;
    return *this;
  }

  Rational& operator -= (const Rational& a) {
    *this = *this - a;
    return *this;
  }

  Rational& operator *= (const Rational& a) {
    *this = *this * a;
    return *this;
  }

  Rational& operator /= (const Rational& a) {
    *this = *this / a;
    return *this;
  }

  friend Rational operator + (const Rational& a, const Rational&b) {
    return Rational(a.numerator * b.denominator + b.numerator * a.denominator, a.denominator*b.denominator);
  }

  friend Rational operator - (const Rational& a, const Rational&b) {
    return Rational(a.numerator * b.denominator - b.numerator * a.denominator, a.denominator * b.denominator);
  }

  friend Rational operator * (const Rational& a, const Rational&b) {
    return Rational(a.numerator * b.numerator, a.denominator * b.denominator);
  }

  friend Rational operator / (const Rational& a, const Rational&b) {
    return Rational(a.numerator * b.denominator, a.denominator*b.numerator);
  }

  friend bool operator < (const Rational& a, const Rational& b) {
    Rational tmp = a / b;
    return tmp.numerator < tmp.denominator;
  }

  friend bool operator > (const Rational& a, const Rational& b) {
    return b < a;
  }

  friend bool operator >= (const Rational& a, const Rational& b) {
    return !(a < b);
  }

  friend bool operator <= (const Rational& a, const Rational& b) {
    return !(a > b);
  }

  friend bool operator == (const Rational& a, const Rational& b) {
    return !(a < b) && !(a > b);
  }

  friend bool operator != (const Rational& a, const Rational& b) {
    return !(a == b);
  }

  explicit operator double() const {
    return std::stod(this->asDecimal(16));
  }

 private:
  BigInteger numerator;
  BigInteger denominator;
};
