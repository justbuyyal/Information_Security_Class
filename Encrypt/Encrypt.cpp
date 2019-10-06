#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;
//#define DEBUG
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
		case 2:
			Vernam(Plaintext, key);
			break;
		case 3:
			Row_Transposition(Plaintext, key);
			break;
		case 4:
			Rail_Fence(Plaintext, key);
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
#ifdef DEBUG
		cout << shiftAmount << endl;
#endif // DEBUG

		shiftAmount = shiftAmount % 26;
		for (int i = 0; i < plaintext.length(); ++i)
		{
			if ((plaintext[i] + shiftAmount) > 'z')
			{
				plaintext[i] = 'a' + ((plaintext[i] + shiftAmount) - 'z' - 1);
			}
			else
			{
				plaintext[i] += shiftAmount;
			}
			plaintext[i] = toupper(plaintext[i]);
			cout << plaintext[i];
		}
		cout << endl;
	}
	// Playfair
	void Playfair(string &plaintext, string key)
	{
		vector<string>Encrypt;
		Encrypt.clear();
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
		}
		cout << endl;
	}
	// Vernam
	void Vernam(string &plaintext, string key)
	{
		int keyLength = key.length();
#ifdef DEBUG
		cout << "key : " << keyLength << endl;
#endif // DEBUG
		for (int i = 0; i < plaintext.length(); ++i)
		{
			plaintext[i] = toupper(plaintext[i]);
		}
		// Build New Key
		key += plaintext;
		key.erase(key.end() - keyLength, key.end());
#ifdef DEBUG
		cout << key << endl;
#endif // DEBUG

		for (int i = 0; i < plaintext.length(); ++i)
		{
			plaintext[i] = ((plaintext[i] - 'A') ^ (key[i] - 'A')) + 'A';
			cout << plaintext[i];
		}
		cout << endl;
	}
	// Row
	void Row_Transposition(string &plaintext, string key)
	{
		vector<int>position;
		position.clear();
		int row = plaintext.length() / key.length();
		if (plaintext.length() % key.length())
		{
			++row;
		}
		// To upper
		for (int i = 0; i < plaintext.length(); ++i)
			plaintext[i] = toupper(plaintext[i]);
		// Build Dynamic array
		char** Map = new char*[row];
		for (int i = 0; i < row; ++i)
		{
			Map[i] = new char[key.length()];
		}
		// Array initial
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < key.length(); ++j)
			{
				Map[i][j] = 0;
			}
		}
		// Make Order
		for (int i = 0; i < key.length(); ++i)
		{
			position.push_back((key[i] - '0' - 1));
		}
		// Insert to Map
		for (int i = 0; i < plaintext.length(); ++i)
		{
			Map[i / position.size()][position[i % position.size()]] = plaintext[i];
		}
		// Output
		for (int i = 0; i < position.size(); ++i)
		{
			for (int j = 0; j < row; ++j)
			{
				if(Map[j][i])
					cout << Map[j][i];
			}
		}
		cout << endl;
	}
	// Rail_Fence
	void Rail_Fence(string &plaintext, string key)
	{
		int rows = 0;
		for (int i = key.length() - 1; i >= 0; --i)
		{
			rows += (key[i] - '0') * pow(10, (key.length() - i - 1));
		}
		// To upper
		for (int i = 0; i < plaintext.length(); ++i)
			plaintext[i] = toupper(plaintext[i]);
		// Build Dynamic rows
		char** Fence = new char*[rows];
		for (int i = 0; i < rows; ++i)
		{
			Fence[i] = new char[plaintext.length()];
		}
		// Array initial
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < plaintext.length(); ++j)
			{
				Fence[i][j] = 0;
			}
		}
		// Encrypting
		int positionx, positiony, count;
		bool flag = false;
		positionx = positiony = count = 0;
		while (count <= plaintext.length())
		{
			Fence[positionx][positiony] = plaintext[count];
			// Up to Down
			if (!flag)
			{
				if (positionx == (rows - 1))
				{
					flag = true;
					continue;
				}
				++positionx;
				++positiony;
			}
			// Down to Up
			else
			{
				if (positionx == 0)
				{
					flag = false;
					continue;
				}
				--positionx;
				++positiony;
			}
			++count;
		}
		// Output
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < plaintext.length(); ++j)
			{
				if (Fence[i][j])
				{
					cout << Fence[i][j];
				}
				else
				{
					continue;
				}
			}
		}
		cout << endl;
	}
};
int main(int argc, char **argv)
{
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