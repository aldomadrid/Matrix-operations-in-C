/******************************************************************************

Online C Compiler.
Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>

struct mat {
	int size;
	int rows;
	double* cell;
};

void fillMat(struct mat *x) {
	x->cell[0] = 3.0;
	x->cell[1] = 0;
	x->cell[2] = 2.0;
	x->cell[3] = 2.0;
	x->cell[4] = 0;
	x->cell[5] = -2.0;
	x->cell[6] = 0;
	x->cell[7] = 1.0;
	x->cell[8] = 1.0;
}

int findLCM(int one, int two) {
	int m1 = 2;
	int m2 = 2;
	int n1 = one;
	int n2 = two;

	if (n1 < 0) n1 = n1 * -1;
	if (n2 < 0) n2 = n2 * -1;

	int max = one * two;
	while (n1 != n2 || n1 <= max || n2 <= max) {
		if (n2 > n1) {
			n1 = m1 * one;
			m1++;
		}
		else if (n2 < n1) {
			n2 = two * m2;
			m2++;
		}
		else {
			break;
		}
	}
	return n1;
}

void createIdentity(struct mat *id) {
	//create the identity matrix
	for (int i = 0; i < id->rows; i++) {
		for (int j = 0; j < id->size / id->rows; j++) {
			if (i == j)
				id->cell[i*id->rows + j] = 1;
			else
				id->cell[i*id->rows + j] = 0;
		}
	}
}


bool alignDiagonals(struct mat *m, struct mat *id) {
	bool impossible;
	int order = 0;
	int checked = 0;
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->size / m->rows; j++) {
			if (i == j && m->cell[i*m->rows + j] == 0) {
				//look for values that are not zero in other rows
				//limits are i = 0 and i = rows - 1
				//needs to check the ones that have not been checked before 
				order = i;
				checked = i + 1;
				//check the bottom rows
				impossible = true;
				while (checked < m->rows) {
					//check the next diagonal
					if (m->cell[(checked)*m->rows + j] != 0) {
						//swap rows
						int temp;
						for (int k = 0; k < m->size / m->rows; k++) {
							//swap cell values
							//original
							temp = m->cell[(checked)*(m->rows) + k];
							m->cell[(checked)*(m->rows) + k] = m->cell[i*(m->rows) + k];
							m->cell[i*(m->rows) + k] = temp;
							//identity                            
							temp = id->cell[(checked)*(id->rows) + k];
							id->cell[(checked)*(m->rows) + k] = id->cell[i*(id->rows) + k];
							id->cell[i*(id->rows) + k] = temp;
						}
						impossible = false;
						break;
					}
					checked++;
				}
				//check the upper rows
				checked = i - 1;
				while (checked > 0 && impossible) {
					//check the next diagonal
					if (m->cell[(checked)*m->rows + j] != 0) {
						//swap rows
						int temp;
						for (int k = 0; k < m->size / m->rows; k++) {
							//swap cell values
							//original
							temp = m->cell[(checked)*(m->rows) + k];
							m->cell[(checked)*(m->rows) + k] = m->cell[i*(m->rows) + k];
							m->cell[i*(m->rows) + k] = temp;
							//identity                            
							temp = id->cell[(checked)*(id->rows) + k];
							id->cell[(checked)*(m->rows) + k] = id->cell[i*(id->rows) + k];
							id->cell[i*(id->rows) + k] = temp;
						}
						impossible = false;
						break;
					}
					checked--;
				}
			}
		}
	}
	return impossible;
}


void rowOp(struct mat *m, struct mat *id, int i, int j, int cellP, int lcm) { //add identity matrix

	int m1 = m->cell[(i + cellP)*m->rows + j];
	int m2 = m->cell[(i)*m->rows + j];

	if (m1 < 0) m1 = m1 * -1;
	if (m2 < 0) m2 = m2 * -1;

	m1 = lcm / m1;
	m2 = lcm / m2;

	if (m->cell[(i + cellP)*m->rows + j] > 0 && m->cell[i*m->rows + j] > 0) {
		for (int it = 0; it < m->size / m->rows; it++) {
			m->cell[(i)*m->rows + it] = (m->cell[(i)*m->rows + it])*m2 - (m->cell[(i + cellP)*m->rows + it])*m1;
			id->cell[(i)*m->rows + it] = (id->cell[(i)*m->rows + it])*m2 - (id->cell[(i + cellP)*m->rows + it])*m1;

		}
	}
	else if (m->cell[(i + cellP)*m->rows + j] < 0 && m->cell[i*m->rows + j] > 0) {

		for (int it = 0; it < m->size / m->rows; it++) {
			m->cell[(i)*m->rows + it] = (m->cell[(i)*m->rows + it])*m2 + (m->cell[(i + cellP)*m->rows + it])*m1;
			id->cell[(i)*m->rows + it] = (id->cell[(i)*m->rows + it])*m2 + (id->cell[(i + cellP)*m->rows + it])*m1;

		}
	}
	else if (m->cell[(i + cellP)*m->rows + j] > 0 && m->cell[i*m->rows + j] < 0) {

		for (int it = 0; it < m->size / m->rows; it++) {
			m->cell[(i)*m->rows + it] = (m->cell[(i)*m->rows + it])*m2 + (m->cell[(i + cellP)*m->rows + it])*m1;
			id->cell[(i)*m->rows + it] = (id->cell[(i)*m->rows + it])*m2 + (id->cell[(i + cellP)*m->rows + it])*m1;

		}
	}
	else {
		for (int it = 0; it < m->size / m->rows; it++) {
			m->cell[(i)*m->rows + it] = (m->cell[(i)*m->rows + it])*m2 - (m->cell[(i + cellP)*m->rows + it])*m1;
			id->cell[(i)*m->rows + it] = (id->cell[(i)*m->rows + it])*m2 - (id->cell[(i + cellP)*m->rows + it])*m1;

		}
	}

}

bool eliminateNonDiagonals(struct mat *m, struct mat *id) {
	//find the non-zeros by column 
	bool found;
	for (int j = 0; j < m->size / m->rows; j++) {
		for (int i = 0; i < m->rows; i++) {
			if (i != j && m->cell[i*m->rows + j] != 0) {
				//if the adjasent cell is not zero then any row for operation is valid
				//if the adjasent cell is zero then choose the value that is two rows down
				int look1 = 1;
				found = false;
				bool sink;
				int lcm = 0;
				//if i < j then sink
				if (i < j)
					sink = true; //sink
				else
					sink = false; //bubble up
								  //bubble up and find the first non zero
				while (i - look1 >= 0 && !sink) {
					if (m->cell[(i - look1)*m->rows + j] != 0) {
						//check for adjesent value if j > 0
						found = true;
						lcm = findLCM(m->cell[(i - look1)*m->rows + j], m->cell[i*m->rows + j]);
						rowOp(m, id, i, j, -look1, lcm);
						break;
					}
					look1++;
				}
				look1 = j - i;
				while (i + look1 <= m->rows && sink) {
					if (m->cell[(i + look1)*m->rows + j] != 0) {
						lcm = findLCM(m->cell[(i + look1)*m->rows + j], m->cell[i*m->rows + j]);
						rowOp(m, id, i, j, look1, lcm);   //add identity matrix
						found = true;
						break;
					}
					look1++;
				}
				if (!found) return true;
			}
		}
	}
	return false;
}

void divideMat(struct mat *m, struct mat *id) {
	//divide the identity and the actual matrix
	double dVal = 1.0;
	for (int i = 0; i < m->rows; i++) {
		for (int j = 0; j < m->size / m->rows; j++) {
			if (i == j) {
				dVal = m->cell[i*m->rows + j];
				m->cell[i*m->rows + j] = m->cell[i*m->rows + j] / dVal;
				for (int it = 0; it < id->size / id->rows; it++) {
					id->cell[i*id->rows + it] = id->cell[i*id->rows + it] / dVal;
				}
			}
		}
	}
}


bool inverse(struct mat *m, struct mat *id) {
	bool impossible;
	//check if the matrix is a square matrix
	if (m->size / m->rows != m->rows) {
		cout << "rows: " << m->rows;
		cout << "size: " << m->size << endl; 
		cout << "\ncols != rows" << endl;
		return true; 
	}
    //check if matrix are the same
	if (id->rows != m->rows && id->size != m->size) {
		cout << "\nid and m are not the same" << endl;
		return true;
	}
	//create the identity matrix
	createIdentity(id);
	//look for any diagonal that is not a zero
	impossible = alignDiagonals(m, id);
	//find the non-zeros by column and eliminate them
	impossible = eliminateNonDiagonals(m, id);
	if(impossible)
		return impossible;

	//divide the identity and the original matrix
	divideMat(m, id); 
	return impossible; 
}

int main()
{
	struct mat m, id;
	const int size = 9;
	const int rows = 3;
	double mI[size];   //array of doubles
	double mI1[size];  //another array of doubles
	//initialize original matrix m
    m.size = size;
	m.rows = rows;
	m.cell = NULL;
	m.cell = mI;  //cell points to the base of array mI
	//initialize inverse matrix id

    id.size = size;
	id.rows = rows;
	id.cell = mI1; //cell points to the base of array mI1
	fillMat(&m);
	
	printf("\nYour matrix: \n");
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < size / rows; j++) {
			printf("%+10.02f", m.cell[i*rows + j]);
			if (j == 2)
				printf("\n");
		}
	}

	//feed inverse an empty id matrix with same dimensions as m
	bool impossible = inverse(&m, &id);
	if (impossible) printf("impossible matrix");

	printf("\nYour inverse matrix: \n");
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < size / rows; j++) {
			printf("%+10.02f", id.cell[i*rows + j]);
			if (j == 2)
				printf("\n");
		}
	}
	return 0;
}
