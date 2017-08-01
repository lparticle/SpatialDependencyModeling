#if !defined RANDOM
#define RANDOM

#include <vector>

using namespace std;

struct UnrepeatedIntRandom
{
	int lower_; // [lower, upper)
	int upper_;
	int count_;

	int i_;
	vector<int> container_;
};

int UniformRandom(int l, int u); // [l, u)

void InitUnrepeatedIntRandom(UnrepeatedIntRandom *random, int l, int u, int cnt);

int Random(UnrepeatedIntRandom *random);

#endif
