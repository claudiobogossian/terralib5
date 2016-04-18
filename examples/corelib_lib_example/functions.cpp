#include "functions.h"

int fatorial(int v)
{
  return (v == 1) ? 1 : v * fatorial(v - 1);
}