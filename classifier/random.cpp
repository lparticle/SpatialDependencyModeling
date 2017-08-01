#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "random.h"


int UniformRandom(int l, int u) // [l, u)
{
	return (rand()%(u-l)+l);
}

void InitUnrepeatedIntRandom(UnrepeatedIntRandom *random, int l, int u, int cnt)
{
	assert(cnt<=u-l);

	random->lower_= l;
	random->upper_= u;
	random->count_= cnt;

	random->i_= 0;
	random->container_.resize(u-l);
	for(int i=0; i<u-l; ++i)
	{
		random->container_[i]= l+i;
	}
}

int Random(UnrepeatedIntRandom *random)
{
	assert(random->i_ < random->count_);
	int t= UniformRandom(random->i_, random->upper_-random->lower_);

	int tmp= random->container_[random->i_];
	random->container_[random->i_]= random->container_[t];
	random->container_[t]= tmp;

	++random->i_;

	return random->container_[random->i_-1];
}