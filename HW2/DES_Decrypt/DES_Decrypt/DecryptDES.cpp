#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include "table.h"
using namespace std;

//#define CIN
//#define DEBUG

//sorce https://bytes.com/topic/c/answers/473139-bitset-rotational-shift
template <std::size_t N>
void rRotate(std::bitset<N>& b, unsigned m)
{
	b = b >> m | b << (N - m);
}

//f-function
bitset<32> f_Function(const bitset<32> & Ri_1, const bitset<48> & Ki)
{
	bitset<32> fResult, bTemp;
	bitset<48> expanR;
	unsigned long long int iTemp = 0;

	//Expansion
	for (int i = 0; i < 48; i++)
	{
		expanR[i] = Ri_1[32 - E[47 - i]];
	}

	//XOR with round key
	expanR ^= Ki;

	//S-box substitution
	for (int i = 0, j = 47; j >= 0; j -= 6, i++)
	{
		int m, n;
		m = 2 * (int)expanR[j] + (int)expanR[j - 5];
		n = 8 * (int)expanR[j - 1] + 4 * (int)expanR[j - 2] + 2 * (int)expanR[j - 3] + (int)expanR[j - 4];

		iTemp += S_Box[i][m][n];

		// Do not need to displacement at last time
		if (j > 6)
			iTemp <<= 4;
	}

	// assign the result to bTemp
	bTemp = iTemp;

	//Permutation
	for (int i = 0; i < 32; i++)
	{
		fResult[i] = bTemp[32 - P[31 - i]];
	}

#ifdef DEBUG
	cout << "f-function:" << fResult << endl;
#endif // DEBUG

	return fResult;
}

int main(int argc, char** argv)
{

	stringstream ss1, ss2;	//我不知道為什麼只用一個ss會有bug所以用2個
	string sCiphertext, sKEY;
	unsigned long long  int KEY, ciphertext;
	bitset<64> plaintext;

	//INPUT

#ifdef CIN
	cin >> sKEY >> sCiphertext;
#endif // CIN

#ifndef CIN
	sKEY = argv[1];
	sCiphertext = argv[2];
#endif // !CIN

	ss1 << hex << sKEY.erase(0, 2);
	ss1 >> KEY;
	ss2 << hex << sCiphertext.erase(0, 2);
	ss2 >> ciphertext;

	bitset<32> Li_1, Ri_1, Li, Ri;

	//PROCESS

	//set bPlaintext & temp (original bPlaintext)
	bitset<64> bCiphertext, temp(ciphertext);

	//Bitwise initial permutation(IP) and split 2 parts
	for (int i = 0, j = 0, k = 0; i < 64; i++)
	{
		bCiphertext[i] = temp[64 - IP[63 - i]];
		if (i < 32)
			Ri_1[j++] = bCiphertext[i];
		else
			Li_1[k++] = bCiphertext[i];
	}

	//KEY PROCESS
	bitset<64> bKEY(KEY);
	bitset<56> PC_1_KEY;
	bitset<28> Ci, Di;
	bitset<48> PC_2_KEY;

	//Key extend(PC_1) and split 2 parts
	for (int i = 0, j = 0; i < 56; i++)
	{
		PC_1_KEY[i] = bKEY[64 - PC_1[55 - i]];
		if (i < 28)
			Di[i] = PC_1_KEY[i];
		else
			Ci[j++] = PC_1_KEY[i];
	}
	
	//key shrink(PC_2)
	for (int i = 0; i < 48; i++)
	{
		PC_2_KEY[i] = PC_1_KEY[56 - PC_2[47 - i]];
	}

	//f-function
	Ri = Ri_1;
	Li = Li_1 ^ f_Function(Ri_1, PC_2_KEY);
	

	//DES Feistel Network 
	for (int N = 2; N <= 16; N++)
	{
		//swap
		Ri_1 = Li;
		Li_1 = Ri;
		
		//get subkey (rotate)
		if (N == 2 || N == 9 || N == 16)
		{
			rRotate(Ci, 1);
			rRotate(Di, 1);
		}
		else
		{
			rRotate(Ci, 2);
			rRotate(Di, 2);
		}

		unsigned long long temp;
		temp = Ci.to_ullong();
		temp <<= 28;
		temp += Di.to_ullong();
		PC_1_KEY = temp;

		//key shrink(PC_2)
		for (int i = 0; i < 48; i++)
		{
			PC_2_KEY[i] = PC_1_KEY[56 - PC_2[47 - i]];
		}

		//f-function
		Ri = Ri_1;
		Li = Li_1 ^ f_Function(Ri_1, PC_2_KEY);		
	}

	// Final Permutation
	for (int i = 0, j = 0, k = 0; k < 64; k++)
	{
		if (k < 32)
			temp[k] = Ri[i++];
		else
			temp[k] = Li[j++];
	}
	for (int i = 0; i < 64; i++)
	{
		plaintext[i] = temp[64 - IPinverse[63 - i]];
	}

	//OUTPUT
	string Plaintext = "0x";
	for (int i = 15; i >= 0; --i)
	{
		int count = 0;
		for (int j = 0; j < 4; ++j)
		{
			count += ((int)plaintext[i * 4 + j] * pow(2, j));
		}
		if (count < 10)
			Plaintext += ('0' + count);
		else
			Plaintext += ('a' + (count - 10));
	}
	cout << Plaintext << endl;

#ifdef CIN
	cin >> Plaintext;
#endif // CIN

	return 0;
}