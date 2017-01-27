/***********************************************
	B. GARCON & P.-L. PISSAVY

			Spectral Clustering

************************************************/

#ifndef CLUSTERINGALGORITHM_H
#define CLUSTERINGALGORITHM_H

#include "CImg.h"

using namespace cimg_library;

/*******************************************************************************
	
			Classe d'abstraction des algorithmes

*******************************************************************************/
class ClusteringAlgorithm
{
    public:
        ClusteringAlgorithm();
        virtual ~ClusteringAlgorithm();

		/// foncteur d'application de l'algorithme
        virtual CImg<float> operator()(CImg<float>&,int) = 0;
		/// limite d'iteration
        static const unsigned long MAXITER = 10000;

    protected:

    private:
};

#endif // CLUSTERINGALGORITHM_H
