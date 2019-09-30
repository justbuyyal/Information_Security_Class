#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;
#define DEBUG
class Encrypt
{
public:
	string Ciphers[5] = { "caesar", "playfair", "vernam", "row", "rail_fence" };
	string Plaintext;
	int flag = -1;
	Encrypt(string cipher, string key, string plaintext) {
		Plaintext = plaintext;
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
			Caesar(Plaintext, key);
			break;
		case 1:
			Playfair(Plaintext, key);
			break;
		default:
			cout << "Wrong Input !" << endl;
			break;
		}
	}
private:
	// Caesar
	void Caesar(string &plaintext, string key) {
#ifdef DEBUG
		cout << "Caesar" << endl;
#endif // DEBUG
		int shiftAmount = 0;
		for (int i = key.length() - 1; i >= 0; --i)
		{
			shiftAmount += (key[i] - '0') * pow(10, (key.length() - i - 1));
		}
		shiftAmount = shiftAmount % 26;
		for (int i = 0; i < plaintext.length(); ++i)
		{
			plaintext[i] += shiftAmount;
			if (plaintext[i] > 'z')
			{
				plaintext[i] = 'a' + (plaintext[i] - 'z') - 1;
			}
			plaintext[i] = toupper(plaintext[i]);
		}
		cout << plaintext << endl;
	}
	// Playfair
	void Playfair(string &plaintext, string key)
	{
		vector<string>Encrypt;
		char Map[5][5] = { 0 };
		for (int i = 0; i < plaintext.length(); ++i)
		{
			string temp = "";
			temp += toupper(plaintext[i]);
			if (i != plaintext.length() - 1)
			{
				if (plaintext[i + 1] != plaintext[i])
				{
					temp += toupper(plaintext[i + 1]);
					Encrypt.push_back(temp);
					++i;
				}
				else
				{
					if (plaintext[i] == 'X')
						temp += 'Q';
					else
						temp += 'X';
					Encrypt.push_back(temp);
					continue;
				}
			}
			else
			{
				if (plaintext[i] == 'X')
					temp += 'Q';
				else
					temp += 'X';
				Encrypt.push_back(temp);
			}
		}
#ifdef DEBUG
		for (int i = 0; i < Encrypt.size(); ++i)
		{
			cout << Encrypt[i] << " ";
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
		for (int i = 0; i < Encrypt.size(); ++i)
		{
			int row[2],col[2];
			for (int j = 0; j < 2; ++j)
			{
				bool temp = false;
				for (int k = 0; k < 5; ++k)
				{
					for (int l = 0; l < 5; ++l)
					{
						if (Map[k][l] == Encrypt[i][j] || (Map[k][l] == 'I' && Encrypt[i][j] == 'J') || (Map[k][l] == 'J' && Encrypt[i][j] == 'I'))
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
	}
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
		string plaintext(argv[3]);
		Encrypt encrypt(cipher, key, plaintext);
		return 0;
	}
}