#include <gtest/gtest.h>

#include "test_keygen.hpp"
#include "test_api.hpp"
#include "test_goppa_codes.hpp"
#include "test_keygen.hpp"
#include "test_keygen.hpp"
#include "test_sign_leader.hpp"
#include "test_sign_participant.hpp"
#include "test_patterson.hpp"

//==================================================================

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

//==================================================================
