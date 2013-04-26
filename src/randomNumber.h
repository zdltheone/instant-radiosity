#pragma once

#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

class RandomNumberGenerator
{
public:
	RandomNumberGenerator();
	RandomNumberGenerator( const RandomNumberGenerator& otherInstance );
	~RandomNumberGenerator();

public:
	void GeneratePrimeList( unsigned int upperBound );
	double GetPseudoRandomNum();
	// Halton sequence
	double PhiBDirected( int base, int ithNum );
	double PhiBIncremental( int base, double x );

	unsigned int GetithPrimeNumber( int ith );

private:
	vector<unsigned int> m_primeArray;
	// Seach prime numbers that belong to [1, m_maxNum]
	unsigned int         m_maxNum;
};