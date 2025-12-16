#include <codac>

using namespace std;
using namespace codac2;

int main()
{
  IntervalVector a(2), b(2);
  double u;
  IntervalVector x = a | b;       // ok
  IntervalVector y = a | (u * b); // ok
  // IntervalVector z = (u * a) | b; // breaks
}