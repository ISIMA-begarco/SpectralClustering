#include <iostream>
#include <string>

#include "CImg.h"
#include "SpectralClustering.h"
#include "KMeans.h"

using namespace cimg_library;
using namespace std;

/*******************************************************************************

                  Calcul de la fonction d'extraction d'attributs

*******************************************************************************/
CImg<> AttributsExtraction(const CImg<> & img)
{
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

void printmat(const CImg<float> & mat) {
    cimg_forXY(mat,x,y)
        cout << mat(x,y) << "\t";
}

int main(int argc, const char* argv []) {
    string file = argc > 1 ? argv[1] : "convexe.bmp";
    CImg<float> img = CImg<>(file.c_str()).channel(0);

    int ncl = argc > 2 ? atoi(argv[2]) : 2;    // Nombre de classes

    SpectralClustering algo;
    KMeans km;

    CImg<float> etiquette = algo(img,ncl);
    etiquette.normalize(0,255);

    // Affichage finale
    CImgDisplay dispImg(img,"Image originale");
    CImgDisplay dispRes(etiquette,"Image segmentee par Spectral");
    /*CImg<> ss = algo.AttributsExtraction(img);
    CImg<> tt = km(ss,ncl);
    CImgDisplay dispKme(tt.normalize(0,255),"Image segmentee par Kmeans");*/

    while (!dispImg.is_closed() && !dispRes.is_closed() /*&& !dispKme.is_closed()*/)
    {
        dispImg.wait();
        dispRes.wait();
        //dispKme.wait();
    }
    return 0;
}

