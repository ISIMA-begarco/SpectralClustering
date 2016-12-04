#ifndef SPECTRALCLUSTERING_H
#define SPECTRALCLUSTERING_H

#include "ClusteringAlgorithm.h"

class SpectralClustering : public ClusteringAlgorithm {
    public:
        SpectralClustering();
        SpectralClustering(ClusteringAlgorithm &);
        virtual ~SpectralClustering();

        virtual CImg<unsigned char> operator()(CImg<>,int);

    protected:
        ClusteringAlgorithm * classificator;

    private:
        CImg<> GetFinalClusteringImage(CImg<> &,CImg<> &);
        CImg<> GetNormalizedEigenMatrix(CImg<> &);
        CImg<> GetEigenMatrix(const CImg<> &,unsigned);
        CImg<> GetLMatrix(const CImg<> &);
        CImg<> GetDSqrtInvMatrix(const CImg<> &);
        CImg<> GetAffinityMatrix(const CImg<> &,float,unsigned);
        float Square(float);
        float GetNorm(float,float,float,float);
        CImg<> AttributsExtraction(const CImg<> &);
};

#endif // SPECTRALCLUSTERING_H
