#include <iostream>
#include <string>
#include <algorithm>
#include "BigInteger.hpp"

BigInteger::BigInteger() : digits(), sign(true){

}
BigInteger::BigInteger(long long other) : sign(other >= 0){
	while (other){
		digits.push_back(other % 10);
		other /= 10;
	}
	std::reverse(digits.begin(), digits.end());
}
BigInteger::BigInteger(const std::string& other) : sign(other[0] == '-'){
	std::for_each(other.begin(), other.end(), [&](char digit){digits.push_back(digit - '0');});
	std::reverse(digits.begin(), digits.end());
}

BigInteger::BigInteger(const BigInteger& other): digits(other.digits), sign(other.sign){

}

BigInteger::BigInteger(const std::vector<short>& other_digits, bool other_sign) : digits(other_digits), sign(other_sign){

}

BigInteger::~BigInteger(){

}

