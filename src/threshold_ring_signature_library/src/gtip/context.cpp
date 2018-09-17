#include <map>
#include <cmath>
#include <string>
#include <iostream>
#include <glog/logging.h>

#include <NTL/GF2E.h>
#include <NTL/GF2EX.h>
#include <NTL/mat_GF2.h>
#include <NTL/mat_GF2E.h>
#include <NTL/GF2XFactoring.h>
#include <NTL/GF2EXFactoring.h>

#include "context.hpp"
#include "utilities.hpp"

//------------------------------------------

using namespace NTL;
using namespace gtip;

//------------------------------------------

NTL::GF2X context::k;
bool context::initialized = false;
std::mt19937 context::random_engine;
std::random_device context::random_device;

//------------------------------------------

void
context::init(
		const NTL::GF2X& k,
		const unsigned long m)
{
	NTL::GF2E::init(k);

	// check if k can generate support set L
	if (has_support_set(m) == false)
	{
		LOG(ERROR) << "Failed to initialze";
		std::runtime_error("Failed to initialize");
	}

	context::k = k;
	context::initialized = true;
	context::random_engine = std::mt19937(context::random_device());
}

//------------------------------------------

void
context::init(
		const unsigned long m)
{
	// we choose an irreducible polynomial k(X) of degree m
	NTL::GF2X k = NTL::BuildIrred_GF2X(m);
	context::init(k, m);
}

//------------------------------------------

bool
context::is_initialized()
{
	return context::initialized;
}

//------------------------------------------

const NTL::GF2X&
context::get_k()
{
	return context::k;
}

std::mt19937&
context::get_random_engine()
{
	return context::random_engine;
}

//------------------------------------------

bool
context::has_support_set(
		const unsigned long m)
{
	// TODO, theo says irred polynomials always have support set
	return true;
}

//------------------------------------------
