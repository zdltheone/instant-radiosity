#include "randomNumber.h"

#include <iostream>

RandomNumberGenerator::RandomNumberGenerator() 
	: m_maxNum( 0 )
{
}

RandomNumberGenerator::RandomNumberGenerator( const RandomNumberGenerator& otherInstance )
	: m_maxNum( 0 )
{
}

RandomNumberGenerator::~RandomNumberGenerator()
{
}

void RandomNumberGenerator::GeneratePrimeList( unsigned int upperBound )
{
	unsigned int i;

	// list starts from 1 in the array.
	int* flag = new int[ 1 + upperBound ];
	for( int i = 0; i < 1 + upperBound; i++ )
	{
		flag[ i ] = 1;
	}

	// Element all even number whose value in range[ 1 upperbound ]
	for( unsigned int i = 2; i < upperBound + 1; i += 2 )
	{
		flag[ i ] = 0;
	}
	flag[ 2 ] = 1;

	// Element odd numbers' mulriple
	for( unsigned int i = 3; i < upperBound / 2; i++ )
	{
		// This number has been marked
		if( flag[ i ] == 0 )
		{
			continue;
		}

		for( unsigned int j = i + i; j < upperBound + 1; j += i )
		{
			flag[ j ] = 0;
		}
	}

	// Generate prime number list based on the info storing in flag
	for( unsigned int i = 2; i < upperBound + 1; i++ )
	{
		if( flag[ i ] == 1 )
		{
			m_primeArray.push_back( i );
			//std::cout << i << std::endl;
		}
	}

	delete[] flag;
}

double RandomNumberGenerator::GetPseudoRandomNum()
{
	return rand() / (double)(RAND_MAX);
}

double RandomNumberGenerator::PhiBDirected( int base, int ithNum )
{
	double x = 0.0;
	double f = 1.0 / base;

	while( ithNum )
	{
		x += f * (double) ( ithNum % base );
		ithNum *= f;
		f *= f;
	}

	return x;
}

double RandomNumberGenerator::PhiBIncremental( int base, double x )
{
	double h, hh, r = 1.0 - x - 1e-10;
	
	if( 1.0 / base < r )
	{
		x += 1.0 / base;
	}
	else
	{
		h = 1.0 / base;

		do{
			hh = h;
			h *= 1.0 / base;
		}while( h >= r );

		x += hh + h -1.0;
	}

	return x;
}