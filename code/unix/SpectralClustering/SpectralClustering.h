#ifndef SPECTRALCLUSTERING_H
#define SPECTRALCLUSTERING_H

#include "ClusteringAlgorithm.h"

class SpectralClustering : public ClusteringAlgorithm {
    public:
        SpectralClustering();
        SpectralClustering(ClusteringAlgorithm &);
        virtual ~SpectralClustering();

        virtual CImg<float> operator()(CImg<float>&,int);
        CImg<float> AttributsExtraction(const CImg<float> &);

    protected:
        ClusteringAlgorithm * classificator;

    private:
        CImg<float> GetFinalClusteringImage(CImg<float> &,CImg<float> &);
        CImg<float> GetNormalizedEigenMatrix(CImg<float> &);
        CImg<float> GetEigenMatrix(const CImg<float> &,unsigned);
        CImg<float> GetLMatrix(const CImg<float> &);
        CImg<float> GetDSqrtInvMatrix(const CImg<float> &);
        CImg<float> GetAffinityMatrix(const CImg<float> &,float,unsigned);
        float Square(float);
        float GetNorm(float,float,float,float);
};

#endif // SPECTRALCLUSTERING_H
