/*
 * assert.c
 *
 *  Created on: Mar 19, 2011
 *      Author: mateus
 */

#include "../include/traits.h"
#include "../include/abst_assert.h"
#include "../include/error.h"

#if LINUX
#include <iostream>

using namespace std;

#else
#include <utility/ostream.h>

static System::OStream cout;
#endif

void Assert(bool expression, const char context[], const char message[])
{
	if (! expression) {
		cout << "In " << context << ", assertion error: " << message << "!\n";
		Abort();
	}
}
