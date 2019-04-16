#pragma warning(disable:4996)
#include <stdio.h>
#include<stdlib.h>
#define MALLOC(p,s)\
if (!((p) = malloc(s))) {\
	fprintf(stderr, "Insufficient memory");\
	exit(EXIT_FAILURE);\
	}
#define MAX_SIZE  50

typedef enum{head,entry} tagfield;
/* tag=entry인 node에 사용되는 값과 그 위치에 대한 struct이다. */
typedef struct _SEntryNode{
	int mRow;
	int mCol;
	int mValue;
} SEntryNode;
typedef struct _SMatrixNode* matrixPointer;
/* 모든 행렬의 node에 대한 struct이다. */
typedef struct _SMatrixNode{
	matrixPointer ptrStruDown;
	matrixPointer ptrStruRight;
	tagfield mTag;
	union {
		matrixPointer ptruStruNext;
		SEntryNode struEntry;
	} u;
} SMatrixNode;

matrixPointer arrHdNode[MAX_SIZE];

/* 행렬을 읽어 연결 표현으로 구성한다. 전역 보조 배열 hdnode가 사용된다.*/
matrixPointer mread(void);
/* 임시 node를 생성해 주소값을 리턴한다. */
matrixPointer newNode(void);


int main() {
	matrixPointer ptrStruNode;
	int a;
	char c;
	ptrStruNode = mread();
	a = ptrStruNode->ptrStruRight->u.ptruStruNext->ptrStruDown->u.struEntry.mValue;
	printf("the value of (0,0) is %d",a);
	return 0;
}

matrixPointer newNode() {
	SMatrixNode _SMatrixNode;
	matrixPointer _ptrMatrix;
	MALLOC(_ptrMatrix, sizeof(*_ptrMatrix));
	return _ptrMatrix;
}

matrixPointer mread(void) {
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;

	printf("Enter the number of rows, columns and number of nonzero terms: ");
	scanf("%d%d%d", &numRows, &numCols, &numTerms);
	numHeads = (numCols > numRows) ? numCols : numRows;
	/* 헤더 노드 리스트에 대한 헤더 노드를 생성한다. */
	node = newNode();
	node->mTag = entry;
	node->u.struEntry.mRow = numRows;
	node->u.struEntry.mCol = numCols;

	if (!numHeads) node->ptrStruRight = node;
	else {/* 헤더 노드들을 초기화한다. */
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			arrHdNode[i] = temp; 
			arrHdNode[i]->mTag = head;
			arrHdNode[i]->ptrStruRight = temp;
			arrHdNode[i]->u.ptruStruNext = temp;
		}
		currentRow = 0;
		last = arrHdNode[0]; /* 현재 행의 마지막 노드 */
		for (i = 0; i < numTerms; i++) {
			printf("Enter row, column and value: ");
			scanf("%d%d%d", &row, &col, &value);
			if (row > currentRow) { /* 현재 행을 종료함. */
				last->ptrStruRight = arrHdNode[currentRow];
				currentRow = row;
				last = arrHdNode[row];
			}
			MALLOC(temp, sizeof(*temp));
			temp->mTag = entry;
			temp->u.struEntry.mRow = row;
			temp->u.struEntry.mCol = col;
			temp->u.struEntry.mValue = value;
			last->ptrStruRight = temp; /* 행 리스트에 연결 */
			last = temp;

			/* 열 리스트에 연결 */
			arrHdNode[col]->u.ptruStruNext->ptrStruDown = temp;
			arrHdNode[col]->u.ptruStruNext = temp;
		}
		/* 마지막 행을 종료함 */
		last->ptrStruRight = arrHdNode[currentRow];
		/* 모든 열 리스트를 종료함 */
		for (i = 0; i < numCols; i++) {
			arrHdNode[i]->u.ptruStruNext->ptrStruDown = arrHdNode[i];
		}
		/* 모든 헤더 노드들을 연결함 */
		for (i = 0; i < numHeads - 1; i++) {
			arrHdNode[i]->u.ptruStruNext = arrHdNode[i + 1];
		}
		arrHdNode[numHeads - 1]->u.ptruStruNext = node;
		node->ptrStruRight = arrHdNode[0];
	}
	return node;
}
