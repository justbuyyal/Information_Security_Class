#include <iostream>
#include <string>
#include <bitset>
#include "table.h"
using namespace std;

template <std::size_t N>
void rRotate(std::bitset<N>& b, unsigned m)
{
	b = b >> m | b << (N - m);
}

template <std::size_t N>
void lRotate(std::bitset<N>& b, unsigned m)
{
	b = b << m | b >> (N - m);
}


int main()
{
	return 0;

}