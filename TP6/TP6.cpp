#include <stdio.h>
#include "utilitaires.cpp"

#include <iostream>

using namespace std;

double PSNRGrey(OCTET *S, OCTET *R, int ntaille)
{
  double EQM;

  for (size_t i = 0; i < ntaille; i++)
  {
    EQM += ((S[i] - R[i]) * (S[i] - R[i]));
  }
  EQM /= ntaille;

  return 10 * log10((255 * 255) / EQM);
}

OCTET *Reconstruction_et_PSNR(OCTET *ImgIn, OCTET *BF, OCTET *BHF, OCTET *HBF, OCTET *HF, int nW, int nH, int N)
{
  OCTET *ImgOut;
  int nTaille = nW * nH;
  allocation_tableau(ImgOut, OCTET, nTaille);
  for (int i = 0; i < nH / 2; i++)
  {
    for (int j = 0; j < nW / 2; j++)
    {
      ImgOut[i * 2 * nW + j * 2] = std::min(255, std::max(0, ((HBF[i * (nW / 2) + j] - 128) / 2) + BF[i * (nW / 2) + j] + ((BHF[i * (nW / 2) + j] - 128) / 2) + ((HF[i * (nW / 2) + j] - 128) / 4)));

      ImgOut[i * 2 * nW + j * 2 + 1] = std::min(255, std::max(0, BF[i * (nW / 2) + j] + ((HBF[i * (nW / 2) + j] - 128) / 2) - ((BHF[i * (nW / 2) + j] - 128) / 2) - ((HF[i * (nW / 2) + j] - 128) / 4)));

      ImgOut[i * 2 * nW + j * 2 + nW] = std::min(255, std::max(0, BF[i * (nW / 2) + j] - ((HBF[i * (nW / 2) + j] - 128) / 2) + ((BHF[i * (nW / 2) + j] - 128) / 2) - ((HF[i * (nW / 2) + j] - 128) / 4)));

      ImgOut[i * 2 * nW + j * 2 + nW + 1] = std::min(255, std::max(0, ((HF[i * (nW / 2) + j] - 128) / 4) + BF[i * (nW / 2) + j] - ((HBF[i * (nW / 2) + j] - 128) / 2) - ((BHF[i * (nW / 2) + j] - 128) / 2)));
    }
  }

  ecrire_image_pgm((char *)"ReconstruiteQuantificationCoeff1_4_4_16.pgm", ImgOut, nH, nW);

  cout << 7 - N << " : " << PSNRGrey(ImgIn, ImgOut, nTaille) << endl;
  return ImgOut;
}

void UneImageEnQuatre(OCTET *ImgIn, OCTET *&ImgHautGauche, OCTET *&ImgHautDroite, OCTET *&ImgBasGauche, OCTET *&ImgBasDroite, int nW, int nH)
{

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgHautGauche[i * (nW / 2) + j] = ImgIn[i * nW + j];
    }
  }

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgHautDroite[i * (nW / 2) + j] = ImgIn[(i)*nW + j + nW / 2];
    }
  }

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgBasGauche[i * (nW / 2) + j] = ImgIn[(i + nW / 2) * nW + j];
    }
  }

  for (size_t i = 0; i < nW / 2; i++)
  {
    for (size_t j = 0; j < nH / 2; j++)
    {
      ImgBasDroite[i * (nW / 2) + j] = ImgIn[(i + nW / 2) * nW + j + nW / 2];
    }
  }
}

OCTET *QuatresImagesEnUne(OCTET *ImgHautGauche, OCTET *ImgHautDroite, OCTET *ImgBasGauche, OCTET *ImgBasDroite, int WFinale, int HFinale)
{

  OCTET *ImgOut;
  allocation_tableau(ImgOut, OCTET, WFinale * HFinale);

  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[i * WFinale + j] = ImgHautGauche[i * (WFinale / 2) + j];
    }
  }
  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[(i)*WFinale + j + WFinale / 2] = ImgHautDroite[i * (WFinale / 2) + j];
    }
  }

  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[(i + WFinale / 2) * WFinale + j] = ImgBasGauche[i * (WFinale / 2) + j];
    }
  }
  for (size_t i = 0; i < WFinale / 2; i++)
  {
    for (size_t j = 0; j < HFinale / 2; j++)
    {
      ImgOut[(i + WFinale / 2) * WFinale + j + WFinale / 2] = ImgBasDroite[i * (WFinale / 2) + j];
    }
  }
  return ImgOut;
}

OCTET *fusionParAdjacence(OCTET *&ImgIn, int w, int h, int seuil)
{

  for (size_t i = 0; i < h; i++)
  {
    for (size_t j = 0; j < w; j++)
    {
      int minVois = INT32_MAX;
      for (int ip = i - 1; ip < i + 1; ip++)
        for (int jp = j - 1; jp < j + 1; jp++)
        {
          if (ip < 0 || ip >= w || jp < 0 || jp >= h)
          {
            continue;
          }
          else
          {
            if (ImgIn[ip * w + jp] < minVois)
            {
              minVois = ImgIn[ip * w + jp];
            }
          }
        }
      if (minVois!=INT32_MAX && abs(minVois - ImgIn[i * w + j]) <= seuil)
      {
        ImgIn[i * w + j]=minVois;
      }
    }
  }
}

OCTET *divisionRecursive(OCTET *ImgIn, float Seuil, int w, int h)
{
  OCTET *Quart1, *Quart2, *Quart3, *Quart4, *Quart1bis, *Quart2bis, *Quart3bis, *Quart4bis;
  allocation_tableau(Quart1, OCTET, (w * h) / 4);
  allocation_tableau(Quart1bis, OCTET, (w * h) / 4);
  allocation_tableau(Quart2, OCTET, (w * h) / 4);
  allocation_tableau(Quart2bis, OCTET, (w * h) / 4);
  allocation_tableau(Quart3, OCTET, (w * h) / 4);
  allocation_tableau(Quart3bis, OCTET, (w * h) / 4);
  allocation_tableau(Quart4, OCTET, (w * h) / 4);
  allocation_tableau(Quart4bis, OCTET, (w * h) / 4);

  UneImageEnQuatre(ImgIn, Quart1, Quart2, Quart3, Quart4, w, h);

  float moy1 = 0;
  float moy2 = 0;
  float moy3 = 0;
  float moy4 = 0;
  for (size_t i = 0; i < (w * h) / 4; i++)
  {
    moy1 += Quart1[i];
    moy2 += Quart2[i];
    moy3 += Quart3[i];
    moy4 += Quart4[i];
  }
  moy1 /= ((w * h) / 4);
  moy2 /= ((w * h) / 4);
  moy3 /= ((w * h) / 4);
  moy4 /= ((w * h) / 4);

  float variance1 = 0, variance2 = 0, variance3 = 0, variance4 = 0;

  for (size_t i = 0; i < (w * h) / 4; i++)
  {
    variance1 += ((Quart1[i] - moy1) * (Quart1[i] - moy1));
    Quart1bis[i] = moy1;
    variance2 += ((Quart2[i] - moy2) * (Quart2[i] - moy2));
    Quart2bis[i] = moy2;
    variance3 += ((Quart3[i] - moy3) * (Quart3[i] - moy3));
    Quart3bis[i] = moy3;
    variance4 += ((Quart4[i] - moy4) * (Quart4[i] - moy4));
    Quart4bis[i] = moy4;
  }

  variance1 /= ((w * h) / 4);
  variance2 /= ((w * h) / 4);
  variance3 /= ((w * h) / 4);
  variance4 /= ((w * h) / 4);

  if (variance1 > Seuil && (w * h / 4) > 16)
  {
    Quart1bis = divisionRecursive(Quart1, Seuil, w / 2, h / 2);
  }

  if (variance2 > Seuil && (w * h / 4) > 16)
  {
    Quart2bis = divisionRecursive(Quart2, Seuil, w / 2, h / 2);
  }
  if (variance3 > Seuil && (w * h / 4) > 16)
  {
    Quart3bis = divisionRecursive(Quart3, Seuil, w / 2, h / 2);
  }
  if (variance4 > Seuil && (w * h / 4) > 16)
  {
    Quart4bis = divisionRecursive(Quart4, Seuil, w / 2, h / 2);
  }

  cout << "M1=" << moy1 << " V1=" << variance1 << " M2=" << moy2 << " V2=" << variance2 << " M3=" << moy3 << " V3=" << variance3 << " M4=" << moy4 << " V4=" << variance4 << endl;

  return QuatresImagesEnUne(Quart1bis, Quart2bis, Quart3bis, Quart4bis, w, h);
}

int main(int argc, char *argv[])
{
  char cNomImgLue[250];
  int h, w;

  if (argc != 3)
  {
    printf("Usage: ImageIn.pgm ImageSortie.pgm\n");
    exit(1);
  }
  sscanf(argv[1], "%s", cNomImgLue);

  OCTET *ImgIn, *ImgOut;

  lire_nb_lignes_colonnes_image_pgm(cNomImgLue, &h, &w);
  int nTaille = h * w;

  allocation_tableau(ImgIn, OCTET, nTaille);
  allocation_tableau(ImgOut, OCTET, nTaille);
  lire_image_pgm(cNomImgLue, ImgIn, h * w);

  // ImgOut = divisionRecursive(ImgIn, 25, w, h);

  fusionParAdjacence(ImgIn, w, h, 8);

  // ImgOut=QuatresImagesEnUne(Quart1,Quart2,Quart3,Quart4,w,h);

  ecrire_image_pgm((char *)argv[2], ImgIn, h, w);

  return 0;
}
