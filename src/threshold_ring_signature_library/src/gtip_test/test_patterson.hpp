#ifndef GTIP_TEST_PATTERSON_HPP
#define GTIP_TEST_PATTERSON_HPP

#include <gtest/gtest.h>

#include <gtip/context.hpp>
#include <gtip/patterson.hpp>
#include <gtip/goppa_code.hpp>

//------------------------------------------

TEST(TestDecoding, HandlesPatterson)
{
	try
	{
		const unsigned long m = 4;
		const unsigned long t = 2;
		const unsigned long n = std::pow(2, m);

		gtip::context::init(m);
		gtip::goppa_code gc(m, t, n, true, false);

		NTL::vec_GF2 ciphertext;

		for (int i = 0; i < n; ++i)
		{
			ciphertext.append(NTL::GF2(1));
		}

		NTL::vec_GF2 error_vector =
			gtip::decoding::patterson(
					ciphertext,
					gc,
					false);

		ASSERT_EQ(error_vector.length(), ciphertext.length());
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

#endif
