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
/* tag=entry�� node�� ���Ǵ� ���� �� ��ġ�� ���� struct�̴�. */
typedef struct _SEntryNode{
	int mRow;
	int mCol;
	int mValue;
} SEntryNode;
typedef struct _SMatrixNode* matrixPointer;
/* ��� ����� node�� ���� struct�̴�. */
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

/* ����� �о� ���� ǥ������ �����Ѵ�. ���� ���� �迭 hdnode�� ���ȴ�.*/
matrixPointer mread(void);
/* �ӽ� node�� ������ �ּҰ��� �����Ѵ�. */
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
	/* ��� ��� ����Ʈ�� ���� ��� ��带 �����Ѵ�. */
	node = newNode();
	node->mTag = entry;
	node->u.struEntry.mRow = numRows;
	node->u.struEntry.mCol = numCols;

	if (!numHeads) node->ptrStruRight = node;
	else {/* ��� ������ �ʱ�ȭ�Ѵ�. */
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			arrHdNode[i] = temp; 
			arrHdNode[i]->mTag = head;
			arrHdNode[i]->ptrStruRight = temp;
			arrHdNode[i]->u.ptruStruNext = temp;
		}
		currentRow = 0;
		last = arrHdNode[0]; /* ���� ���� ������ ��� */
		for (i = 0; i < numTerms; i++) {
			printf("Enter row, column and value: ");
			scanf("%d%d%d", &row, &col, &value);
			if (row > currentRow) { /* ���� ���� ������. */
				last->ptrStruRight = arrHdNode[currentRow];
				currentRow = row;
				last = arrHdNode[row];
			}
			MALLOC(temp, sizeof(*temp));
			temp->mTag = entry;
			temp->u.struEntry.mRow = row;
			temp->u.struEntry.mCol = col;
			temp->u.struEntry.mValue = value;
			last->ptrStruRight = temp; /* �� ����Ʈ�� ���� */
			last = temp;

			/* �� ����Ʈ�� ���� */
			arrHdNode[col]->u.ptruStruNext->ptrStruDown = temp;
			arrHdNode[col]->u.ptruStruNext = temp;
		}
		/* ������ ���� ������ */
		last->ptrStruRight = arrHdNode[currentRow];
		/* ��� �� ����Ʈ�� ������ */
		for (i = 0; i < numCols; i++) {
			arrHdNode[i]->u.ptruStruNext->ptrStruDown = arrHdNode[i];
		}
		/* ��� ��� ������ ������ */
		for (i = 0; i < numHeads - 1; i++) {
			arrHdNode[i]->u.ptruStruNext = arrHdNode[i + 1];
		}
		arrHdNode[numHeads - 1]->u.ptruStruNext = node;
		node->ptrStruRight = arrHdNode[0];
	}
	return node;
}
