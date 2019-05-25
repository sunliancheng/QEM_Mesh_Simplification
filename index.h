#ifndef MYINDEX_H

#define MYINDEX_H

#include "Mesh.h"
#include <iostream>
using namespace std;



// �������� �� ��0�� 0ǧ0��0ʮ�� ��Ĭ��Ϊ��һ����
template<class T>
struct myindex{

	T  *current;//ָ��ǰ�ڵ�,�����һ���ڵ�ָ��0w ������1w
	bool isEmpty;//�Ƿ�Ϊ��
	int current_index;//��ǰ��ʾ���
	myindex *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7, *p8, *p9;//��һ�����ҵ� 0-9
	myindex *next;//���ʹ��next��ǧ��ʮ������

	//���캯��
	myindex() {
		current = new T();
		isEmpty = true;
		current_index = -1;
		//ע�� ���ﲻ��p0-P10�����ڴ档next�������ˣ���Ϊֻ�� ��Ϊ��λ��������
		next = NULL;
		p0 = NULL;p1 = NULL;p2 = NULL;p3 = NULL;
		p4 = NULL;p5 = NULL;p6 = NULL;p7 = NULL;
		p8 = NULL;p9 = NULL;
	}
};



template<class T>
void modify_indexlist(int x,myindex<T> *q) {
	//������ǰ����ָ������������Ϣ
	for (int qian = 0; qian <= 9; qian++) {
		if (qian == 0) {
			if (q->p0 == NULL) {
				//cout << "create p0" << endl;
				q->p0 = new myindex<T>();
				q->p0->isEmpty = true;
				q->p0->current_index = q->current_index + 0*x;
			}
			else {

			}
		}
		else if (qian == 1) {
			if (q->p1 == NULL) {
				//cout << "create p1" << endl;
				q->p1 = new myindex<T>();
				q->p1->isEmpty = true;
				q->p1->current_index = q->current_index + 1*x;
			}
			else {

			}
		}
		else if (qian == 2) {
			if (q->p2 == NULL) {
				//cout << "create p2" << endl;
				q->p2 = new myindex<T>();
				q->p2->isEmpty = true;
				q->p2->current_index = q->current_index + 2*x;
			}
			else {

			}
		}
		else if (qian == 3) {
			if (q->p3 == NULL) {
				//cout << "create p3" << endl;
				q->p3 = new myindex<T>();
				q->p3->isEmpty = true;
				q->p3->current_index = q->current_index + 3*x;
			}
			else {

			}
		}
		else if (qian == 4) {
			if (q->p4 == NULL) {
				//cout << "create p4" << endl;
				q->p4 = new myindex<T>();
				q->p4->isEmpty = true;
				q->p4->current_index = q->current_index + 4*x;
			}
			else {

			}
		}
		else if (qian == 5) {
			if (q->p5 == NULL) {
				//cout << "create p5" << endl;
				q->p5 = new myindex<T>();
				q->p5->isEmpty = true;
				q->p5->current_index = q->current_index + 5*x;
			}
			else {

			}
		}
		else if (qian == 6) {
			if (q->p6 == NULL) {
				//cout << "create p6" << endl;
				q->p6 = new myindex<T>();
				q->p6->isEmpty = true;
				q->p6->current_index = q->current_index + 6*x;
			}
			else {

			}
		}
		else if (qian == 7) {
			if (q->p7 == NULL) {
				//cout << "create p7" << endl;
				q->p7 = new myindex<T>();
				q->p7->isEmpty = true;
				q->p7->current_index = q->current_index + 7*x;
			}
			else {

			}
		}
		else if (qian == 8) {
			if (q->p8 == NULL) {
				//cout << "create p8" << endl;
				q->p8 = new myindex<T>();
				q->p8->isEmpty = true;
				q->p8->current_index = q->current_index + 8*x;
			}
			else {

			}
		}
		else if (qian == 9) {
			if (q->p9 == NULL) {
				//cout << "create p9" << endl;
				q->p9 = new myindex<T>();
				q->p9->isEmpty = true;
				q->p9->current_index = q->current_index + 9*x;
			}
			else {

			}
		}
	}
	q->p0->next = q->p1; q->p1->next = q->p2; q->p2->next = q->p3; q->p3->next = q->p4; q->p4->next = q->p5;
	q->p5->next = q->p6; q->p6->next = q->p7; q->p7->next = q->p8; q->p8->next = q->p9; q->p9->next = NULL;

	
}
//��ʼ������ Ĭ��4��
template<class T>
void initial_indexlist(myindex<T> *indexlist) {
	indexlist->isEmpty = false;
	indexlist->current_index = 0;
	indexlist->next = new myindex<T>(); indexlist->next->current_index = 10000;
	indexlist->next->next = new myindex<T>(); indexlist->next->next->current_index = 20000;
	indexlist->next->next->next = new myindex<T>(); indexlist->next->next->next->current_index = 30000;
	indexlist->next->next->next->next = new myindex<T>(); indexlist->next->next->next->next->current_index = 40000;
	indexlist->next->next->next->next->next = NULL;
	for (myindex<T> *q = indexlist; q != NULL; q = q->next) {
		modify_indexlist(1000,q);
		for (myindex<T> *q2 = q->p0; q2 != NULL; q2 = q2->next) {
			modify_indexlist(100,q2);
			for (myindex<T> *q3 = q2->p0; q3 != NULL; q3 = q3->next) {
				modify_indexlist(10, q3);
			}
		}
	}

	//����ʼ���Ƿ�ɹ�
	//for (myindex<T> *q = indexlist; q != NULL; q = q->next) {
	//	for (myindex<T> *q2 = q->p0; q2 != NULL; q2 = q2->next) {
	//		
	//		for (myindex<T> *q3 = q2->p0; q3 != NULL; q3 = q3->next) {
	//			for (myindex<T> *q4 = q3->p0; q4 != NULL; q4 = q4->next) {
	//				//cout << q4->current_index << endl;
	//				if (q4->isEmpty == false) {
	//					cout << "wrong" << endl;
	//				}
	//			}
	//		}
	//	}
	//}


}
//�ֱ��� �� ǧ �� ʮ
template<class T>
void insert_index(int num, T *p, myindex<T> *indexlist) {
	int wan = num / 10000;
	num = num % 10000;
	int qian = num / 1000;
	num = num % 1000;
	int bai = num / 100;
	num = num % 100;
	int shi = num / 10;
	//cout << wan << " " << qian << " " << bai << " " << shi << " " << endl;
	//������һ����qָ�� ��ǰ ��Ķ�������
	myindex<T> *q = new myindex<T>();
	q = indexlist;
	myindex<T> *q2 = new myindex<T>();
	myindex<T> *q3 = new myindex<T>();
	myindex<T> *q4 = new myindex<T>();
	for (int i = 1; i <= wan; i++) {
		q = q->next;
	}
	q->isEmpty = false;
	//ǧ �������
	q2 = q->p0;
	for (int j = 1; j <= qian; j++) {
		q2 = q2->next;
	}
	q2->isEmpty = false;
	q3 = q2->p0;
	for (int k = 1; k <= bai; k++) {
		q3 = q3->next;
	}
	q3->isEmpty = false;
	q4 = q3->p0;
	for (int m = 1; m <= shi; m++) {
		q4 = q4->next;
	}
	q4->isEmpty = false;


	if (num == 1) {
		indexlist->p0->p0->p0->current = p;
	}

	
	q4->current = p;
	//cout << q4->current_index << endl;

	//cout << q->current_index << endl;

}
template<class T>
T* find_index(int num, myindex<T> *indexlist) {
	int wan = num / 10000;
	num = num % 10000;
	int qian = num / 1000;
	num = num % 1000;
	int bai = num / 100;
	num = num % 100;
	int shi = num / 10;
	//cout << wan << " " << qian << " " << bai << " " << shi << " " << endl;
	//������һ����qָ�� ��ǰ ��Ķ�������
	myindex<T> *q ;
	q = indexlist;
	myindex<T> *q2;
	myindex<T> *q3 ;
	myindex<T> *q4;
	for (int i = 1; i <= wan; i++) {
		q = q->next;
	}
	
	//ǧ �������
	q2 = q->p0;
	for (int j = 1; j <= qian; j++) {
		q2 = q2->next;
	}
	
	q3 = q2->p0;
	for (int k = 1; k <= bai; k++) {
		q3 = q3->next;
	}
	
	q4 = q3->p0;
	for (int m = 1; m <= shi; m++) {
		q4 = q4->next;
	}
	

	//cout << q4->current_index << endl;
	return q4->current;
	//cout << coutq4->current_index << endl;

	//cout << q->current_index << endl;
}
template<class T>
T* find_end(myindex<T> *indexlist) {
	myindex<T> *q1;
	for (q1 = indexlist; q1->isEmpty != true && q1->next != NULL && q1->next->isEmpty != true ; q1 = q1->next) {
		//cout << "wan" << endl;
	}
	myindex<T> *q2;
	for (q2 = q1->p0; q2->isEmpty != true && q2->next != NULL && q2->next->isEmpty != true; q2 = q2->next) {
	}
	myindex<T> *q3;
	for (q3 = q2->p0; q3->isEmpty != true && q3->next != NULL && q3->next->isEmpty != true; q3 = q3->next) {
	}
	myindex<T> *q4;
	for (q4 = q3->p0; q4->isEmpty != true && q4->next != NULL && q4->next->isEmpty != true; q4 = q4->next) {
	}
	//cout << q4->current_index << endl;
	return q4->current;

}



template<class T>
void print_index(myindex<T> *indexlist) {
	for (myindex<T> *q = indexlist; q != NULL; q = q->next) {
		for (myindex<T> *q2 = q->p0; q2 != NULL; q2 = q2->next) {
			
			for (myindex<T> *q3 = q2->p0; q3 != NULL; q3 = q3->next) {
				for (myindex<T> *q4 = q3->p0; q4 != NULL; q4 = q4->next) {
					if (q4->isEmpty == true)return;
					cout << *q4->current << endl;
					
				}
			}
		}
	}
}



#endif