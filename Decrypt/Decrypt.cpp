#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;
#define DEBUG
class Decrypt
{
public:
	string Ciphers[5] = { "caesar", "playfair", "vernam", "row", "rail_fence" };
	string Ciphertext;
	int flag = -1;
	Decrypt(string cipher, string key, string Ciphertext) {
		Ciphertext = Ciphertext;
		for (int i = 0; i < 5; ++i)
		{
			if (cipher.compare(Ciphers[i]) == 0)
			{
				flag = i;
				break;
			}
		}
		switch (flag)
		{
		case 0:
			Caesar(Ciphertext, key);
			break;
		case 1:
			//Playfair(Ciphertext, key);
			break;
		default:
			cout << "Wrong Input !" << endl;
			break;
		}
	}
private:
	// Caesar
	void Caesar(string &Ciphertext, string key) {
#ifdef DEBUG
		cout << "Caesar" << endl;
#endif // DEBUG
		int shiftAmount = 0;
        shiftAmount = stoi(key);
		shiftAmount = shiftAmount % 26;
		for (int i = 0; i < Ciphertext.length(); ++i)
		{
			Ciphertext[i] -= shiftAmount;
			if (Ciphertext[i] < 'A')
			{
				Ciphertext[i] = 'Z' - (('Z' - Ciphertext[i]) % 26);
			}
			Ciphertext[i] = tolower(Ciphertext[i]);
		}
		cout << Ciphertext << endl;
	}
	// Playfair
    /*
	void Playfair(string &Ciphertext, string key)
	{
		vector<string>Decrypt;
		char Map[5][5] = { 0 };
		for (int i = 0; i < Ciphertext.length(); ++i)
		{
			string temp = "";
			temp += toupper(Ciphertext[i]);
			if (i != Ciphertext.length() - 1)
			{
				if (Ciphertext[i + 1] != Ciphertext[i])
				{
					temp += toupper(Ciphertext[i + 1]);
					Decrypt.push_back(temp);
					++i;
				}
				else
				{
					if (Ciphertext[i] == 'X')
						temp += 'Q';
					else
						temp += 'X';
					Decrypt.push_back(temp);
					continue;
				}
			}
			else
			{
				if (Ciphertext[i] == 'X')
					temp += 'Q';
				else
					temp += 'X';
				Decrypt.push_back(temp);
			}
		}
#ifdef DEBUG
		for (int i = 0; i < Decrypt.size(); ++i)
		{
			cout << Decrypt[i] << " ";
		}
#endif // DEBUG

#ifdef DEBUG
		cout << "Playfair" << endl;
#endif // DEBUG
		for (int i = 0; i < key.length(); ++i)
		{
			bool temp = false;
			for (int j = 0; j < 5; ++j)
			{
				for (int k = 0; k < 5; ++k)
				{
					if(Map[j][k] == key[i])
					{
						temp = true;
						break;
					}
					else if (Map[j][k] == 0)
					{
						Map[j][k] = key[i];
						temp = true;
						break;
					}
				}
				if (temp)
				{
					break;
				}
			}
		}
		for (int i = 0; i < 26; ++i)
		{
			// Insert into map
			bool temp = false;
			for (int j = 0; j < 5; ++j)
			{
				for (int k = 0; k < 5; ++k)
				{
					if (Map[j][k] == ('A' + i) || (('A' + i) == 'I' && Map[j][k] == 'J') || (('A' + i) == 'J') && Map[j][k] == 'I')
					{
						temp = true;
						break;
					}
					else if (Map[j][k] == 0)
					{
						Map[j][k] = ('A' + i);
						temp = true;
						break;
					}
				}
				if (temp)
				{
					break;
				}
			}
		}
#ifdef DEBUG
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				cout << Map[i][j] << " ";
			}
			cout << endl;
		}
#endif // DEBUG
		for (int i = 0; i < Decrypt.size(); ++i)
		{
			int row[2],col[2];
			for (int j = 0; j < 2; ++j)
			{
				bool temp = false;
				for (int k = 0; k < 5; ++k)
				{
					for (int l = 0; l < 5; ++l)
					{
						if (Map[k][l] == Decrypt[i][j] || (Map[k][l] == 'I' && Decrypt[i][j] == 'J') || (Map[k][l] == 'J' && Decrypt[i][j] == 'I'))
						{
							temp = true;
							row[j] = k;
							col[j] = l;
#ifdef DEBUG
							cout << "row : " << k << " col : " << l << endl;
#endif // DEBUG
							break;
						}
					}
					if (temp)
						break;
				}
			}
#ifdef DEBUG
			cout << "row : " << row[0] << " , " << row[1] << " col : " << col[0] << " , " << col[1] << endl;
#endif // DEBUG

			// same row
			if (row[0] == row[1])
			{
#ifdef DEBUG
				cout << "case 1" << endl;
#endif // DEBUG
				for (int j = 0; j < 2; ++j)
				{
					if (col[j] == 4)
						col[j] = 0;
					else
						++col[j];
				}
			}
			// same col
			else if (col[0] == col[1])
			{
#ifdef DEBUG
				cout << "case 2" << endl;
#endif // DEBUG

				for (int j = 0; j < 2; ++j)
				{
					if (row[j] == 4)
						row[j] = 0;
					else
						++row[j];
				}
			}
			// other
			else
			{
#ifdef DEBUG
				cout << "case 3" << endl;
#endif // DEBUG
				swap(col[0], col[1]);
			}
			for (int k = 0; k < 2; ++k)
			{
				cout << Map[row[k]][col[k]];
			}
			cout << " " << endl;
		}
		cout << endl;
	}*/
};
//#define DEBUG
int main(int argc, char **argv)
{
#ifdef DEBUG
	cout << argc << endl;
#endif // DEBUG

	if (argc < 4)
	{
#ifdef DEBUG
		cout << "Not in" << endl;
#endif // DEBUG
		return -1;
	}
	else
	{
		string cipher(argv[1]);
		string key(argv[2]);
		string Ciphertext(argv[3]);
		Decrypt Decrypt(cipher, key, Ciphertext);
		return 0;
	}
}