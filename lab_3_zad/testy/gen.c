#include <stdio.h>
#include "ary.h"
#include "stdbool.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

bool _ISZERO(double x) { return fabs(x) < 1e-10; }

int rnd(int lo, int hi)
{
	return rand() % (hi - lo + 1) + lo;
}

double rndf(int lo, int hi)
{
	return (rand() % (hi - lo + 1) + lo) / 1.0;
}

double val(double x)
{
	if (isnan(x))
		return NAN;
	if (_ISZERO(x))
		return 0.0;
	return x;
}

int minv = -2;
int maxv = +2;

int __T = 10;
bool __DEBUG = false;

wartosc gen(int depth)
{
	if (depth == 0) {
		double x, y;
		x = rndf(minv, maxv);
		y = rndf(0.0, 1000.0);
		if (__DEBUG) printf("wartosc_dokladnosc(%.11f, %.11f)", x, y);
		return wartosc_dokladnosc(x, y);
	}
	int n = rnd(1, 4);
	wartosc res, l, r;
	switch (n) {
	case 1:
		if (__DEBUG) printf("plus(");
		l = gen(depth - 1);
		if (__DEBUG) printf(", ");
		r = gen(depth - 1);
		if (__DEBUG) printf(")");
		res = plus(l, r);
		break;
	case 2:
		if (__DEBUG) printf("minus(");
		l = gen(depth - 1);
		if (__DEBUG) printf(", ");
		r = gen(depth - 1);
		if (__DEBUG) printf(")");
		res = minus(l, r);
		break;
	case 3:
		if (__DEBUG) printf("razy(");
		l = gen(depth - 1);
		if (__DEBUG) printf(", ");
		r = gen(depth - 1);
		if (__DEBUG) printf(")");
		res = razy(l, r);
		break;
	case 4:
		if (__DEBUG) printf("podzielic(");
		l = gen(depth - 1);
		if (__DEBUG) printf(", ");
		r = gen(depth - 1);
		if (__DEBUG) printf(")");
		res = podzielic(l, r);
		break;
	default:
		res = wartosc_dokladna(0.0);
		break;
	}
	if (true || !__DEBUG) {
		printf("%.11f %.11f %.11f ", val(min_wartosc(res)), val(max_wartosc(res)), val(sr_wartosc(res)));
	}
	for (int i = 0; i < __T; ++i) {
		double x = rndf(minv*2, maxv*2);
		if (true || !__DEBUG) printf("%.11f %d ", x, in_wartosc(res, x));
	}
	return res;
}

int main()
{
	unsigned int seed;
	assert(scanf("%u", &seed) == 1);
	srand(seed);

	int d = 2;
	gen(d);
	printf("\n");
}