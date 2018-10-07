#ifndef GTIP_TEST_PATTERSON_HPP
#define GTIP_TEST_PATTERSON_HPP

#include <gtest/gtest.h>

#include <gtip/gtip.hpp>
#include <gtip_api/gtip_api.h>

//------------------------------------------

TEST(TestDecoding, HandlesPatterson)
{
	try
	{
		// m * t = n - k
		const unsigned long k = 8;
		const unsigned long m = 4;
		const unsigned long t = 2;
		const unsigned long n = std::pow(2, m);

		ASSERT_EQ(m * t, n - k);

		gtip::context::init(k);
		gtip::goppa_code gc(m, t, n, true, false);

		NTL::vec_GF2 ciphertext;

		for (int i = 0; i < n; ++i)
		{
			ciphertext.append(NTL::GF2(1));
		}

		NTL::vec_GF2 error_vector;

		bool computed =
			gtip::decoding::patterson(
					ciphertext,
					gc,
					error_vector,
					false);

		if (computed == true)
		{
			ASSERT_EQ(error_vector.length(), ciphertext.length());
		}
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestDecoding, HandlesPattersonRobust)
{
	try
	{
		// m * t = n - k
		const unsigned long k = 8;
		const unsigned long m = 4;
		const unsigned long t = 2;
		const unsigned long n = std::pow(2, m);

		ASSERT_EQ(m * t, n - k);

		gtip::context::init(k);
		std::uniform_int_distribution<unsigned long> distribution(0, n);
		auto rng = gtip::context::get_random_engine();

		for (int i = 0; i < 1000; ++i)
		{
			gtip::goppa_code gc(m, t, n, true, false);

			NTL::vec_GF2 ciphertext;

			for (int k = 0; k < n; ++k)
			{
				ciphertext.append(NTL::GF2(distribution(rng)));
			}

			NTL::vec_GF2 error_vector;

			bool computed =
				gtip::decoding::patterson(
						ciphertext,
						gc,
						error_vector,
						false);

			if (computed == true)
			{
				ASSERT_EQ(error_vector.length(), ciphertext.length());
			}
		}
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestDecoding, HandlesIsDecodable)
{
	try
	{
		// m * t = n - k
		const unsigned long k = 8;
		const unsigned long m = 4;
		const unsigned long t = 2;
		const unsigned long n = std::pow(2, m);

		ASSERT_EQ(m * t, n - k);

		gtip::context::init(k);
		gtip::goppa_code gc(m, t, n, true, false);

		//TODO here
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

#endif
