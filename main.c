#pragma warning(disable:4996)
#include <stdio.h>
#include<stdlib.h>
#define MALLOC(p,s)\
if (!((p) = malloc(s))) {\
	fprintf(stderr, "Insufficient memory");\
	exit(EXIT_FAILURE);\
	}
/* x와 y의 대소관계를 비교하는 매크로이다. x가 큰 경우 1, 같은 경우 0, 작은 경우에는 -1이 된다.*/
#define COMPARE(x,y)\
(x==y)?0:((x>y)?1:-1)

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
	matrixPointer mPtrStruDown;
	matrixPointer mPtrStruRight;
	tagfield mTag;
	union {
		matrixPointer mPtruStruNext;
		SEntryNode mStruEntry;
	} u;
} SMatrixNode;

matrixPointer arrHdNode[3][MAX_SIZE];
matrixPointer mread(int index);
matrixPointer newNode(void);
matrixPointer madd(matrixPointer ptrStruAugend, matrixPointer ptrStruAddend);
matrixPointer mtranspose(matrixPointer ptrStruMatrix, int index);
matrixPointer mmult(matrixPointer ptrStruAugend, matrixPointer ptrStruAddend);
void mwrite(matrixPointer node);
matrixPointer createNewEmptyList(int row, int col,int index);
void insertNode(int row, int col, int index, matrixPointer ptrStruListHead, matrixPointer ptrStruNode);
void deleteNode(int row, int col, int index, matrixPointer ptrStruListHead);
void mainMenu();
void subMenu2();
void subMenu3();

int main() {
	matrixPointer ptrStruAugend, ptrStruAddend,ptrStruResult;

	int menu;
	mainMenu();
	scanf("%d", &menu);
	if (menu == 1) {
		printf("\n");
		ptrStruAugend = mread(0);
		subMenu2();
		scanf("%d", &menu);
		if (menu == 1) {
			ptrStruAddend = mread(1);
			subMenu3();
			scanf("%d", &menu);
			if (menu == 1) {
				mwrite(madd(ptrStruAugend, ptrStruAddend));
			}
			else if (menu == 2) {
				ptrStruResult = mmult(ptrStruAugend, ptrStruAddend);
				mwrite(ptrStruResult);
			}
		}
		else if (menu == 2) {
			mtranspose(ptrStruAugend,0);
			mwrite(ptrStruAugend);
		}
	}
	printf("\n\n\n----<최소 행렬 연산이 종료되었습니다. 0을 입력하면 종료 됩니다.> --->>");
	getchar();
	getchar();
	return 0;
}

/* 행렬을 출력한다. */
void mwrite(matrixPointer node) {
	int i;
	matrixPointer temp, head = node->mPtrStruRight;
	/* 행렬의 차원 */
	printf("\n numRows = %d, numCols = %d \n", node->u.mStruEntry.mRow, node->u.mStruEntry.mCol);
	printf(" The matrix by row, column, and value : \n\n");
	for (i = 0; i < node->u.mStruEntry.mRow; i++) {
		/* 각 항에 있는 엔트리들을 출력*/
		for (temp = head->mPtrStruRight; temp != head; temp = temp->mPtrStruRight) {
			printf("%5d%5d%5d \n", temp->u.mStruEntry.mRow, temp->u.mStruEntry.mCol, temp->u.mStruEntry.mValue);
		}
		head = head->u.mPtruStruNext;
	}
}

/* 임시 node를 생성해 주소값을 리턴한다. */
matrixPointer newNode() {
	matrixPointer _ptrMatrix;
	MALLOC(_ptrMatrix, sizeof(*_ptrMatrix));
	return _ptrMatrix;
}

/* 행렬을 읽어 연결 표현으로 구성한다. 전역 보조 배열 hdnode가 사용된다. arrHdNode[index]에 헤더 노드들을 저장한다.*/
matrixPointer mread(int index) {
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;

	printf("\n\nEnter the number of rows, columns and number of nonzero terms: ");
	scanf("%d%d%d", &numRows, &numCols, &numTerms);
	if (numTerms > numRows * numCols) {
		printf("행렬의 최대 크기보다 항이 큽니다.");
	}
	numHeads = (numCols > numRows) ? numCols : numRows;
	/* 헤더 노드 리스트에 대한 헤더 노드를 생성한다. */
	node = newNode();
	node->mTag = entry;
	node->u.mStruEntry.mRow = numRows;
	node->u.mStruEntry.mCol = numCols;
	node->u.mStruEntry.mValue = numTerms;

	if (!numHeads) node->mPtrStruRight = node;
	else {/* 헤더 노드들을 초기화한다. */
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			arrHdNode[index][i] = temp;
			arrHdNode[index][i]->mTag = head;
			arrHdNode[index][i]->mPtrStruRight = temp;
			arrHdNode[index][i]->mPtrStruDown = temp;
			arrHdNode[index][i]->u.mPtruStruNext = temp;
		}
		currentRow = 0;
		last = arrHdNode[index][0];
		/* 현재 행의 마지막 노드 */
		for (i = 0; i < numTerms; i++) {
			while (1) {
				printf("Enter row, column and value(first row of matrice is 0)\n>>");
				scanf("%d%d%d", &row, &col, &value);
				if (row < numRows && col < numCols)
					break;
				else
					printf("out of range\n");
			}
			if (row > currentRow) { /* 현재 행을 종료함. */
				last->mPtrStruRight = arrHdNode[index][currentRow];
				currentRow = row;
				last = arrHdNode[index][row];
			}
			MALLOC(temp, sizeof(*temp));
			temp->mTag = entry;
			temp->u.mStruEntry.mRow = row;
			temp->u.mStruEntry.mCol = col;
			temp->u.mStruEntry.mValue = value;
			last->mPtrStruRight = temp; /* 행 리스트에 연결 */
			last = temp;

			/* 열 리스트에 연결 */
			arrHdNode[index][col]->u.mPtruStruNext->mPtrStruDown = temp;
			arrHdNode[index][col]->u.mPtruStruNext = temp;
		}
		/* 마지막 행을 종료함 */
		last->mPtrStruRight = arrHdNode[index][currentRow];
		/* 모든 열 리스트를 종료함 */
		for (i = 0; i < numCols; i++) {
			arrHdNode[index][i]->u.mPtruStruNext->mPtrStruDown = arrHdNode[index][i];
		}
		/* 모든 헤더 노드들을 연결함 */
		for (i = 0; i < numHeads - 1; i++) {
			arrHdNode[index][i]->u.mPtruStruNext = arrHdNode[index][i + 1];
		}
		arrHdNode[index][numHeads - 1]->u.mPtruStruNext = node;
		node->mPtrStruRight = arrHdNode[index][0];
	}
	return node;
}

/* a (index 0) + b (index 1)를 계산해 바뀐 a의 matrixPointer를 반환한다. */
matrixPointer madd(matrixPointer ptrStruAugend, matrixPointer ptrStruAddend) {
	if (ptrStruAugend->u.mStruEntry.mCol != ptrStruAddend->u.mStruEntry.mCol || ptrStruAugend->u.mStruEntry.mRow != ptrStruAddend->u.mStruEntry.mRow) {
		printf("행렬의 차원이 같지 않습니다.");
		exit(EXIT_FAILURE);
	}
	/* index 0 : augend node, index 1 : auddend node */
	matrixPointer arrPtrStruHead[2];
	/* index 0 : augend node, index 1 : auddend node */
	matrixPointer arrPtrStruEntry[2];
	matrixPointer temp, temp1;

	arrPtrStruHead[0] = ptrStruAugend->mPtrStruRight;
	arrPtrStruHead[1] = ptrStruAddend->mPtrStruRight;
	for (; arrPtrStruHead[0] != ptrStruAugend || arrPtrStruHead[1] != ptrStruAddend;
		arrPtrStruHead[0] = arrPtrStruHead[0]->u.mPtruStruNext, arrPtrStruHead[1] = arrPtrStruHead[1]->u.mPtruStruNext) {
		/* 한 행에 대한 덧셈*/
		for (arrPtrStruEntry[0]=arrPtrStruHead[0]->mPtrStruRight,arrPtrStruEntry[1]=arrPtrStruHead[1]->mPtrStruRight;
			arrPtrStruEntry[0]!=arrPtrStruHead[0]||arrPtrStruEntry[1]!=arrPtrStruHead[1];) {
			if (arrPtrStruHead[1] == arrPtrStruEntry[1])
				arrPtrStruEntry[0] = arrPtrStruHead[0];
			else if (arrPtrStruEntry[0]==arrPtrStruHead[0]) {
				for (; arrPtrStruEntry[1] != arrPtrStruHead[1]; arrPtrStruEntry[1] =arrPtrStruEntry[1]->mPtrStruRight) {
					temp = newNode();
					*temp = *arrPtrStruEntry[1];
					insertNode(arrPtrStruEntry[1]->u.mStruEntry.mRow, arrPtrStruEntry[1]->u.mStruEntry.mCol, 0, ptrStruAugend, temp);
				}
			}
			else {
				switch (COMPARE(arrPtrStruEntry[0]->u.mStruEntry.mCol, arrPtrStruEntry[1]->u.mStruEntry.mCol)) {
				case 1:
					/* auddend의 node를 augend 리스트에 삽입 */
					temp = newNode();
					*temp = *arrPtrStruEntry[1];
					insertNode(temp->u.mStruEntry.mRow, temp->u.mStruEntry.mCol, 0, ptrStruAugend, temp);
					arrPtrStruEntry[1] = arrPtrStruEntry[1]->mPtrStruRight;
					break;
				case 0:
					arrPtrStruEntry[0]->u.mStruEntry.mValue += arrPtrStruEntry[1]->u.mStruEntry.mValue;
					temp = arrPtrStruEntry[0];
					arrPtrStruEntry[0] = arrPtrStruEntry[0]->mPtrStruRight;
					arrPtrStruEntry[1] = arrPtrStruEntry[1]->mPtrStruRight;
					if (!temp->u.mStruEntry.mValue) {
						deleteNode(temp->u.mStruEntry.mRow, temp->u.mStruEntry.mCol, 0, ptrStruAugend);
					}
					break;
				case -1:
					arrPtrStruEntry[0] = arrPtrStruEntry[0]->mPtrStruRight;
					break;
				default:
					exit(EXIT_FAILURE);
					break;
				}
			}
		}
	}

	return ptrStruAugend;
}

/* 전치행렬을 반환 */
matrixPointer mtranspose(matrixPointer ptrStruMatrix,int index) {
	int tempInt,cnt=0;
	matrixPointer temp;
	matrixPointer arrPtrNode[100];
	tempInt=ptrStruMatrix->u.mStruEntry.mCol;
	ptrStruMatrix->u.mStruEntry.mCol = ptrStruMatrix->u.mStruEntry.mRow;
	ptrStruMatrix->u.mStruEntry.mRow=tempInt;

	for (matrixPointer i=ptrStruMatrix->mPtrStruRight;i!=ptrStruMatrix;i=i->u.mPtruStruNext) {
		for (matrixPointer row=i->mPtrStruRight;row!=i;row=row->mPtrStruRight) {
			arrPtrNode[cnt] = row;
			cnt++;
		}
	}
	if (cnt != ptrStruMatrix->u.mStruEntry.mValue)
		exit(EXIT_FAILURE);

	for (int i = 0; i < cnt; i++) {
		if (arrPtrNode[i]->u.mStruEntry.mCol== arrPtrNode[i]->u.mStruEntry.mRow)
			continue;
		temp = newNode();
		*temp = *(arrPtrNode[i]);
		tempInt = temp->u.mStruEntry.mRow;
		temp->u.mStruEntry.mRow = temp->u.mStruEntry.mCol;
		temp->u.mStruEntry.mCol = tempInt;

		deleteNode(temp->u.mStruEntry.mCol, temp->u.mStruEntry.mRow, index, ptrStruMatrix);
		insertNode(temp->u.mStruEntry.mRow, temp->u.mStruEntry.mCol, index, ptrStruMatrix, temp);
	}

	return ptrStruMatrix;
}

/* 행렬곱을 반환한다. */
matrixPointer mmult(matrixPointer ptrStruAugend, matrixPointer ptrStruAddend) {
	matrixPointer result,temp,row,col;
	if (ptrStruAugend->u.mStruEntry.mCol != ptrStruAddend->u.mStruEntry.mRow) {
		printf("잘못된 입력입니다. ");
		exit(EXIT_FAILURE);
	}
	result = newNode();
	result = createNewEmptyList(ptrStruAugend->u.mStruEntry.mRow, ptrStruAddend->u.mStruEntry.mCol, 2);
	for (int i = 0; i < ptrStruAugend->u.mStruEntry.mRow;i++) {
		for (int j = 0; j < ptrStruAddend->u.mStruEntry.mCol;j++) {
			temp = newNode();
			temp->mTag = entry;
			temp->u.mStruEntry.mRow = i;
			temp->u.mStruEntry.mCol = j;
			temp->u.mStruEntry.mValue = 0;
			for (row = arrHdNode[0][i]->mPtrStruRight, col = arrHdNode[1][j]->mPtrStruDown;;) {
				if (row == arrHdNode[0][i]) {
					col = arrHdNode[1][j];
					break;
				}
				if (col == arrHdNode[1][j]) {
					row = arrHdNode[0][i];
					break;
				}
				if (row->u.mStruEntry.mCol == col->u.mStruEntry.mRow){
					temp->u.mStruEntry.mValue += row->u.mStruEntry.mValue * col->u.mStruEntry.mValue;
					row = row->mPtrStruRight;
					col = col->mPtrStruDown;
				}
				else if (row->u.mStruEntry.mCol > col->u.mStruEntry.mRow) {
					col = col->mPtrStruDown;
				}
				else
					row = row->mPtrStruRight;
			}
			if (temp->u.mStruEntry.mValue!=0) {
				insertNode(i, j, 2, result, temp);
			}
		}
	}
	return result;
}

/* row X col 크기의 빈 행렬을 hdNode[index]위치에 생성 */
matrixPointer createNewEmptyList(int _row, int _col,int index) {
	int numHeads, i;
	matrixPointer temp, last, node;

	numHeads = (_col > _row) ? _col : _row;
	/* 헤더 노드 리스트에 대한 헤더 노드를 생성한다. */
	node = newNode();
	node->mTag = entry;
	node->u.mStruEntry.mRow = _row;
	node->u.mStruEntry.mCol = _col;
	node->u.mStruEntry.mValue = 0;

	if (!numHeads) node->mPtrStruRight = node;
	else {/* 헤더 노드들을 초기화한다. */
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			arrHdNode[index][i] = temp;
			arrHdNode[index][i]->mTag = head;
			arrHdNode[index][i]->mPtrStruRight = temp;
			arrHdNode[index][i]->mPtrStruDown = temp;
			arrHdNode[index][i]->u.mPtruStruNext = temp;
		}
		last = arrHdNode[index][0];
		/* 모든 헤더 노드들을 연결함 */
		for (i = 0; i < numHeads - 1; i++) {
			arrHdNode[index][i]->u.mPtruStruNext = arrHdNode[index][i + 1];
		}
		arrHdNode[index][numHeads - 1]->u.mPtruStruNext = node;
		node->mPtrStruRight = arrHdNode[index][0];
	}
	return node;
}

/* ptrStruListHead(index번째 행렬)의 (row, col) 위치에 ptrStruNode를 삽입한다. */
void insertNode(int row, int col, int index, matrixPointer ptrStruListHead, matrixPointer ptrStruNode) {
	matrixPointer ptrStruHead,temp;
	ptrStruListHead->u.mStruEntry.mValue++;
	/* 행 리스트에 연결 */
	ptrStruHead = arrHdNode[index][row];
	if (ptrStruHead->mPtrStruRight->mTag == head) {
		ptrStruNode->mPtrStruRight = ptrStruHead;
		ptrStruHead->mPtrStruRight = ptrStruNode;
	}
	else {
		temp = ptrStruHead;
		while (1) {
			/* 가장 큰 행에 삽입하는 경우 */
			if (temp->mPtrStruRight->mTag==head) {
				ptrStruNode->mPtrStruRight = temp->mPtrStruRight;
				temp->mPtrStruRight = ptrStruNode;
				break;
			}
			if (temp->mPtrStruRight->u.mStruEntry.mCol > col) {
				ptrStruNode->mPtrStruRight = temp->mPtrStruRight;
				temp->mPtrStruRight = ptrStruNode;
				break;
			}
			temp = temp->mPtrStruRight;
		}
	}

	/* 열 리스트에 연결 */
	ptrStruHead = arrHdNode[index][col];
	if (ptrStruHead->mPtrStruDown->mTag == head) {
		ptrStruNode->mPtrStruDown = ptrStruHead;
		ptrStruHead->mPtrStruDown = ptrStruNode;
	}
	else {
		temp = ptrStruHead;
		while (1) {
			/* 가장 큰 열에 삽입하는 경우 */
			if (temp->mPtrStruDown->mTag == head) {
				ptrStruNode->mPtrStruDown = temp->mPtrStruDown;
				temp->mPtrStruDown = ptrStruNode;
				break;
			}
			if (temp->mPtrStruDown->u.mStruEntry.mCol > col) {
				ptrStruNode->mPtrStruDown = temp->mPtrStruDown;
				temp->mPtrStruDown = ptrStruNode;
				break;
			}
			temp = temp->mPtrStruDown;
		}
	}
}

/* hdNode[index]를 이용한 ptrStruListHead행렬에서 row,col 번째 node를 삭제한다. */
void deleteNode(int row, int col, int index, matrixPointer ptrStruListHead) {
	matrixPointer ptrStruHead, temp, delNode;
	ptrStruListHead->u.mStruEntry.mValue--;
	ptrStruListHead->u.mStruEntry.mValue++;
	/* 행 리스트에서 제거 */
	ptrStruHead = arrHdNode[index][row];
	for (temp=ptrStruHead; temp->mPtrStruRight->u.mStruEntry.mCol != col; temp = temp->mPtrStruRight);
	delNode = temp->mPtrStruRight;
	temp->mPtrStruRight = temp->mPtrStruRight->mPtrStruRight;

	/* 열 리스트에서 제거 */
	ptrStruHead = arrHdNode[index][col];
	for (temp = ptrStruHead; temp->mPtrStruDown->u.mStruEntry.mCol != col; temp = temp->mPtrStruDown);
	temp->mPtrStruDown = temp->mPtrStruDown->mPtrStruDown;
	free(delNode);
}

/* 1번 메뉴를 콘솔에 출력한다. */
void mainMenu() {

	for (int i = 0; i < 60; i++) {
		printf("*");
	}
	printf("\n");
	for (int i = 0; i < 15; i++) {
		printf(" ");
	}
	printf("희소행렬의 각종 연산\n");
	for (int i = 0; i < 38; i++) {
		printf(" ");
	}
	printf("자료구조 2차 Assignment\n");
	for (int i = 0; i < 38; i++) {
		printf(" ");
	}
	printf("2018320195 컴퓨터학과\n");
	for (int i = 0; i < 38; i++) {
		printf(" ");
	}
	printf("김동민\n\n");
	for (int i = 0; i < 60; i++) {
		printf("*");
	}
	printf("\n메뉴선택");
	for (int i = 0; i < 52; i++) {
		printf("-");
	}
	printf("\n");
	for (int i = 0; i < 20; i++) {
		printf(" ");
	}
	printf("[1] 희소 행렬 입력하기  [2] 끝내기\n");
	for (int i = 0; i <60; i++) {
		printf("-");
	}
	printf("\n-------------원하는 메뉴를 선택하세요---->>");
}

/* 두 번째 행렬을 입력하는 메뉴를 출력한다. */
void subMenu2() {
	printf("\n메뉴선택");
	for (int i = 0; i < 52; i++) {
		printf("-");
	}
	printf("\n");
	for (int i = 0; i < 20; i++) {
		printf(" ");
	}
	printf("[1] 두번째 희소 행렬 입력하기 [2] 전치행렬 출력 [3] 끝내기\n");
	for (int i = 0; i < 40; i++) {
		printf("-");
	}
	printf("\n-------------원하는 메뉴를 선택하세요---->>");
}

/* 두 번째 행렬에 대한 연산메뉴를 출력한다. */
void subMenu3() {
	printf("\n메뉴선택");
	for (int i = 0; i < 52; i++) {
		printf("-");
	}
	printf("\n");
	for (int i = 0; i < 20; i++) {
		printf(" ");
	}
	printf("[1] 덧셈 [2] 곱셈 [3] 끝내기\n");
	for (int i = 0; i < 40; i++) {
		printf("-");
	}
	printf("\n-------------원하는 메뉴를 선택하세요---->>");
}
