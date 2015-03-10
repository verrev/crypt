#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <windows.h>
#include <random>
#include <time.h>
using namespace std;
UINT read(const string &inFilePath, char **bytes)
{
	ifstream inFile(inFilePath.c_str(), ios::binary | ios::ate);
	if (inFile.is_open() && inFile.good()) {
		UINT size = inFile.tellg();
		inFile.seekg(0);
		cout << "Size: " << size << "\n";
		*bytes = new char[size];
		inFile.read(*bytes, size);
		inFile.close();
		return size;
	}
	return 0;
}
void crypt(char *bytes, const UINT &len, const char *key, const UINT &keyLen)
{
	UINT j = 0;
	for (UINT i = 0; i < len; ++i){
		bytes[i] = bytes[i] ^ key[j];
		if (j == keyLen - 1)
			j = 0;
		j++;
	}
}
bool write(const string &outFilePath, const char *bytes, const UINT &len, const UINT &flags = ios::binary)
{
	ofstream outFile(outFilePath.c_str(), flags);
	if (outFile.is_open() && outFile.good()) {
		outFile.write(bytes, len);
		outFile.close();
		return 1;
	}
	return 0;
}
void genKey(string &key, const UINT &len)
{
	srand(time(0));
	for (UINT i = 0; i < len; ++i){
		key.push_back((char)(rand() % (126 - 33 + 1) + 33)); // perhaps unsigned?
	}
}
int main(int argc, char **argv)
{
	string s = "";
	if (argc > 1) s = argv[1];
	while (!s.length())
		cin >> s;

	string key = "";
	cout << "Enter key (input 'OTP' for OTP): ";
	cin >> key;

	char *contentBuffer = nullptr;
	UINT size = read(s, &contentBuffer);
	if (size) {
		if (key == "OTP") {
			key.clear();
			genKey(key, size);
		}
		write("key.txt", key.c_str(), key.length(), 0);
		crypt(contentBuffer, size, key.c_str(), key.length());
		write(s, contentBuffer, size);
	}
	delete[] contentBuffer;
}

