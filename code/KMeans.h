/***********************************************
	B. GARCON & P.-L. PISSAVY

			Spectral Clustering

************************************************/

#ifndef KMEANS_H
#define KMEANS_H

#include <random>

#include "ClusteringAlgorithm.h"

/*******************************************************************************
	
			Classe d'implémentation des K-Means

*******************************************************************************/
class KMeans : public ClusteringAlgorithm {
    public:
        KMeans();
        virtual ~KMeans();

        virtual CImg<float> operator()(CImg<float>&,int);

    protected:
		/// generateur aleatoire
        std::mt19937 gen;

    private:
        float d2(CImg<float>&,CImg<float>&,int);
        CImgList<> GetRandomCenter(CImg<float> &,int);
        float GetNearestClass(CImg<float> &,int,CImgList<> &);
        CImgList<> GetNewCenters(CImg<float> &,CImg<float> &,int);
        float GetError(CImg<float> &,CImg<float> &,CImgList<> &);
        float HasChanged(CImg<float> &,CImg<float> &);
		/// methode d'execution appelee dans le foncteur
        CImg<float> execute(CImg<float>&,int);

};

#endif // KMEANS_H
