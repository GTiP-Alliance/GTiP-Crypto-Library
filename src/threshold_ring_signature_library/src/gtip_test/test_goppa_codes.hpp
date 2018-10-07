#ifndef GTIP_TEST_GOPPA_CODES_HPP
#define GTIP_TEST_GOPPA_CODES_HPP

#include <gtest/gtest.h>

#include <gtip/gtip.hpp>
#include <gtip_api/gtip_api.h>

//------------------------------------------

TEST(TestGoppaCodes, HandlesConstructor4x2)
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

		// TEST H_a
		ASSERT_EQ(gc.H_a().NumRows(), t);
		ASSERT_EQ(gc.H_a().NumCols(), n);

		// TEST H_b
		ASSERT_EQ(gc.H_b().NumRows(), m  * t);
		ASSERT_EQ(gc.H_b().NumCols(), n);
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestGoppaCodes, HandlesConstructorStressTest4x2)
{
	try
	{
		for (int i = 0; i < 10000; ++i)
		{
			// m * t = n - k
			const unsigned long k = 8;
			const unsigned long m = 4;
			const unsigned long t = 2;
			const unsigned long n = std::pow(2, m);

			ASSERT_EQ(m * t, n - k);

			gtip::context::init(k);
			gtip::goppa_code gc(m, t, n, true, false);

			// TEST H_a
			ASSERT_EQ(gc.H_a().NumRows(), t);
			ASSERT_EQ(gc.H_a().NumCols(), n);

			// TEST H_b
			ASSERT_EQ(gc.H_b().NumRows(), m  * t);
			ASSERT_EQ(gc.H_b().NumCols(), n);
		}
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

TEST(TestGoppaCodes, HandlesConstructor10x2)
{
	try
	{
		for (int i = 0; i < 100; ++i)
		{
			// m * t = n - k
			const unsigned long k = 1004;
			const unsigned long m = 10;
			const unsigned long t = 2;
			const unsigned long n = std::pow(2, m);

			ASSERT_EQ(m * t, n - k);

			gtip::context::init(k);
			gtip::goppa_code gc(m, t, n, true, false);

			// TEST H_a
			ASSERT_EQ(gc.H_a().NumRows(), t);
			ASSERT_EQ(gc.H_a().NumCols(), n);

			// TEST H_b
			ASSERT_EQ(gc.H_b().NumRows(), m  * t);
			ASSERT_EQ(gc.H_b().NumCols(), n);
		}
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

#endif
