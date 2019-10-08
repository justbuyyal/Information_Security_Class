#include <iostream>
#include <cmath>
#include <string>
#include <vector>
using namespace std;
//#define DEBUG

class Decrypt
{
public:
	string Ciphers[5] = { "caesar", "playfair", "vernam", "row", "rail_fence" };
	string Ciphertext;
	int flag = -1;
	Decrypt(string cipher, string key, string Ciphertext) {
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
			Playfair(Ciphertext, key);
			break;
		case 2:
			Vernam(Ciphertext, key);
			break;
		case 3:
			Row(Ciphertext, key);
			break;
		case 4:
			Rail_fence(Ciphertext, key);
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
			else if (Ciphertext[i] > 'Z')
			{
				Ciphertext[i] = 'A' + ((Ciphertext[i] - 'A') % 26);
			}
			Ciphertext[i] = tolower(Ciphertext[i]);
		}
		cout << Ciphertext << endl;
	}

	// Playfair
	void Playfair(string &Ciphertext, string key)
	{
		vector<string>Decrypt;
		char Map[5][5] = { 0 };
		
		//let ciphertext in pairs
		for (int i = 0; i < Ciphertext.length(); i += 2)
		{
			string temp = "";
			temp += Ciphertext[i];
			temp += Ciphertext[i+1];
			Decrypt.push_back(temp);
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

		//create key map
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
		string plaintext = "";
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
					if (col[j] == 0)
						col[j] = 4;
					else
						--col[j];
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
					if (row[j] == 0)
						row[j] = 4;
					else
						--row[j];
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
			
			//erase space in plaintext
			for (int k = 0; k < 2; ++k)
			{
				plaintext += tolower(Map[row[k]][col[k]]);
			}
		}

		// erase 'x' in plaintext we fill in before
		for (int i = 0; i < plaintext.length(); ++i)
		{
			if (plaintext[i] == 'x' && plaintext[i - 1] == plaintext[i + 1])
			{
				plaintext.erase(i, 1);
			}
		}
		cout << plaintext;
	}

	//vernam
	void Vernam(string& Ciphertext, string key)
	{
		string plaintext = "";
			   
		for (int i = 0; i < Ciphertext.length(); ++i)
		{
			if (i < key.length())
				plaintext += tolower(((Ciphertext[i] - 'A') ^ (key[i] - 'A')) + 'A');
			else
				plaintext += tolower(((Ciphertext[i] - 'A') ^ (plaintext[i - key.length()] - 'A')) + 'A');
		}
		
		cout << plaintext;
	}

	//row
	void Row(string & Ciphertext, string key)
	{
		//create empty plaintext table
		vector<vector<char>> pTextTable;
		int col = key.length();
		int row = ceil(double(Ciphertext.length()) / double(col));
		pTextTable.resize(row);
		for (int i = 0; i < row; ++i)
		{
			pTextTable[i].resize(col);
		}

		//decrypt
		int flagP = col + '0';	//position flag check key[i]
		for (int i = 0; i < key.length(); i++)
		{
			//point to ciphertext position
			int p = key[i] - '0' - 1;
			//plaintext is rectangle
			if (Ciphertext.length() % col == 0)
			{
				p *= row;
			}
			//plaintext isn't rectangle
			else
			{
				int flag = Ciphertext.length() % col;
				p *= (row - 1);
				
				if (i < flag)
				{
					flagP = key[i];	//record the column num of the extra latter
					pTextTable[row - 1][i] = tolower(Ciphertext[p + row - 1]);	//fill in extra latter first
				}
				else if (key[i] > flagP)
				{
					p += flag;
				}
			}

			//fill in the part of rectangle
			for (int j = 0; j < row; ++j, ++p)
			{
				pTextTable[j][i] = tolower(Ciphertext[p]);
				if (Ciphertext.length() % col != 0 && j == row - 2)
					break;
			}
		}

		//output
		string plaintext = "";
		for (int i = 0; i < row;++i)
		{
			for (int j = 0; j < col && pTextTable[i][j] != 0; ++j)
			{
				plaintext += pTextTable[i][j];
			}
			pTextTable[i].clear();
		}
		cout << plaintext;
		pTextTable.clear();
	}

	//rail_fence
	void Rail_fence(string& Ciphertext, string key)
	{
		int rows = stoi(key);
		// To lower
		for (int i = 0; i < Ciphertext.length(); ++i)
			Ciphertext[i] = tolower(Ciphertext[i]);

		vector<vector<char>> Fence;
		Fence.resize(rows);
		for (int i = 0; i < rows; i++)
		{
			Fence[i].resize(Ciphertext.length());
		}

		// Decrypting
		int positionx, positiony, count;
		bool flag = false;
		positionx = positiony = count = 0;
		int space = 2 * (rows - 1);
		int length = Ciphertext.length();
		int flagP = 0;

		for (int i = 0; i < rows; i++)
		{
			for (int p = 0, c = 0, positiony = i; positiony < length && p < length; c++)
			{
				if (Ciphertext[p] == '_')
				{
					p++;
					continue;
				}
				Fence[i][positiony] = Ciphertext[p];

				if (c == 1)
					flagP = positiony;

				positiony += space;
				Ciphertext[p] = '_';

				if (i == 0 || i == rows - 1)
					p++;
				else
					p += 2;
			}

			for (int p = 0, positiony = flagP; positiony < length && i != 0 && p <length;)
			{
				if (Ciphertext[p] == '_')
				{
					p++;
					continue;
				}
				Fence[i][positiony] = Ciphertext[p];
				positiony += space;
				Ciphertext[p] = '_';
			}
			flagP--;
		}

		string plaintext = "";
		while (count < Ciphertext.length())
		{
			plaintext += Fence[positionx][positiony];
			++count;
			// Up to Down
			if (!flag)
			{
				if (positionx == (rows - 1))
				{
					flag = true;
					--positionx;
					++positiony;
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
					++positionx;
					++positiony;
					continue;
				}
				--positionx;
				++positiony;
			}
			
		}
		cout << plaintext;
	}
};


int main(int argc, char **argv)
{
#ifdef DEBUG
	cout << argc << endl;
#endif // DEBUG

#ifndef CIN
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
#endif // !CIN

#ifdef CIN
	{
		string cipher,key,Ciphertext;
		while(cin >> cipher >> key >>  Ciphertext)
#endif // CIN

			Decrypt Decrypt(cipher, key, Ciphertext);
		
		return 0;
	}
}