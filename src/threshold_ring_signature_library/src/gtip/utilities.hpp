#ifndef GTIP_UTILITIES_HPP
#define GTIP_UTILITIES_HPP

#include <map>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>

// NTL includes
#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>

namespace gtip
{
	namespace utilities
	{
		//------------------------------------------

		template<typename T>
		std::string
		print_coefficients(const T& p)
		{
			std::stringstream ss;

			for (int i = 0; i <= NTL::deg(p); ++i)
			{
				if (NTL::IsZero(NTL::coeff(p, i)) == false)
				{
					ss << "X^" << i;

					if (i != NTL::deg(p))
					{
						ss << " + ";
					}
				}
			}

			return ss.str();
		}

		//------------------------------------------

		bool
		extended_euclidean_algorithm(
				const NTL::GF2EX& p0,
				const NTL::GF2EX& p1,
				NTL::GF2EX& inv);

		//------------------------------------------

		bool
		extended_euclidean_algorithm(
				const NTL::GF2EX& S,
				const NTL::GF2EX& G,
				NTL::GF2EX& T,
				NTL::GF2EX& d);


		//------------------------------------------
	}
}

#endif
