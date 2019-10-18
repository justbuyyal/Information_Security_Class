#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include "table.h"
using namespace std;

#define DEBUG
#define CIN

//sorce https://bytes.com/topic/c/answers/473139-bitset-rotational-shift
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

//f-function
bitset<32> f_Function(const bitset<32>& Ri_1, const bitset<48>& Ki)
{
	bitset<32> fResult, bTemp;
	bitset<48> expanR;
	unsigned long long int iTemp = 0;

	//Expansion
	for (int i = 0; i < 48; i++)
	{
		expanR[i] = Ri_1[E[i] - 1];
	}

	//XOR with round key
	expanR ^= Ki;

	//S-box substitution
	for (int i = 0, j = 47; j >= 0; j -= 6, ++i)
	{
		int m, n;
		m = 2 * expanR[j] + expanR[j - 5];
		n = 8 * expanR[j - 1] + 4 * expanR[j - 2] + 2 * expanR[j - 3] + expanR[j - 4];
		iTemp += S_Box[i][m][n];
		iTemp <<= 4;
	}

	// assign the result to bTemp
	bTemp = iTemp;

	//Permutation
	for (int i = 0; i < 32; i++)
	{
		fResult[i] = bTemp[P[i] - 1];
	}
	return fResult;
}

int main()
{
	
#ifdef CIN
	stringstream ss1, ss2;	//我不知道為什麼只用一個ss會有bug所以用2個
	string sPlaintext, sKEY;
	unsigned long long  int KEY, plaintext;
	bitset<64> ciphertext;

	//INPUT
	cin >> sKEY >> sPlaintext;
	ss1 << hex << sKEY.erase(0, 2);
	ss1 >> KEY;
	ss2 << hex << sPlaintext.erase(0, 2);
	ss2 >> plaintext;
#endif // CIN
	
	bitset<32> Li_1, Ri_1, Li, Ri;
	
	
	//PROCESS
	
	//set bPlaintext & temp (original bPlaintext)
	bitset<64> bPlaintext, temp(plaintext);

	//Bitwise initial permutation(IP) and split 2 parts
	for (int i = 0,j = 0, k = 0; i < 64; i++)
	{
		bPlaintext[i] = temp[IP[i] - 1];
		if (i < 32)
			Ri_1[j++] = bPlaintext[i];
		else
			Li_1[k++] = bPlaintext[i];
	}

	//KEY PROCESS
	bitset<64> bKEY(KEY);
	bitset<56> PC_1_KEY;
	bitset<28> Ci, Di;
	bitset<48> PC_2_KEY;

	//Key extend(PC_1) and split 2 parts
	for (int i = 0, j = 0; i < 56; i++)
	{
		PC_1_KEY[i] = bKEY[PC_1[i] - 1];
		if (i < 28)
			Di[i] = PC_1_KEY[i];
		else
			Ci[j++] = PC_1_KEY[i];
	}

	//DES Feistel Network 
	for (int N = 0; N < 16; N++)
	{
		//get subkey (rotate)
		if (N == 1 || N == 2 || N == 9 || N == 16)
		{
			lRotate(Ci, 1);
			lRotate(Di, 1);
		}
		else
		{
			lRotate(Ci, 2);
			lRotate(Di, 2);
		}

		unsigned long long temp;
		temp = Ci.to_ullong();
		temp <<= 28;
		temp += Di.to_ullong();
		PC_1_KEY = temp;

		//key shrink(PC_2)
		for (int i = 0; i < 48; i++)
		{
			PC_2_KEY[i] = PC_1_KEY[PC_2[i] - 1];
		}

		//f-function and swap
		Li = Ri_1;
		Ri = Li_1 ^ f_Function(Ri_1, PC_2_KEY);
	}

	// Final Permutation
	for (int i = 0, j = 0, k = 0; k < 64; ++k)
	{
		if (k < 32)
			ciphertext[k] = Ri[i++];
		else
			ciphertext[k] = Li[j++];
	}
	temp = ciphertext;
	for (int i = 0; i < 64; i++)
	{
		ciphertext[i] = temp[IPinverse[i] - 1];
	}

	//OUTPUT
	string Ciphertext = "";
	unsigned long long output = ciphertext.to_ullong();
#ifdef DEBUG
	cout << output << endl;
#endif // DEBUG

	for (int i = 0; i < 16; ++i)
	{
		int r = output % 16;
		if (r < 10)
			Ciphertext.insert(0, ("0" + r));
		else
			Ciphertext.insert(0, ("A" + r - 10));
		output >>= 4;
	}
	cout << Ciphertext << endl;

#ifdef DEBUG
	cin >> sKEY;
#endif //DEBUG
	return 0;
}