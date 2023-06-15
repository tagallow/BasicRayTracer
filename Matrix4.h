#pragma once

class Matrix4
{
private:
	float m[4][4];

	bool helper_inverse(float minv[16]);
public:
	Matrix4(void);
	Matrix4(float a00,float a01,float a02,float a03,float a10,float a11,float a12,float a13,float a20,float a21,float a22,float a23,float a30,float a31,float a32,float a33);
	~Matrix4(void);
	Matrix4(const Matrix4& A);

	void setIdentity();
	void initFromOpenGL(float M[16]);
	void init(float M[4][4]);

	void getOpenGLMatrix(float *M);

	Matrix4 add(Matrix4& A);
	Matrix4 sub(Matrix4& A);
	Matrix4 preMul(Matrix4& A);
	Matrix4 postMul(Matrix4& A);
	Matrix4 inverse();
	Matrix4 transpose();

        void multPos(float V[3]); //returns the result in the array parameter itself
        void multVec(float V[3]); //returns the result in the array parameter itself

};
