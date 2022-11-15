#include <stdio.h>
#include <math.h>

#include "ary.h"

const double precision = 1e-10;

bool iszero2(double x) { return fabs(x) < precision; }

bool equals(double a, double b) { return iszero2(a - b); }

// 1 / [-1, 1] + [2, 4]
void test1()
{
  printf("test1: ");
  wartosc inv = podzielic(wartosc_dokladna(1.0), wartosc_od_do(-1.0, 1.0));
  wartosc res = plus(inv, wartosc_od_do(2.0, 4.0));
  if (min_wartosc(res) > -HUGE_VAL || max_wartosc(res) < HUGE_VAL || !in_wartosc(res, 10.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// 1 / [-1, 1] + 1 / [-1, 1]
void test2()
{
  printf("test2: ");
  wartosc inv = podzielic(wartosc_dokladna(1.0), wartosc_od_do(-1.0, 1.0));
  wartosc res = plus(inv, inv);
  if (min_wartosc(res) > -HUGE_VAL || max_wartosc(res) < HUGE_VAL) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// [-1, 1] / [-1, 1]
void test3()
{
  printf("test3: ");
  wartosc res = podzielic(wartosc_od_do(-1.0, 1.0), wartosc_od_do(-1.0, 1.0));
  if (min_wartosc(res) > -HUGE_VAL || max_wartosc(res) < HUGE_VAL) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// 1 + [1, 7]
void test_add1()
{
  printf("test_add1: ");
  wartosc a = wartosc_dokladna(1.0);
  wartosc b = wartosc_dokladnosc(4.0, 75.0);
  wartosc res = plus(a, b);
  if (!equals(min_wartosc(res), 2.0) || !equals(max_wartosc(res), 8.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// 2 + 4
void test_add2()
{
  printf("test_add2: ");
  wartosc a = wartosc_dokladna(4.0);
  wartosc b = wartosc_dokladna(2.0);
  wartosc res = plus(a, b);
  if (!in_wartosc(res, 6.0) || in_wartosc(res, 6.00001) || in_wartosc(res, 5.99999)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// [2, 3] - [4, 7]
void test_sub1()
{
  printf("test_sub1: ");
  wartosc a = wartosc_dokladnosc(2.5, 20.0);
  wartosc b = wartosc_od_do(4.0, 7.0);
  wartosc res = minus(a, b);
  if (!equals(min_wartosc(res), -5.0) || !equals(max_wartosc(res), -1.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// 2 * [4, 7]
void test_mul1()
{
  printf("test_mul1: ");
  wartosc a = wartosc_dokladna(2.0);
  wartosc b = wartosc_od_do(4.0, 7.0);
  wartosc res = razy(a, b);
  if (!equals(min_wartosc(res), 8.0) || !equals(max_wartosc(res), 14.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// [3, 6] * [4, 7]
void test_mul2()
{
  printf("test_mul2: ");
  wartosc a = wartosc_od_do(3.0, 6.0);
  wartosc b = wartosc_od_do(4.0, 7.0);
  wartosc res = razy(a, b);
  if (!equals(min_wartosc(res), 12.0) || !equals(max_wartosc(res), 42.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// [-1, -0.5] * [2, 4]
void test_mul3()
{
  printf("test_mul3: ");
  wartosc a = wartosc_od_do(-1.0, -0.5);
  wartosc b = wartosc_od_do(2.0, 4.0);
  wartosc res = razy(a, b);
  if (!equals(min_wartosc(res), -4.0) || !equals(max_wartosc(res), -1.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// [-1, 3] * [2, 4]
void test_mul4()
{
  printf("test_mul4: ");
  wartosc a = wartosc_dokladnosc(1.0, 200.0);
  wartosc b = wartosc_od_do(2.0, 4.0);
  wartosc res = razy(a, b);
  if (!equals(min_wartosc(res), -4.0) || !equals(max_wartosc(res), 12.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// 1 / [0, 2]
void test_inv1()
{
  printf("test_inv1: ");
  wartosc a = wartosc_dokladna(1.0);
  wartosc b = wartosc_od_do(0.0, 2.0);
  wartosc res = podzielic(a, b);
  if (!equals(min_wartosc(res), 0.5) || max_wartosc(res) < HUGE_VAL) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// 1 / 0
void test_nan1()
{
  printf("test_nan1: ");
  wartosc a = wartosc_dokladna(1.0);
  wartosc b = wartosc_dokladna(0.0);
  wartosc res = podzielic(a, b);
  if (in_wartosc(res, 1.0) || in_wartosc(res, 0.0)) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

// sr_wartosc([1, 2] / 0)
void test_nan2()
{
  printf("test_nan2: ");
  wartosc a = wartosc_od_do(1.0, 2.0);
  wartosc b = wartosc_dokladna(0.0);
  wartosc res = podzielic(a, b);
  if (!isnan(sr_wartosc(res))) {
    printf("Wyjebalo sie! [%f, %f]\n", min_wartosc(res), max_wartosc(res));
    return;
  }
  printf("OK\n");
}

int main()
{
  test_add1();
  test_add2();
  test_sub1();
  test_mul1();
  test_mul2();
  test_mul3();
  test_mul4();
  test_inv1();
  test_nan1();
  test_nan2();

  test1();
  test2();
  test3();
  
  return 0;
}
