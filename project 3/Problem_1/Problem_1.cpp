#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* lps;
int len;
int dp[1024][1024];
#define max(a,b) (a>b?a:b)

void LPS(const int m, const char* X, int* LPS_length, char** LPS_string) {
	int lpslen = 0;
	for (int i = 0; i < len + 1; i++) {
		for (int j = 0; j < len + 1; j++) {
				dp[i][j] = 0;
		}
	}
	char* inverseX = (char*)malloc(sizeof(char) * len);
	for (int i = 0; i < len; i++) {
		inverseX[i] = X[len - i - 1];
	}
	for (int i = 1; i <= len; i++) {
		for (int j = 1; j <= len; j++) {
			if (X[i - 1] == inverseX[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;
			}
			else
				dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
			if (dp[i][j] > lpslen)
				lpslen = dp[i][j];
		}
	}
	*LPS_length = lpslen;
	lps = (char*)malloc(sizeof(char) * 1024);
	for (int i = 0; i < lpslen; i++) {
		lps[i] = NULL;
	}
	int idx = lpslen, column = len;
	for (int i = len; i >= 1; i--) {
		for (int j = column; j >= 1; j--) {
			if (dp[i][j] == idx && dp[i][j - 1] == idx - 1 && dp[i - 1][j - 1] == idx - 1 && dp[i - 1][j] == idx - 1) {
				lps[--idx] = X[i - 1];
				column = j - 1;
				break;
			}
			else if (dp[i - 1][j] == idx){
				i--;
				j++;
			}
		}
		if (idx == 0)
			break;
	}
	lps[*LPS_length] = '\0';
	*LPS_string = lps;
	free(inverseX);
}
int main() {
	FILE* fp = fopen("config_LPS.txt","r");
	int num, index = 0, LPS_len;
	char* input,* output, X[1025], ch, *LPS_str, numArr[100];
	while (1) {
		ch = fgetc(fp);
		if (ch == '\n') {
			numArr[index] = '\0';
			index = 0;
			break;
		}
		numArr[index++] = ch;
	}
	num = atoi(numArr);
	for (int i = 0; i < num; i++) {
		input = (char*)malloc(sizeof(char) * 100);
		output = (char*)malloc(sizeof(char) * 100);
		while (1) {
			ch = fgetc(fp);
			if (ch == '\n') {
				input[index] = '\0';
				index = 0;
				break;
			}
			input[index++] = ch;
		}
		while (1) {
			ch = fgetc(fp);
			if (ch == '\n' || ch == EOF) {
				output[index] = '\0';
				index = 0;
				break;
			}
			output[index++] = ch;
		}
		FILE* in = fopen(input,"rb");
		FILE* out = fopen(output, "wb");
		free(input);
		free(output);
		fread(&len, sizeof(int), 1, in);
		for (int i = 0; i < len; i++) {
			fread(&ch, sizeof(char), 1, in);
			X[index++] = ch;
		}
		X[index] = '\0';
		index = 0;
		LPS(len, X, &LPS_len, &LPS_str);
		//파일 출력
		fwrite(&LPS_len, 4, 1, out);
		for (int i = 0; i < LPS_len; i++) {
			ch = LPS_str[i];
			fwrite(&ch, 1, 1, out);
		}
		printf("%s\n", LPS_str);
		free(lps);
		fclose(in);
		fclose(out);
	}
	fclose(fp);
	return 0;
}