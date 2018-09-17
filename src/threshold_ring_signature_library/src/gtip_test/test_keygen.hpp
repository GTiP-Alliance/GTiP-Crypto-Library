#ifndef GTIP_TEST_KEYGEN_HPP
#define GTIP_TEST_KEYGEN_HPP

#include <gtest/gtest.h>

#include <random>
#include <algorithm>

#include <gtip/keygen.hpp>
#include <gtip_api/gtip_api.h>

//------------------------------------------

TEST(TestKeygen, HandlesKeygenConstructorInit)
{	
	try
	{
		gtip::gtip_parameters parameters;
		{
			parameters.n = 3;
			parameters.w = 2;
			parameters.k = 2;
		}

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
