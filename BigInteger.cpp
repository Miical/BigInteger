#include "biginteger.h"


BigInteger::BigInteger() {
	negative = false;
	num.push_back(0);
}
BigInteger::BigInteger(const std::string& s) {
	if (s[0] == '-') negative = true;
	else negative = false;
	
	int leading_zero = 0;
	for (auto it = s.begin() + negative; it != s.end(); it++) {
		if (*it == '0') leading_zero++;
		else break;
	}

	int len = s.length() - negative - leading_zero, n = (len - 1) / WIDTH + 1;
	for (int i = 0; i < n; i++) {
		int t = 0;
		for (int j = (i == n-1 ? len - (n - 1) * WIDTH - 1: 7); 
			j >= 0; j--) {
			t = t * 10 + 
				s[len - (j + i * WIDTH) -1 + negative + leading_zero] - '0';
		}
		num.push_back(t);
	}
}
BigInteger::BigInteger(long long s) {
	if (s == 0) {
		negative = false;
		num.push_back(0);
	}
	else {
		negative = (s < 0 ? true : false);
		if (negative) s = -s;
		while (s) {
			num.push_back(s % BASE);
			s /= BASE;
		}
	}
}


BigInteger BigInteger::Plus(const BigInteger& x)const {
	//Ignore negative
	BigInteger ret; 
	ret.num.clear();
	int n = std::max(num.size(), x.num.size());
	int carry = 0; 
	for (int i = 0; i < n; i++) {
		if (i < num.size()) carry += num[i];
		if (i < x.num.size()) carry += x.num[i];
		ret.num.push_back(carry % BASE);
		carry /= BASE;
	}
	if (carry) ret.num.push_back(carry);
	return ret;
}
BigInteger BigInteger::Minus(const BigInteger& x)const {
	//big num - small num
	//Ignore negative
	BigInteger ret; 
	ret.num.clear();
	int n = std::max(num.size(), x.num.size());
	int carry = 0;
	for (int i = 0; i < n; i++) {
		int t = carry + num[i];
		if (i < x.num.size()) t -= x.num[i];
		if (t < 0) {
			t += BASE;
			carry = -1;
		}
		else carry = 0;
		ret.num.push_back(t);	 
	}
	while (!ret.num.empty() && *(ret.num.end() - 1) == 0)
		ret.num.erase(ret.num.end() - 1);
	if (ret.num.empty()) ret.num.push_back(0);
	return ret;
}


BigInteger BigInteger::operator=(const std::string& s) {
	*this = BigInteger(s);
	return *this;
}
BigInteger BigInteger::operator=(long long x) {
	*this = BigInteger(x);
	return *this;
}

bool operator==(const BigInteger& a, const BigInteger& b) {
	if (a.num.size() == 1 && a.num[0] == 0 &&
		b.num.size() == 1 && b.num[0] == 0)
		return true;
	return a.num == b.num && a.negative == b.negative;
}
bool operator!=(const BigInteger& a, const BigInteger& b) {
	return !(a == b);
}
bool operator< (const BigInteger& a, const BigInteger& b) {
	if (a.negative && !b.negative) return true;
	if (!a.negative && b.negative) return false;
	int f = a.negative ? -1 : 1;

	if (a.num.size() != b.num.size())
		return f * a.num.size() < f * b.num.size();
	for (int i = a.num.size() - 1; i >= 0; i--)
		if (a.num[i] != b.num[i])
			return f * a.num[i] < f * b.num[i];
	return false;
}
bool operator> (const BigInteger& a, const BigInteger& b) {
	return !(a < b) && !(a == b);
}
bool operator<=(const BigInteger& a, const BigInteger& b) {
	return a < b || a == b;
}
bool operator>=(const BigInteger& a, const BigInteger& b) {
	return a > b || a == b;
}

BigInteger BigInteger::operator+() const{
	return *this;
}
BigInteger BigInteger::operator-() const{
	BigInteger ret = *this;
	ret.negative = !ret.negative;
	return ret;
}
BigInteger BigInteger::operator++() {
	return *this = *this + BigInteger(1);
}
BigInteger BigInteger::operator--() {
	return *this = *this - BigInteger(1);
}
BigInteger BigInteger::operator++(int) {
	return *this = *this + BigInteger(1);
}
BigInteger BigInteger::operator--(int) {
	return *this = *this - BigInteger(1);
}
BigInteger BigInteger::operator+=(const BigInteger& x) {
	*this = *this + x;
	return *this;
}
BigInteger BigInteger::operator-=(const BigInteger& x) {
	*this = *this - x;
	return *this;
}
BigInteger BigInteger::operator*=(const BigInteger& x) {
	*this = *this * x;
	return *this;
}
BigInteger BigInteger::operator/=(const BigInteger& x) {
	*this = *this / x;
	return *this;
}
BigInteger BigInteger::operator%=(const BigInteger& x) {
	*this = *this % x;
	return *this;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
	if (a.negative == b.negative) {
		BigInteger ret = a;
		ret = ret.Plus(b);
		ret.negative = a.negative;
		return ret;
	}
	else {
		BigInteger x = a, y = b;
		if (x.negative) std::swap(x, y); //x is positive, y is negative
		if (x >= -y) return x.Minus(y);
		else return -y.Minus(x);
	}
}
BigInteger operator-(const BigInteger& a, const BigInteger& b) {
	return a + (-b);
}
BigInteger operator*(const BigInteger& a, const BigInteger& b) {
	BigInteger ret; 
	if (a == 0 || b == 0) return BigInteger(0);
	for (int i = 0; i < a.num.size(); i++) {
		BigInteger t; t.num.clear();
		for (int j = 0; j < i; j++)
			t.num.push_back(0);
		long long carry = 0;
		for (int j = 0; j < b.num.size(); j++) {
			long long mul = 1LL * a.num[i] * b.num[j] + carry;
			t.num.push_back(int(mul % (1LL * BigInteger::BASE)));
			carry = mul / (1LL * BigInteger::BASE);
		}
		if (carry) t.num.push_back(int(carry));
		ret += t;
	}
	if (a.negative != b.negative)
		ret.negative = true;
	return ret;
}

void BigInteger::Divide2() {
	int carry = 0;
	if (num[num.size() - 1] == 1) {
		num.erase(--num.end());
		carry = 1;
	}
	for (int i = num.size() - 1; i >= 0; i--) {
		int t = carry * BASE + num[i];
		num[i] = t / 2;
		carry = t % 2;
	}
}
BigInteger operator/(const BigInteger& a, const BigInteger& b) {
	if (b == 0) return 1 / b.num[0];
	BigInteger x = a, y = b;
	x.negative = y.negative = false;
	if (x < y) return BigInteger(0);
	BigInteger Num = y, Bit_Num = y, ans = 1, bit_ans = 1;
	while (Num < x) {
		Num = Num + Num;
		Bit_Num = Bit_Num + Bit_Num;
		ans = ans + ans;
		bit_ans = bit_ans + bit_ans;
	}
	while (!bit_ans.num.empty()) {
		if (Num - Bit_Num >= x) {
			Num -= Bit_Num;
			ans -= bit_ans;
		}
		Bit_Num.Divide2();
		bit_ans.Divide2();
	}
	if (Num > x) ans--;
	if (a.negative != b.negative)
		ans.negative = true;
	return ans;
}
BigInteger operator%(const BigInteger& a, const BigInteger& b) {
	return a - a / b * b;
}

#include<iomanip>
std::string s;
std::istream& operator>>(std::istream& _cin, BigInteger& x) {
	_cin >> s;
	x = BigInteger(s);
	return _cin;
}
std::ostream& operator<<(std::ostream& _cout, const BigInteger& x) {
	if (x.negative && x != 0) _cout << '-';
	_cout << x.num[x.num.size()-1];x.summary();
	for (int i = x.num.size() - 2; i >= 0; i--)
		_cout << std::setw(BigInteger::WIDTH) << std::setfill('0')
			  << x.num[i];

	return _cout;
}

#include <iostream>
void BigInteger::summary()const {
	using namespace std;
	cout << endl << "===================================" << endl;
	cout << "BASE: " << BASE << endl;
	cout << "BIT_NUM: " << num.size() << endl;
	int count = 0, t = num[num.size() - 1];
	while (t) {
		count++;
		t /= 10;
	}
	cout << "BIT_NUM_IN_DEC: " << (num.size() - 1) * WIDTH + count << endl;
	cout << "IS_NEGATIVE: " << (negative ? "TRUE" : "FALSE") << endl;
	cout << "NUM = " << endl;
	
	count =0;
	for (int i = num.size() - 1; i >= 0; i--) {
		cout <<std::setw(BigInteger::WIDTH) << std::setfill('0') 
			 << num[i] << ' ';
		count++;
		if (count % 4 == 0 && count != 0) cout << endl;
	}
	cout << endl << "===================================" << endl;
}
