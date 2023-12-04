#include "mp3.h"

void decoding(char **argv) {
	// 파일 open
	ifstream inputFile(argv[2]);
	if (inputFile.fail()) {
		cout << "error : file does not exist" << endl;
		exit(1);
	}

	// hufman code 정보 읽어들이기
	for(int i = 0; i < 128; i++) {
		string s;
		getline(inputFile, s);
		hufCode[i] = s;
	}

	// 8로 나누어떨어지지 않을 경우를 대비한 정보 읽어들이기
	string quotient, remainder;
	getline(inputFile, quotient);
	getline(inputFile, remainder);

	// 압축 해제을 위한 임시 텍스트 작성(2진수 string)
	char c;
	string binaryText;
	while (inputFile.get(c)) {
		bitset<8> bits(c);
		binaryText += bits.to_string();
	}
	inputFile.close();

	// 앞선 정보를 바탕으로 remainder 문자열 조작
	binaryText.erase(8 * stoi(quotient), 8 - stoi(remainder));

	// 디코딩
	ofstream outputFile(string(argv[2]) + ".yy");
	string word;
	for(int i = 0; i < binaryText.length(); i++) {
		word += binaryText.substr(i, 1);
		if (word.length() && find(hufCode.begin(), hufCode.end(), word) != hufCode.end()) {
			// 2진수를 문자로 변환
			bitset<8> bits(word);
			outputFile << static_cast<char>(find(hufCode.begin(), hufCode.end(), word) - hufCode.begin());
			// 초기화
			word.clear();
		}
	}
	outputFile.close();
}
