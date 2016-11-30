#include "CImg.h"
#include <cmath>

using namespace cimg_library;
using namespace std;

int main(int argc, const char* argv []) {

	return 0;
}

float GetNorm(float x1, float x2, float y1, float y2) {
	float a = (x1-y1), b = (x2-y2);
	return sqrtf(a*a+b*b);
}

float Square(float a) {
	return a*a;
}

CImg<> GetAffinityMatrix(CImg<> points, float sigma, unsigned k) {
	int numberOfPoints = points.width()*points.height();	// nombre de points dans l'image
	float coef = 2*Square(sigma);		// coef a base de variance
	CImg<> result(numberOfPoints,numberOfPoints,points.depth(),1,0.0);	// matrice d'affinite

	cimg_forX(result, i) {
		for (int j = i+1; j < points.width();++j) {
			// remplissage de la partie superieure droite
			result(i,j) = expf(-Square(
								GetNorm(	points(i/points.width(),i%points.width(), 0),
											points(i/points.width(),i%points.width(), 1),
											points(j/points.width(),j%points.width(), 0),
											points(j/points.width(),j%points.width(), 1)))
											/coef);
			// remplissage par symetrie de la partie inferieure gauche
			result(j,i) = result(i,j);
		}
	}

	return result;
}

CImg<> GetDSqrtInvMatrix(CImg<> & affinity) {
	CImg<> res(affinity.width(),affinity.height(),1,1,0.0);	// matrice L

	cimg_forXY(affinity, x, y) {
		res(x,x) += affinity(x,y);
	}

	cimg_forX(res, x) {
		res(x,x) = 1/sqrtf(res(x,x));
	}

	return res;
}

CImg<> GetLMatrix(CImg<> & affinity) {
	CImg<> dsi = GetDSqrtInvMatrix(affinity);

	CImg<> res = dsi.get_mul(affinity.get_mul(dsi));

	return res;
}

CImg<> GetEigenMatrix(CImg<> & l, unsigned k) {
	return (l.get_eigen()[1].columns(0,k-1));
}

void NormalizedEigenMatrix(CImg<> & eigenMatrix) {

	cimg_forXY(eigenMatrix, x, y) {

	}

	return;
}

