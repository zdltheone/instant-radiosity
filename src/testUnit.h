#include <iostream>
#include "randomNumber.h"
#include "instantRadiosity.h"
#include <ctime>

using namespace std;

void simpleTest()
{
	srand( (unsigned)time(0) );

	AllocConsole();
	freopen("conin$","r",stdin);
	freopen("conout$","w",stdout);
	freopen("conout$","w",stderr);

//	RandomNumberGenerator rng;
//	//rng.GeneratePrimeList( 200 );
//	for( int i = 0; i < 10; i++ )
//	{
//		//cout << rng.GetPseudoRandomNum() << endl;
//		cout << rng.PhiBDirected( 3, i ) << endl;
//	}

}