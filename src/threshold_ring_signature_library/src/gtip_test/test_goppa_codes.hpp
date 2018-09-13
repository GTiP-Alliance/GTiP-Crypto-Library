#ifndef GTIP_TEST_GOPPA_CODES_HPP
#define GTIP_TEST_GOPPA_CODES_HPP

#include <gtest/gtest.h>

#include <gtip/context.hpp>
#include <gtip/goppa_code.hpp>

//------------------------------------------

TEST(TestGoppaCodes, HandlesConstructor4x2)
{
	try
	{
		const unsigned long m = 4;
		const unsigned long t = 2;

		gtip::context::init(m);
		gtip::goppa_code gc(m, t, std::pow(2, m), true, false);
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
		const unsigned long m = 4;
		const unsigned long t = 2;

		for (int i = 0; i < 10000; ++i)
		{
			gtip::context::init(m);
			gtip::goppa_code gc(m, t, std::pow(2, m), true, false);
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
		const unsigned long m = 10;
		const unsigned long t = 2;

		gtip::context::init(m);
		gtip::goppa_code gc(m, t, std::pow(2, m), true, false);
	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

#endif
