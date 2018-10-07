#ifndef GTIP_TEST_KEYGEN_HPP
#define GTIP_TEST_KEYGEN_HPP

#include <gtest/gtest.h>

#include <gtip/gtip.hpp>
#include <gtip_api/gtip_api.h>

//------------------------------------------

TEST(TestKeygen, HandlesKeygenConstructorInit)
{	
	try
	{
		// m * t = n - k
		const unsigned long k = 8;
		const unsigned long m = 4;
		const unsigned long t = 2;
		const unsigned long n = std::pow(2, m);

		gtip::gtip_parameters parameters;
		{
			parameters.n = n;
			parameters.t = t;
			parameters.k = k;
		}

		gtip::context::init(k);
		gtip::keygen keygen(parameters);

		gtip::keygen::private_key_t private_key;
		keygen.compute_private_key(private_key);

		gtip::keygen::public_key_t public_key;
		keygen.compute_public_key(public_key, private_key);
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

#endif
