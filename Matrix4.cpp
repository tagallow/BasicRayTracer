#include ".\matrix4.h"
#include <math.h>

Matrix4::Matrix4(void)
{
	setIdentity();
}

Matrix4::Matrix4(float a00,float a01,float a02,float a03,float a10,float a11,float a12,float a13,float a20,float a21,float a22,float a23,float a30,float a31,float a32,float a33)
{
	m[0][0] = a00;
	m[0][1] = a01;
	m[0][2] = a02;
	m[0][3] = a03;

	m[1][0] = a10;
	m[1][1] = a11;
	m[1][2] = a12;
	m[1][3] = a13;

	m[2][0] = a20;
	m[2][1] = a21;
	m[2][2] = a22;
	m[2][3] = a23;

	m[3][0] = a30;
	m[3][1] = a31;
	m[3][2] = a32;
	m[3][3] = a33;
}


Matrix4::~Matrix4(void)
{
}

Matrix4::Matrix4(const Matrix4& A)
{
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			m[i][j] = A.m[i][j];
		}
	}
}

void Matrix4::setIdentity()
{
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			if (i==j)
				m[i][j] = 1.0f;
			else
				m[i][j] = 0.0f;
		}
	}
}

void Matrix4::init(float M[4][4])
{
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			m[i][j] = M[i][j];
		}
	}
}

void Matrix4::initFromOpenGL(float M[16])
{
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			m[j][i] = M[4*i+j];  //opengl stores matrices in column major order
		}
	}
}

void Matrix4::getOpenGLMatrix(float *M)
{
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			M[4*i+j] = m[j][i];  //opengl stores matrices in column major order
		}
	}
}

void Matrix4::multPos(float Point[3])
{
	float V[4],W[4];
	int i,j;

	for (i=0;i<3;i++)
		V[i] = Point[i];
	V[3] = 1.0f;

	for (i=0;i<4;i++)
	{
		W[i] = 0.0f;
		for (j=0;j<4;j++)
		{
			W[i] = W[i] + m[i][j]*V[j];
		}
	}

	for (i=0;i<3;i++)
		Point[i] = W[i]/W[3];
}

void Matrix4::multVec(float Point[3])
{
	float V[4],W[4];
	int i,j;

	for (i=0;i<3;i++)
		V[i] = Point[i];
	V[3] = 0.0f;

	for (i=0;i<4;i++)
	{
		W[i] = 0.0f;
		for (j=0;j<4;j++)
		{
			W[i] = W[i] + m[i][j]*V[j];
		}
	}

	for (i=0;i<3;i++)
		Point[i] = W[i];
}


Matrix4 Matrix4::add(Matrix4& A)
{
	Matrix4 C;
	float c[4][4];
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			c[i][j] = m[i][j] + A.m[i][j];
		}
	}
	C.init(c);
	return C;
}

Matrix4 Matrix4::sub(Matrix4& A)
{
	Matrix4 C;
	float c[4][4];
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			c[i][j] = m[i][j] - A.m[i][j];
		}
	}
	C.init(c);
	return C;

}

Matrix4 Matrix4::preMul(Matrix4& A)
{
	Matrix4 C;
	float c[4][4];
	int i,j,k;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			c[i][j] = 0.0f;
			for (k=0;k<4;k++)
			{
                c[i][j] = c[i][j] + A.m[i][k]*m[k][j];
			}
		}
	}
	C.init(c);
	return C;
}

Matrix4 Matrix4::postMul(Matrix4& A)
{
	Matrix4 C;
	float c[4][4];
	int i,j,k;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			c[i][j] = 0.0f;
			for (k=0;k<4;k++)
			{
                c[i][j] = c[i][j] + m[i][k]*A.m[k][j];
			}
		}
	}
	C.init(c);
	return C;
}

Matrix4 Matrix4::inverse()
{
	float minv[16];
	Matrix4 C;

	if (helper_inverse(minv))
		C.initFromOpenGL(minv);

	return C;
}

Matrix4 Matrix4::transpose()
{
	Matrix4 C;
	int i,j;

	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			C.m[i][j] = m[j][i];
		}
	}
	return C;
}

bool Matrix4::helper_inverse(float minv[16])
{

	float r1[8], r2[8], r3[8], r4[8];
	float *s[4], *tmprow;

	s[0] = &r1[0];
	s[1] = &r2[0];
	s[2] = &r3[0];
	s[3] = &r4[0];

	register int i,j,p,jj;
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			s[i][j] = m[i][j];
			if(i==j) s[i][j+4] = 1.0;
			else     s[i][j+4] = 0.0;
		}
	}
	float scp[4];
	for(i=0;i<4;i++)
	{
		scp[i] = fabs(s[i][0]);
		for(j=1;j<4;j++)
			if(fabs(s[i][j]) > scp[i])
				scp[i] = fabs(s[i][j]);
			if(scp[i] == 0.0)
				return false; // singular matrix!
	}

	int pivot_to;
	float scp_max;
	for(i=0;i<4;i++)
	{
		// select pivot row
		pivot_to = i;
		scp_max = fabs(s[i][i]/scp[i]);
		// find out which row should be on top
		for(p=i+1;p<4;p++)
			if(fabs(s[p][i]/scp[p]) > scp_max)
			{
				scp_max = fabs(s[p][i]/scp[p]);
				pivot_to = p;
			}
			// Pivot if necessary
			if(pivot_to != i)
			{
				tmprow = s[i];
				s[i] = s[pivot_to];
				s[pivot_to] = tmprow;
				float tmpscp;
				tmpscp = scp[i];
				scp[i] = scp[pivot_to];
				scp[pivot_to] = tmpscp;
			}

			float mji;
			// perform gaussian elimination
			for(j=i+1;j<4;j++)
			{
				mji = s[j][i]/s[i][i];
				s[j][i] = 0.0;
				for(jj=i+1;jj<8;jj++)
					s[j][jj] -= mji*s[i][jj];
			}
	}
	if(s[3][3] == 0.0)
		return false; // singular matrix!

	//
	// Now we have an upper triangular matrix.
	//
	//  x x x x | y y y y
	//  0 x x x | y y y y
	//  0 0 x x | y y y y
	//  0 0 0 x | y y y y
	//
	//  we'll back substitute to get the inverse
	//
	//  1 0 0 0 | z z z z
	//  0 1 0 0 | z z z z
	//  0 0 1 0 | z z z z
	//  0 0 0 1 | z z z z
	//

	float mij;
	for(i=3;i>0;i--)
	{
		for(j=i-1;j > -1; j--)
		{
			mij = s[j][i]/s[i][i];
			for(jj=j+1;jj<8;jj++)
				s[j][jj] -= mij*s[i][jj];
		}
	}

	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			minv[j*4+i] = s[i][j+4] / s[i][i];

	return true;
}

