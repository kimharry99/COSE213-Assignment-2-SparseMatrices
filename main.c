#pragma warning(disable:4996)
#include <stdio.h>
#define MAX_SIZE  50

typedef enum{head,entry} tagfield;
typedef struct SMatrixNode* ptrMatrix;
typedef struct _SEntryNode{
	int row;
	int col;
	int value;
} SEntryNode;
typedef struct _SMatrixNode{
	ptrMatrix down;
	ptrMatrix right;
	tagfield tag;
	union {
		ptrMatrix next;
		SEntryNode entry;
	} u;
} SMatrixNode;

ptrMatrix hdnode[MAX_SIZE];

ptrMatrix mread(void);
ptrMatrix newNode(void);


int main() {
	ptrMatrix node;
	char c;
	node = mread();
	scanf("%c", &c);
	printf("%c", c);
	return 0;
}

ptrMatrix newNode() {
	SMatrixNode returnMatrix;
	ptrMatrix _ptrMatrix;
	returnMatrix.down = &returnMatrix;
	returnMatrix.right = &returnMatrix;
	returnMatrix.tag = entry;
	returnMatrix.u.entry.col = 0;
	returnMatrix.u.entry.row = 0;
	returnMatrix.u.entry.value = 0;
	_ptrMatrix = (ptrMatrix)&returnMatrix;
	return _ptrMatrix;
}

ptrMatrix mread(void) {
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	ptrMatrix temp, last, node;

	printf("Enter the number of rows, columns and number of nonzero terms: ");
	scanf("%d%d%d", &numRows, &numCols, &numTerms);
	numHeads = (numCols > numRows) ? numCols : numRows;
	/*헤더 노드 리스트에 대한 헤더 노드를 생성한다.*/
	node = newNode();
}
