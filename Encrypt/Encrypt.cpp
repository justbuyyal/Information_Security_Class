#include <iostream>
#include <string>
using namespace std;

string Ciphers[5] = { "caesar", "playfair", "vernam", "row", "rail_fence"};
//#define DEBUG
int main(int argc, char **argv)
{
	cout << argc << endl;
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
		int flag = 0;
		for (int i = 0; i < 5; i++)
		{
			if (cipher.compare(Ciphers[i]) == 0)
			{
				flag = i;
#ifdef DEBUG
				cout << "Compare" << endl;
#endif // DEBUG
				break;
			}
		}
		int shiftAmount = 0;
		// Doing Encrypt
		switch (flag)
		{
			// caesar
		case 0:
			for (int i = strlen(argv[2]) - 1; i >= 0; --i)
			{
				shiftAmount += ((argv[2][i] - '0') * pow(10, (strlen(argv[2]) - i - 1)));
			}
			shiftAmount = shiftAmount % 26;
			for (int i = 0; i < strlen(argv[3]); ++i)
			{
				argv[3][i] += shiftAmount;
				if (argv[3][i] > 'z')
				{
					argv[3][i] = 'a' + (argv[3][i] - 'z') - 1;
				}
				argv[3][i] = toupper(argv[3][i]);
			}
			cout << argv[3] << endl;
			break;
			// playfair
		case 1:
			break;
			// vernam
		case 2:
			break;
			// row
		case 3:
			break;
			// rail_fence
		case 4:
			break;
		default:
			break;
		}
		return 0;
	}
}