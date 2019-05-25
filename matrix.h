
#include <fstream>
#include "Mesh.h"
#include <cstdlib> //qsort在此头文件中声明
#include <string>
#include <sstream>  
#include "list.h"
#include <vector>
#include <assert.h>
#include <Core>
#include <Dense>
#include <iostream>


using Eigen::MatrixXf;
using Eigen::MatrixXd;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::Matrix;
using namespace Eigen;
using namespace std;


void test() {
	
	float k = 0.1;
	Matrix4f mat;
	mat << 5, 2 , 3, 4,3,2,1,3,2,4,4,1,
		2, 1, 3, 1;
	Matrix4f mat1;
	mat1 << 5, 2, 3, 4, 3, 2, 1, 3, 2, 4, 4, 1,
		2, 1, 3, 1;

	Vector4f v1(1, 2, 3, 0); Vector4f v2(1, 3, 1, 0);
	Vector4f u(0, 0, 0, 0);
	u = (1 - k)*v1 + k*v2;

	std::cout << "Here is mat*u:\n" << u.transpose()*(mat1 + mat)*u << std::endl;
	

}

float getMResult(Matrix4f mat,int ) {
	return 1;
}

float DistanceBetween2point(float x1,float y1,float z1,float x2,float y2,float z2) {
	
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) );
}

