#pragma once
#ifndef GTIP_TEST_API_HPP
#define GTIP_TEST_API_HPP

#include <gtest/gtest.h>

//------------------------------------------

TEST(TestAPI, HandlesAPI)
{
	try
	{

	}
	catch(const std::exception& exp)
	{
		FAIL();
	}
}

//------------------------------------------

#endif
