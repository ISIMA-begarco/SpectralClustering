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

void printmat(const CImg<float> & mat) {
    cimg_forXY(mat,x,y)
        cout << mat(x,y) << "\t";
}

int main(int argc, const char* argv []) {
    string file = argc > 1 ? argv[1] : "small.bmp";
    CImg<float> img = CImg<>(file.c_str()).channel(0);

    int ncl = argc > 2 ? atoi(argv[2]) : 2;    // Nombre de classes
    int sig = argc > 3 ? atof(argv[3]) : 25.0f;    // Nombre de classes

    SpectralClustering algo(sig);
    KMeans km;

    CImg<float> etiquette = algo(img,ncl);
    etiquette.normalize(0,255);

    file += "_res.bmp";
    etiquette.save(file.c_str());
    /*
    // Affichage finale
    CImgDisplay dispImg(img,"Image originale");
    CImgDisplay dispRes(etiquette,"Image segmentee");

    while (!dispImg.is_closed() && !dispRes.is_closed()) {
        dispImg.wait();
        dispRes.wait();
    }
    */
    return 0;
}
