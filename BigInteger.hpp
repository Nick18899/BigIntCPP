#include <vector>
#include <string>
class BigInteger
{
	// digits storage
	std::vector<short> digits;
	// if number is negative, sign is true, otherwise sign is false
	bool sign;

public:
	// Constructors
	BigInteger ();
	BigInteger(long long other);
	BigInteger(const std::string& other);
	BigInteger(const BigInteger& other);
	BigInteger(const std::vector<short>& other_digits, bool other_sign);
	const BigInteger& operator =(const BigInteger& other);

	//Destructor
	~BigInteger();

	// Arithmetics
	void operator += (const BigInteger& other);
	void operator -= (const BigInteger& other);
	void operator *= (const BigInteger& other);
	void operator /= (const BigInteger& other);
	
	const BigInteger operator + (const BigInteger& other);
	const BigInteger operator - (const BigInteger& other);
	const BigInteger operator * (const BigInteger& other);
	const BigInteger operator / (const BigInteger& other);

	// Compare operators
	bool operator <  (const BigInteger& other);
	bool operator >  (const BigInteger& other);
	bool operator == (const BigInteger& other);
	bool operator >= (const BigInteger& other);
	bool operator <= (const BigInteger& other);

	// Utilities
	std::string to_string() const;
};
