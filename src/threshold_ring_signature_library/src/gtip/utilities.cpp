#include "utilities.hpp"

using namespace gtip;

//------------------------------------------

bool
utilities::extended_euclidean_algorithm(
		const NTL::GF2EX& t,
		const NTL::GF2EX& g,
		NTL::GF2EX& T,
		NTL::GF2EX& d)
{
	NTL::GF2EX a;
	// d = gcd(t, g), t * T + g * a = d
	NTL::XGCD(d, T, a, t, g);
	return true;
}

//------------------------------------------

bool
utilities::extended_euclidean_algorithm(
		const NTL::GF2EX& S,
		const NTL::GF2EX& G,
		NTL::GF2EX& T)
{
	NTL::GF2EX d;
	NTL::GF2EX a;
	// d = gcd(S, G), S * T + G * a = d
	NTL::XGCD(d, T, a, S, G);
	return true;
}

//------------------------------------------
