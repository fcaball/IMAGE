#include "utilitaires.cpp"
#include <iostream>

using namespace std;

float RadToDeg(float angleRad){
    float angleDeg = (float)(angleRad*180/M_PI) ;
    while(angleDeg < 0){
        angleDeg += 360 ;
    }
    return angleDeg ;
}

int getNeighbor(float angle, int i, int j, int nW, int nH)
{
    if(-M_PI/6 <= angle        && angle < M_PI/6)    return i*nW+(j+1);
    else if( M_PI/6 <= angle   && angle < M_PI/3)    return (i+1)*nW+(j+1);
    else if( M_PI/3 <= angle   && angle < 2*M_PI/3)  return (i+1)*nW+j;
    else if(2*M_PI/3 <= angle  && angle < 5*M_PI/6)  return (i+1)*nW+(j-1);
    else if(-M_PI/3 <= angle   && angle < -M_PI/6)   return (i-1)*nW+(j+1);
    else if(-2*M_PI/3 <= angle && angle < -M_PI/3)   return (i-1)*nW+j;
    else if(-5*M_PI/6 <= angle && angle < -2*M_PI/3) return (i-1)*nW+(j-1);
    else                                             return i*nW+(j-1);
}

int main(int argc, char const *argv[])
{
    OCTET *ImgIn/* , *ImgOut */, *ImgTemp, *ImgGrad;

    int w, h;
    // lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    int ntaille = w * h;
    allocation_tableau(ImgIn, OCTET, ntaille);
    // allocation_tableau(ImgOut, OCTET, ntaille);
    vector<int> ImgOut(ntaille);
    allocation_tableau(ImgTemp, OCTET, ntaille);
    allocation_tableau(ImgGrad, OCTET, ntaille);
    lire_image_pgm((char *)argv[1], ImgIn, ntaille);
    // lire_image_pgm((char *)argv[3], ImgGrad, ntaille);

    // for (size_t i = 1; i < h - 1; i++)
    // {
    //     for (size_t j = 1; j < w - 1; j++)
    //     {
    //         int DeltaX = ImgIn[i * w + j + 1] - ImgIn[i * w + j];
    //         int DeltaY = ImgIn[(i + 1) * w + j] - ImgIn[i * w + j];
    //         ImgGrad[i * w + j] = sqrt(DeltaX * DeltaX + DeltaY * DeltaY);
    //     }
    // }

    for (size_t i = 1; i < h - 1; i++)
    {
        for (size_t j = 1; j < w - 1; j++)
        {
            int v=4 * (int)ImgIn[i * w + j] - (int)ImgIn[(i + 1) * w + j] - (int)ImgIn[(i - 1) * w + j] - (int)ImgIn[i * w + j + 1] - (int)ImgIn[i * w + j - 1];
            ImgOut[i * w + j] = v;
        }
    }

    for (size_t i = 1; i < h - 1; i++)
    {
        for (size_t j = 1; j < w - 1; j++)
        {
            float D;
            float Ai=((float) ImgOut[(i + 1) * w + j] - (float) ImgOut[i * w + j]);
            float Aj=((float) ImgOut[i * w + (j + 1)] - (float) ImgOut[i * w + j]);
            float G=sqrt(Ai*Ai+Aj*Aj);
            D = (float) ((float) Ai / (float)Aj);
            D = atan((float) D);
            cout<<Ai<<" "<<Aj<<endl;

            if(ImgOut[i*w+j]>0 && ImgOut[getNeighbor(D,i,j,w,h)]<0 || ImgOut[i*w+j]<0 && ImgOut[getNeighbor(D,i,j,w,h)]>0 )
                ImgTemp[i*w+j]=G;
        }
    }

    // ImgOut=seuillageGrey(ImgOut,ntaille,20);
    ImgTemp=seuilHisteresisGrey(ImgTemp,10,15,w,h);
    // profilGrey(ImgIn,w,h,false,200);

    ecrire_image_pgm((char *)argv[2], ImgTemp, h, w);

    return 0;
}
