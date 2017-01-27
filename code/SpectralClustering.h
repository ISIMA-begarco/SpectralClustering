/***********************************************
	B. GARCON & P.-L. PISSAVY

			Spectral Clustering

************************************************/

#ifndef SPECTRALCLUSTERING_H
#define SPECTRALCLUSTERING_H

#include "ClusteringAlgorithm.h"

/*******************************************************************************
	
			Classe d'implémentation du SPectral Clustering

*******************************************************************************/
class SpectralClustering : public ClusteringAlgorithm {
    public:
        SpectralClustering();
        SpectralClustering(float);
        SpectralClustering(ClusteringAlgorithm &);
        SpectralClustering(ClusteringAlgorithm &,float);
        virtual ~SpectralClustering();

		// foncteur d'appel
        virtual CImg<float> operator()(CImg<float>&,int);
		// extrait les attributs d'une image
        CImg<float> AttributsExtraction(const CImg<float> &);

    protected:
		// classifieur pour etape 5
        ClusteringAlgorithm * classificator;
		// parametre sigma de l'algorithme
        float sigma;

    private:
		// correspondance entre l'analyse spectrale et l'image
        CImg<float> GetFinalClusteringImage(CImg<float> &,CImg<float> &);
		// normalisation de la matrice 'propre'
        CImg<float> GetNormalizedEigenMatrix(CImg<float> &);
		// Retourne la matrice des k premiers vecteurs propres
        CImg<float> GetEigenMatrix(const CImg<float> &,unsigned);
		// Calcul L
        CImg<float> GetLMatrix(const CImg<float> &);
		// calcul D^(-1/2)
        CImg<float> GetDSqrtInvMatrix(const CImg<float> &);
		// retourne une matrice d'affinite
        CImg<float> GetAffinityMatrix(const CImg<float> &,float,unsigned);
		// fait un carré
        float Square(float);
		// renvoie une norme
        float GetNorm(float,float,float,float);
};

#endif // SPECTRALCLUSTERING_H
