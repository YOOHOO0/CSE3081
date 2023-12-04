#include "mp3.h"

void incoding(char **argv) {
	// 파일 open
	ifstream inputFile(argv[2]);
	if (inputFile.fail()) {
		cout << "error : file does not exist" << endl;
		exit(1);
	}

	// hufman tree & code 생성
	Node *root = makeTree(argv);
	string code;
	genHufCode(root, code);

	// 압축을 위한 임시 텍스트 작성(2진수 string)
	char c;
	string binaryText;
	while (inputFile.get(c))
		binaryText += hufCode[c];
	inputFile.close();

	// hufcode code 정보 출력
	ofstream incodingFile(string(argv[2]) + ".zz");
	for(int i = 0; i < 128; i++)
		incodingFile << hufCode[i] << '\n';

	// 8로 나누어떨어지지 않을 경우를 위한 정보 출력
	incodingFile << binaryText.length() / 8 << '\n';
	incodingFile << binaryText.length() % 8 << '\n';

	// 8비트씩 압축해서 인코딩파일 생성
	for(int i = 0; i < binaryText.length() / 8 + 1; i++) {
		bitset<8> bits(binaryText.substr(i * 8, 8));
		incodingFile << static_cast<char>(bits.to_ulong());
	}
	incodingFile.close();
}

struct cmpNode {
    bool operator()(const Node* l, const Node* r) const {
        return l->freq > r->freq;
    }
};

Node *makeTree(char **argv) {
	priority_queue<Node *, vector<Node *>, cmpNode> pq;
	vector<int> freq(128, 0);
	FILE *inputfile = fopen(argv[2], "r");
	// frequency count
	while (!feof(inputfile)) {
		char c;
		fscanf(inputfile, "%c", &c);
		freq[c]++;
	}
	fclose(inputfile);
	// make node
	for(int i = 0; i < 128; i++) {
		if (freq[i]) {
			Node *n = new Node;
			n->symbol = i;
			n->freq = freq[i];
			n->left = 0;
			n->right = 0;
			pq.push(n);
		}
	}
	// priority queue에 삽입
	Node *u, *v, *w;
	int nodeSize = pq.size();
	for (int i = 0; i < nodeSize - 1; i++) {
		u = pq.top(); pq.pop();
		v = pq.top(); pq.pop();
		Node *w = new Node;
		w->freq = u->freq + v->freq;
		w->left = u;
		w->right = v;
		pq.push(w);
	}
	w = pq.top(); pq.pop(); // root
	return w;
}

void genHufCode(Node* root, std::string code) {
    if (root == 0)
        return;
    if (root->left == 0 && root->right == 0) {
		hufCode[root->symbol] = code;
	}
    genHufCode(root->left, code + "0");
    genHufCode(root->right, code + "1");
}