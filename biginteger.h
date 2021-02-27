//Liu Jincheng 2021/02/25
#ifndef BIGINTEGER_H_
#define BIGINTEGER_H_

#include <vector>
#include <string>

#define BigInt BigInteger

class BigInteger {
	static const int BASE  = 100000000;
	static const int WIDTH = 8;
private:
	bool negative;
	std::vector<int> num;

	BigInteger Plus(const BigInteger&)const;
	BigInteger Minus(const BigInteger&)const;
	void Divide2();

public:
	BigInteger();
	BigInteger(const std::string&);
	BigInteger(long long);
	~BigInteger(){}

	BigInteger operator=(const std::string&);
	BigInteger operator=(long long);
	
	friend bool operator==(const BigInteger&, const BigInteger&);
	friend bool operator!=(const BigInteger&, const BigInteger&);
	friend bool operator< (const BigInteger&, const BigInteger&);
	friend bool operator> (const BigInteger&, const BigInteger&);
	friend bool operator<=(const BigInteger&, const BigInteger&);
	friend bool operator>=(const BigInteger&, const BigInteger&);

	BigInteger operator+ ()const;
	BigInteger operator- ()const;
	BigInteger operator++();
	BigInteger operator--();
	BigInteger operator++(int);
	BigInteger operator--(int);

	BigInteger operator+=(const BigInteger&);
	BigInteger operator-=(const BigInteger&);
	BigInteger operator*=(const BigInteger&);
	BigInteger operator/=(const BigInteger&);
	BigInteger operator%=(const BigInteger&);
	friend BigInteger operator+(const BigInteger&, const BigInteger&);
	friend BigInteger operator-(const BigInteger&, const BigInteger&);
	friend BigInteger operator*(const BigInteger&, const BigInteger&);
	friend BigInteger operator/(const BigInteger&, const BigInteger&);
	friend BigInteger operator%(const BigInteger&, const BigInteger&);

	friend std::istream& operator>>(std::istream&, BigInteger&);
	friend std::ostream& operator<<(std::ostream&, const BigInteger&);

	void summary()const;
};

#endif
