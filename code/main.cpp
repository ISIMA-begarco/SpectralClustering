#include <iostream>
#include <string>

#include "CImg.h"
#include "SpectralClustering.h"

using namespace cimg_library;
using namespace std;

int main(int argc, const char* argv []) {
    string file = argc > 1 ? argv[1] : "convexe.bmp";
    CImg<float> img = CImg<>(file.c_str()).channel(0);

    int ncl = argc > 2 ? atoi(argv[2]) : 2;    // Nombre de classes
    int sig = argc > 3 ? atof(argv[3]) : 25.0f;    // Sigma

    SpectralClustering algo(sig);

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
