#include <vector>
#include <string>
class BigInteger
{
	// digits storage in reversed order
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
	
    BigInteger operator + (const BigInteger& other) const;
    BigInteger operator - (const BigInteger& other) const;
    BigInteger operator * (const BigInteger& other) const;
    BigInteger operator / (const BigInteger& other) const;
    BigInteger operator % (const BigInteger& other) const;

  // Prefix increment and decrement
	BigInteger& operator ++();
	BigInteger& operator --();
	// Postfix increment and decrement
	const BigInteger operator ++(int);
	const BigInteger operator --(int);
    // unar -
    BigInteger operator -() const;

	// Compare operators
	const bool operator <  (const BigInteger& other) const;
	const bool operator >  (const BigInteger& other) const;
	const bool operator == (const BigInteger& other) const;
	bool operator >= (const BigInteger& other) const;
	bool operator <= (const BigInteger& other) const;
    explicit operator bool() const;
    explicit operator double () const;

    // Utilities
	std::string to_string() const;
	std::size_t size() const;
private:
	//Service methods
 	static void add_second_to_first(BigInteger& first, const BigInteger& second);
 	static void substract_second_from_first(BigInteger& first, const BigInteger& second);
	static bool compare_two_numbers(const std::vector<short>& first, const std::vector<short>& second);
	static bool compare_signs(const BigInteger& first, const BigInteger& second);
};

