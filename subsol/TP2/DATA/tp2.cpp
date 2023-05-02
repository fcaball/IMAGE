#include <stdio.h>
#include <iostream>
#include "image_ppm.h"
#include <vector>
#include <fstream>
using namespace std;

unsigned short getValue(unsigned short *img, int x, int y, int z, int dimX, int dimY, int dimZ)
{
    return (int)img[z * dimY * dimX + y * dimX + x];
}

unsigned short minElmt(unsigned short *img, size_t taille)
{
    unsigned short min = 0;
    for (size_t i = 0; i < taille; i++)
    {
        if (img[i] < min)
            min = img[i];
    }
    return min;
}

unsigned short maxElmt(unsigned short *img, size_t taille)
{
    unsigned short max = 0;
    for (size_t i = 0; i < taille; i++)
    {
        if (img[i] > max)
            max = img[i];
    }
    return max;
}

unsigned short inverse(unsigned short val)
{
    float o1 = floor(((double)val / 256.0));
    float o2 = val - o1 * 256;

    return o2 * 256 + o1;
}

vector<vector<int>> voisinage6(int i, int j, int k)
{
    vector<vector<int>> voisinage = {{i - 1, j, k}, {i + 1, j, k}, {i, j - 1, k}, {i, j + 1, k}, {i, j, k - 1}, {i, j, k + 1}};

    return voisinage;
}

int main(int argc, char const *argv[])
{
    FILE *f = fopen((char *)argv[1], "rb");
    unsigned short *contenu;

    int dimX = atoi(argv[2]);
    int dimY = atoi(argv[3]);
    int dimZ = atoi(argv[4]);

    int seuil = atoi(argv[5]);
    allocation_tableau(contenu, unsigned short, dimX *dimY *dimZ);

    size_t taille = fread(contenu, sizeof(unsigned short), dimX * dimY * dimZ, f);
    for (size_t i = 0; i < taille; i++)
    {
        contenu[i] = inverse(contenu[i]);
    }

    int voxelSizeX = 1;
    int voxelSizeY = 1;
    int voxelSizeZ = 1;
    

    ofstream file;
    file.open("modelbis.stl");
    file << "solid modelbis.stl" << endl;

    for (size_t i = 1; i < dimX - 1; i++)
    {
        for (size_t j = 1; j < dimY - 1; j++)
        {
            for (size_t k = 1; k < dimZ - 1; k++)
            {
                if (getValue(contenu, i, j, k, dimX, dimY, dimZ) < seuil)
                {
                    continue;
                }
                vector<vector<int>> vect = voisinage6(i, j, k);
                size_t size = vect.size();
                for (size_t v = 0; v < 6; v++)
                {
                    if (getValue(contenu, vect[v][0], vect[v][1], vect[v][2], dimX, dimY, dimZ) < seuil)
                    {
                        vector<float> center = {(float)vect[v][0], (float)vect[v][1], (float)vect[v][2]};
                        vector<float> zero = {(center[0] - 0.5f) * voxelSizeX, (center[1] - 0.5f) * voxelSizeY, (center[2] + 0.5f) * voxelSizeZ};
                        vector<float> un = {(center[0] + 0.5f) * voxelSizeX, (center[1] - 0.5f) * voxelSizeY, (center[2] - 0.5f) * voxelSizeZ};
                        vector<float> deux = {(center[0] + 0.5f) * voxelSizeX, (center[1] - 0.5f) * voxelSizeY, (center[2] - 0.5f) * voxelSizeZ};
                        vector<float> trois = {(center[0] - 0.5f) * voxelSizeX, (center[1] - 0.5f) * voxelSizeY, (center[2] - 0.5f) * voxelSizeZ};
                        vector<float> quatre = {(center[0] - 0.5f) * voxelSizeX, (center[1] + 0.5f) * voxelSizeY, (center[2] + 0.5f) * voxelSizeZ};
                        vector<float> cinq = {(center[0] + 0.5f) * voxelSizeX, (center[1] + 0.5f) * voxelSizeY, (center[2] + 0.5f) * voxelSizeZ};
                        vector<float> six = {(center[0] + 0.5f) * voxelSizeX, (center[1] + 0.5f) * voxelSizeY, (center[2] - 0.5f) * voxelSizeZ};
                        vector<float> sept = {(center[0] - 0.5f) * voxelSizeX, (center[1] + 0.5f) * voxelSizeY, (center[2] - 0.5f) * voxelSizeZ};
                        switch (v)
                        {
                        case 1:

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << zero[0] << " " << zero[1] << " " << zero[2] << endl;
                            file << "\t\tvertex " << trois[0] << " " << trois[1] << " " << trois[2] << endl;
                            file << "\t\tvertex " << quatre[0] << " " << quatre[1] << " " << quatre[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << trois[0] << " " << trois[1] << " " << trois[2] << endl;
                            file << "\t\tvertex " << sept[0] << " " << sept[1] << " " << sept[2] << endl;
                            file << "\t\tvertex " << quatre[0] << " " << quatre[1] << " " << quatre[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;
                            break;
                        case 2:

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << un[0] << " " << un[1] << " " << un[2] << endl;
                            file << "\t\tvertex " << deux[0] << " " << deux[1] << " " << deux[2] << endl;
                            file << "\t\tvertex " << cinq[0] << " " << cinq[1] << " " << cinq[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << deux[0] << " " << deux[1] << " " << deux[2] << endl;
                            file << "\t\tvertex " << six[0] << " " << six[1] << " " << six[2] << endl;
                            file << "\t\tvertex " << cinq[0] << " " << cinq[1] << " " << cinq[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;
                            break;
                        case 3:

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << zero[0] << " " << zero[1] << " " << zero[2] << endl;
                            file << "\t\tvertex " << un[0] << " " << un[1] << " " << un[2] << endl;
                            file << "\t\tvertex " << trois[0] << " " << trois[1] << " " << trois[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << un[0] << " " << un[1] << " " << un[2] << endl;
                            file << "\t\tvertex " << deux[0] << " " << deux[1] << " " << deux[2] << endl;
                            file << "\t\tvertex " << trois[0] << " " << trois[1] << " " << trois[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;
                            break;
                        case 4:

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << quatre[0] << " " << quatre[1] << " " << quatre[2] << endl;
                            file << "\t\tvertex " << cinq[0] << " " << cinq[1] << " " << cinq[2] << endl;
                            file << "\t\tvertex " << sept[0] << " " << sept[1] << " " << sept[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << cinq[0] << " " << cinq[1] << " " << cinq[2] << endl;
                            file << "\t\tvertex " << six[0] << " " << six[1] << " " << six[2] << endl;
                            file << "\t\tvertex " << sept[0] << " " << sept[1] << " " << sept[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;
                            break;
                        case 5:
                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << trois[0] << " " << trois[1] << " " << trois[2] << endl;
                            file << "\t\tvertex " << deux[0] << " " << deux[1] << " " << deux[2] << endl;
                            file << "\t\tvertex " << sept[0] << " " << sept[1] << " " << sept[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << deux[0] << " " << deux[1] << " " << deux[2] << endl;
                            file << "\t\tvertex " << six[0] << " " << six[1] << " " << six[2] << endl;
                            file << "\t\tvertex " << sept[0] << " " << sept[1] << " " << sept[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;
                            break;
                        case 6:
                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << zero[0] << " " << zero[1] << " " << zero[2] << endl;
                            file << "\t\tvertex " << un[0] << " " << un[1] << " " << un[2] << endl;
                            file << "\t\tvertex " << quatre[0] << " " << quatre[1] << " " << quatre[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;

                            file << "facet normal 0 0 0" << endl;
                            file << "\touter loop" << endl;
                            file << "\t\tvertex " << un[0] << " " << un[1] << " " << un[2] << endl;
                            file << "\t\tvertex " << cinq[0] << " " << cinq[1] << " " << cinq[2] << endl;
                            file << "\t\tvertex " << quatre[0] << " " << quatre[1] << " " << quatre[2] << endl;

                            file << "\tendloop" << endl;
                            file << "endfacet" << endl;
                            break;

                        default:
                            break;
                        }
                    }
                }
            }
        }
        cout << i << " / " << dimX << endl;
        ;
    }

    file << "endsolid" << endl;

    file.close();

    return 0;
}
