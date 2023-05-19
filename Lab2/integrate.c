
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <time.h>

#define DISPENSER 10
#define DO 2
#define KILL 1
#define REQUEST 5
#define SCALE 1000
#define SCALE_SIN 7

double a = 0.001;
double b = 2.5;
double eps = 0.00000001;
// double eps = 0.000000000001;

struct threadData {
  int threadID;
  int id_request;
};
struct lineSegment {
  double lhs;
  double rhs;
};
struct msgbuf {
  long mtype;
  double mtext[4];
};
double func(double x) { return sin(1.0 / x); }
double simpson_integration(double lhs, double middle, double rhs) {
  return (rhs - lhs) / 6.0 * (func(lhs) + 4 * func((middle) + func(rhs)));
}
double trapezoid_integration(double lhs, double rhs) {
  return (rhs - lhs) / 2.0 * (func(lhs) + func(rhs));
}
double adaptive_integration(double lhs, double rhs, double integral,
                            double err) {
  double middle = (lhs + rhs) / 2.0;
  double left_lhs = lhs;
  double left_rhs = middle;
  double right_lhs = middle;
  double right_rhs = rhs;
  double left_integral = trapezoid_integration(left_lhs, left_rhs);
  double right_integral = trapezoid_integration(right_lhs, right_rhs);

  if (fabs(integral - (left_integral + right_integral)) >= err) {
    left_integral =
        adaptive_integration(left_lhs, left_rhs, left_integral, err / 2.0);
    right_integral =
        adaptive_integration(right_lhs, right_rhs, right_integral, err / 2.0);
    return left_integral + right_integral;
  } else {
    return integral;
  }
}
