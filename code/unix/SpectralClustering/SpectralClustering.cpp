#include "SpectralClustering.h"

#include <cmath>
#include <random>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <cfloat>
#include <sstream>

#include "KMeans.h"

SpectralClustering::SpectralClustering() {
    this->classificator = new KMeans();
}

SpectralClustering::SpectralClustering(ClusteringAlgorithm & algo) {
    this->classificator = &algo;
}

SpectralClustering::~SpectralClustering() {
    if(classificator!=nullptr)
        delete this->classificator;
}

CImg<unsigned char> SpectralClustering::operator()(CImg<> img, int nbClusters) {
    /// etape 0 : extraction des attributs
    CImg<> attributs = this->AttributsExtraction(img);
    /// etape 1 : calcul de la matrice d'affinite et calcul de sigma
    float sigma = 0.5f;
    CImg<> affinityMatrix = this->GetAffinityMatrix(attributs, sigma, nbClusters);
    /// etape 2 : calcul de la matrice L
    CImg<> matrixL = this->GetLMatrix(affinityMatrix);
    /// etape 3 : obtention de la matrice des k vecteurs propres
    CImg<> eigenMatrix = this->GetEigenMatrix(matrixL, nbClusters);
    /// etape 4 : normalisation de la matrice 'propre' et preparation pour classification
    CImg<> matrixForClusteringAlgorithm = this->GetNormalizedEigenMatrix(eigenMatrix);
    /// etape 5 : application de l'algo de clustering (Kmeans par defaut)
    CImg<> clusteringResult = this->classificator->operator()(matrixForClusteringAlgorithm, nbClusters);
    /// etape 6 : assignation des valeurs a l'image
    return this->GetFinalClusteringImage(clusteringResult, img);
}

/*******************************************************************************

                  Calcul de la fonction d'extraction d'attributs

*******************************************************************************/
CImg<> SpectralClustering::AttributsExtraction(const CImg<> & img) {
    int p=2; // Nombre d'attributs
    CImg<> attributs(img.width(),img.height(),p);

    float maxMean = 0.0;
    float maxStd = 0.0;

// Calcul de la moyenne et de la variance locale
    CImg<> N(9,9);

    cimg_for9x9(img,x,y,0,0,N,float)
    {
        attributs(x,y,0) = N.mean();
        attributs(x,y,1) = std::sqrt(N.variance());

        maxMean = std::max(attributs(x,y,0), maxMean);
        maxStd = std::max(attributs(x,y,1), maxStd);
    }

    cimg_for9x9(img,x,y,0,0,N,float)
    {
        attributs(x,y,0) /= maxMean;
        attributs(x,y,1) /= maxStd;
    }

    attributs.normalize(0,255);

    return attributs;
}

float SpectralClustering::GetNorm(float x1, float x2, float y1, float y2) {
	float a = (x1-y1), b = (x2-y2);
	return sqrtf(a*a+b*b);
}

float SpectralClustering::Square(float a) {
	return a*a;
}

CImg<> SpectralClustering::GetAffinityMatrix(const CImg<> & points, float sigma, unsigned k) {
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

CImg<> SpectralClustering::GetDSqrtInvMatrix(const CImg<> & affinity) {
	CImg<> res(affinity.width(),affinity.height(),1,1,0.0);	// matrice L

	cimg_forXY(affinity, x, y) {
		res(x,x) += affinity(x,y);
	}

	cimg_forX(res, x) {
		res(x,x) = 1/sqrtf(res(x,x));
	}

	return res;
}

CImg<> SpectralClustering::GetLMatrix(const CImg<> & affinity) {
	CImg<> dsi = GetDSqrtInvMatrix(affinity);

	CImg<> res = dsi.get_mul(affinity.get_mul(dsi));

	return res;
}

CImg<> SpectralClustering::GetEigenMatrix(const CImg<> & l, unsigned k) {
	return (l.get_eigen()[1].columns(0,k-1));
}

CImg<> SpectralClustering::GetNormalizedEigenMatrix(CImg<> & eigenMatrix) {
    CImg<> result(eigenMatrix.height(),1,eigenMatrix.width(),1,0.0);	// matrice normalisee
    float * sums = new float[eigenMatrix.height()];

    // calcul des sommes des carres sur les lignes
	cimg_forXY(eigenMatrix, x, y) {
        sums[x] += Square(eigenMatrix(x,y));
	}
	// somme à la racine carree
	cimg_forX(eigenMatrix, x) {
        sums[x] = sqrt(sums[x]);
	}
	// calcul de la matrice normalisee
	cimg_forXY(eigenMatrix, x, y) {
        result(x,0,y) = (eigenMatrix(x,y)/sums[x]);
	}
    // liberation de la memoire
	delete [] sums;

	return result;
}

CImg<> SpectralClustering::GetFinalClusteringImage(CImg<> & spectralClusters, CImg<> & original) {
    CImg<> result(original.width(),original.height());

    cimg_forXY(result,x,y) {
        result(x,y) = spectralClusters(y*result.width()+x,0);
    }

    return result;
}
