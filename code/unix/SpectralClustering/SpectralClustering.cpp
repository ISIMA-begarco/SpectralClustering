#include "SpectralClustering.h"

#include <cmath>
#include <random>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <cfloat>
#include <sstream>
#include <chrono>

#include "KMeans.h"

using namespace std::chrono;
using std::cout;
using std::endl;

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

void printm(const CImg<float> & mat) {
    cimg_forXY(mat,x,y)
        cout << mat(x,y) << "\t";
}

CImg<float> SpectralClustering::operator()(CImg<float> & img, int nbClusters) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now(), t2;
    unsigned long long duration = 0;

    /// etape 0 : extraction des attributs
    cout << "STEP 0\t\t";
    CImg<float> attributs = this->AttributsExtraction(img);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    t1 = t2;
    /// etape 1 : calcul de la matrice d'affinite et calcul de sigma
    cout << "DONE in " << duration << " microseconds" << endl << "STEP 1\t\t";
    float sigma = 25.0f;
    CImg<float> affinityMatrix = this->GetAffinityMatrix(attributs, sigma, nbClusters);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    t1 = t2;
    /// etape 2 : calcul de la matrice L
    cout << "DONE in " << duration << " microseconds" << endl << "STEP 2\t\t";
    CImg<float> matrixL = this->GetLMatrix(affinityMatrix);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    t1 = t2;
    /// etape 3 : obtention de la matrice des k vecteurs propres
    cout << "DONE in " << duration << " microseconds" << endl << "STEP 3\t\t";
    CImg<float> eigenMatrix = this->GetEigenMatrix(matrixL, nbClusters);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    t1 = t2;
    /// etape 4 : normalisation de la matrice 'propre' et preparation pour classification
    cout << "DONE in " << duration << " microseconds" << endl << "STEP 4\t\t";
    CImg<float> matrixForClusteringAlgorithm = this->GetNormalizedEigenMatrix(eigenMatrix);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    t1 = t2;
    /// etape 5 : application de l'algo de clustering (Kmeans par defaut)
    cout << "DONE in " << duration << " microseconds" << endl << "STEP 5\t\t..." << endl;
    CImg<float> clusteringResult = (*classificator)(matrixForClusteringAlgorithm, nbClusters);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    t1 = t2;
    /// etape 6 : assignation des valeurs a l'image
    cout << "DONE in " << duration << " microseconds" << endl << "STEP 6\t\t";
    CImg<float> finalResult = this->GetFinalClusteringImage(clusteringResult, img);
    t2 = high_resolution_clock::now();
    duration = duration_cast<microseconds>( t2 - t1 ).count();
    t1 = t2;
    cout << "DONE in " << duration << " microseconds" << endl;

    return finalResult;
}

/*******************************************************************************

                  Calcul de la fonction d'extraction d'attributs

*******************************************************************************/
CImg<float> SpectralClustering::AttributsExtraction(const CImg<float> & img) {
    int p=2; // Nombre d'attributs
    CImg<float> attributs(img.width(),img.height(),p);

    float maxMean = 0.0;
    float maxStd = 0.0;

// Calcul de la moyenne et de la variance locale
    CImg<float> N(9,9);

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

CImg<float> SpectralClustering::GetAffinityMatrix(const CImg<float> & points, float sigma, unsigned k) {
	int numberOfPoints = points.width()*points.height();	// nombre de points dans l'image
	float coef = 2*Square(sigma);//2*Square(sigma)/k;		// coef a base de variance
	CImg<float> result(numberOfPoints,numberOfPoints,1,1,0.0);	// matrice d'affinite
	CImg<float> distances(numberOfPoints,numberOfPoints,1,1,0.0);	// matrice des distances
	CImg<float> mim(numberOfPoints,1,points.depth(),1,0.0);	// matrice image
	int aa = 0;
	cimg_forXY(points, x, y) {
        mim(aa,0,0) = points(x,y,0);
        mim(aa,0,1) = points(x,y,1);
        ++aa;
	}
    for (int i = 0 ; i < distances.width() ; ++i) {
		for (int j = i+1; j < distances.height(); ++j) {
            distances(j,i) = GetNorm(   mim(i,0,0),
                                        mim(j,0,0),
                                        mim(i,0,1),
                                        mim(j,0,1));
            distances(i,j) = distances(j,i);
		}
	}

	//distances.normalize(0,1);

	for (int i = 0 ; i < result.width() ; ++i) {
		for (int j = i+1; j < result.height(); ++j) {
            result(i,j) = expf(-Square(distances(i,j))/coef);
            result(j,i) = result(i,j);
		}
	}

	return result;
}

CImg<float> SpectralClustering::GetDSqrtInvMatrix(const CImg<float> & affinity) {
	CImg<float> res(affinity.width(),affinity.height(),1,1,0.0);	// matrice D

	cimg_forXY(affinity, x, y) {
		res(x,x) += affinity(x,y);
	}

	cimg_forX(res, x) {
	    res(x,x) = res(x,x);
		res(x,x) =  1.0/sqrtf(res(x,x));
	}

	return res;
}

CImg<float> SpectralClustering::GetLMatrix(const CImg<float> & affinity) {
	CImg<float> dsi = GetDSqrtInvMatrix(affinity);

	CImg<float> res = dsi*affinity*dsi;

	return res;
}

CImg<float> SpectralClustering::GetEigenMatrix(const CImg<float> & l, unsigned k) {
    CImgList<> res = l.get_symmetric_eigen();
	return (res[1].columns(0,k-1));
}

CImg<float> SpectralClustering::GetNormalizedEigenMatrix(CImg<float> & eigenMatrix) {
    CImg<float> result(eigenMatrix.width(),eigenMatrix.height(),1,1,0.0);	// matrice normalisee
    std::vector<float> sums(eigenMatrix.height());

    // calcul des sommes des carres sur les lignes
	cimg_forXY(eigenMatrix, x, y) {
        sums[y] += Square(eigenMatrix(x,y));
	}

	// somme à la racine carree
	cimg_forY(eigenMatrix, y) {
        sums[y] = sqrtf(sums[y]);
	}

	// calcul de la matrice normalisee
	cimg_forXY(eigenMatrix, x, y) {
        result(x,y,0) = (eigenMatrix(x,y)/sums[y]);
	}

	return result;
}

CImg<float> SpectralClustering::GetFinalClusteringImage(CImg<float> & spectralClusters, CImg<float> & original) {
    CImg<float> result(original.width(),original.height());

    cimg_forXY(result,x,y) {
        result(x,y) = spectralClusters(y*result.width()+x);
    }

    return result;
}

