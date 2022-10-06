#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;
typedef struct _node {
	char symbol;
	int freq;
	vector<char> ch;
	struct _node* left;
	struct _node* right;
}Node;
Node** heap;
Node arrNode[128]; 
char* c;
int n;
void adjust_minheap(int root, int n) {
	int child, rootFreq;
	Node* temp;
	temp = heap[root];
	rootFreq = heap[root]->freq;
	child = 2 * root;
	while (child <= n) {
		if ((child < n) && (heap[child]->freq > heap[child + 1]->freq))
			child++;
		if (rootFreq < heap[child]->freq)
			break;
		else {
			heap[child / 2] = heap[child];
			child *= 2;
		}
	}
	heap[child / 2] = temp;
}
Node* delete_minheap(int* n) {
	int parent, child;
	Node* item, *temp;
	item = heap[1];
	temp = heap[(*n)--];
	parent = 1;
	child = 2;
	while (child <= (*n)) {
		if ((child < (*n)) && (heap[child]->freq > heap[child + 1]->freq))
			child++;
		if (temp->freq <= heap[child]->freq)
			break;
		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}
	heap[parent] = temp;
	return item;
}
void insert_minheap(Node* item, int* n) {
	int i;
	i = ++(*n);
	while ((i != 1) && (item->freq < heap[i / 2]->freq)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}
void encoding(Node* ptr) {
	if (ptr) {
		if (ptr->left) {
			for (int i = 0; i < ptr->ch.size(); i++) {
				ptr->left->ch.push_back(ptr->ch[i]);
			}
			ptr->left->ch.push_back('0');
		}
		encoding(ptr->left);
		if (ptr->right) {
			for (int i = 0; i < ptr->ch.size(); i++) {
				ptr->right->ch.push_back(ptr->ch[i]);
			}
			ptr->right->ch.push_back('1');
		}
		encoding(ptr->right);
	}
}
int main() {
	FILE* in = fopen("P3_input_ASCII.txt", "r");
	Node arr[1000];
	for (int i = 0; i < 128; i++) {
		arr[i].symbol = i;
		arr[i].freq = 0;
		arr[i].right = NULL;
		arr[i].left = NULL;
		arrNode[i].symbol = i;
		arrNode[i].freq = 0;
		arrNode[i].right = NULL;
		arrNode[i].left = NULL;
	}
	int ch, charNum = 0, index = 1;
	char orig_char[100000];
	n = 0;
	while (1) {
		ch = fgetc(in);
		if (ch == EOF)
			break;
		orig_char[charNum++] = ch;
		if (arr[ch].freq == 0)
			n++;
		arr[ch].freq++;
		arrNode[ch].freq++;
	}
	//printf("num=%d\n", charNum);
	fclose(in);
	heap = (Node**)malloc(sizeof(Node*) * (n + 1));
	for (int i = 0; i < 128; i++) {
		if (arr[i].freq != 0) {
			heap[index++] = &arr[i];
		}
	}
	for (int i = n / 2; i > 0; i--) {
		adjust_minheap(i, n);
	}
	Node *u, *v, *w;
	index = 128;
	for (int i = n - 1; i > 0; i--) {
		u = delete_minheap(&n);
		v = delete_minheap(&n);
		//printf("u, v = %d %d\n", u->freq, v->freq);
		w = &arr[index++];
		w->freq = u->freq + v->freq;
		w->left = u;
		w->right = v;
		insert_minheap(w, &n);
	}
	Node* p = delete_minheap(&n);
	//printf("p=%d\n", p->freq);
	encoding(p);
	FILE* out = fopen("P3_output_codewords.txt", "w");
	for (int i = 0; i < 128; i++) {
		if (arr[i].freq != 0) {
			fprintf(out, "%c ", arrNode[arr[i].symbol].symbol);
			for (int j = 0; j < arr[i].ch.size(); j++) {
				arrNode[arr[i].symbol].ch.push_back(arr[i].ch[j]);
				fprintf(out, "%c", arr[i].ch[j]);
			}
			fprintf(out, " %.2f\n", (float)arr[i].freq * 100 / charNum);
		}
	}
	fclose(out);
	unsigned int sum = 0;
	for (int i = 0; i < 128; i++) {
		if (arrNode[i].freq != 0) {
			sum += arrNode[i].freq * arrNode[i].ch.size();
			printf("i = %c, %d ", i, i);
			for (int j = 0; j < arrNode[i].ch.size(); j++) {
				printf("%c", arrNode[i].ch[j]);
			}
			printf("\n");
		}
	}
	for (int i = 0; i < charNum; i++) {
		printf("%c", orig_char[i]);
	}
	printf("\n");
	//printf("\n%d", sum);
	//printf("\n\n");
	FILE* bin = fopen("P3_output_encoded.bin", "wb");
	fwrite(&sum, 4, 1, bin);
	c = (char*)malloc(sizeof(char) * sum);
	if (sum % 8 == 0) {
		sum /= 8;
	}
	else {
		sum /= 8;
		sum++;
	}
	fwrite(&sum, 4, 1, bin);
	int num = 0;
	for (int i = 0; i < charNum; i++) {
		ch = orig_char[i];
		for (int j = 0; j < arrNode[ch].ch.size(); j++) {
			c[num++] = arrNode[ch].ch[j];
		}
	}
	for (int i = 0; i < num % 8; i++) {
		c[num++] = '0';
	}
	char character;
	int idx = 0;
	/*while (1) {
		character = 0;
		for (int i = 7; i >= 0; i--) {
			character |= (c[idx++] - '0') << i;
		}
		printf("%c", character);
		fwrite(&character, sizeof(char), 1, bin);
		if (idx >= num)
			break;
	}*/
	while (1) {
		character = 0;
		for (int i = 7; i >= 0; i--) {
			if (c[idx] == '0') {
			}
			else if (c[idx] == '1') {
				character |= (1 << i);
			}
			else {
				printf("?");
			}
			idx++;
		}
		fwrite(&character, sizeof(char), 1, bin);
		if (idx >= num)
			break;
	}
	fclose(bin);
	return 0;
}