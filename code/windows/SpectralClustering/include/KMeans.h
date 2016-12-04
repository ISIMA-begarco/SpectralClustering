#ifndef KMEANS_H
#define KMEANS_H

#include "ClusteringAlgorithm.h"

class KMeans : public ClusteringAlgorithm {
    public:
        KMeans();
        virtual ~KMeans();

        virtual CImg<unsigned char> operator()(CImg<>,int);

    protected:

    private:
        float d2(CImg<>*,CImg<>*,int,int);
        CImgList<> GetRandomCenter(CImg<> &,int);
        float GetNearestClass(CImg<> &,int,int,CImgList<> &);
        CImgList<> GetNewCenters(CImg<> &,CImg<> &,int);
        float GetError(CImg<> &,CImg<> &,CImgList<> &);
        float HasChanged(CImg<> &,CImg<> &);
        CImg<unsigned char> execute(CImg<>,int);

};

#endif // KMEANS_H
