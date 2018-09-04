#include <map>
#include <string>
#include <iostream>

#include "qredo_ring.h"

using namespace std;
using namespace qredo_ring_api;

//------------------------------------------

extern "C" int
qredo_ring_keygen(
        unsigned char* public_key,
        unsigned char* secret_key,
		const qredo_parameters* parameters)
{
	return 0;
}

//------------------------------------------

extern "C" int
qredo_ring_participant_sign(
        unsigned char* signature,
        const qredo_parameters* parameters,
        const unsigned char* message,
        const unsigned int message_len,
        const unsigned char* secret_key)
{
	return 0;
}

//------------------------------------------

extern "C" int
qredo_ring_leader_sign(
        unsigned char* signature,
        const qredo_parameters* parameters,
        const unsigned char* message,
        const unsigned int message_len,
        const unsigned char* participant_signatures)
{
	return 0;
}

//------------------------------------------

extern "C" int
qredo_ring_verify(
		int* result,
        const qredo_parameters* parameters,
        const unsigned char* message,
        const unsigned int message_len,
        const unsigned char* ring_signature,
        const unsigned char* ring_public_key)
{
	return 0;
}

//------------------------------------------
