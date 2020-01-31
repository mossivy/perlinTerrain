#include <iostream>
#include <random>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <time.h>
#include <stdlib.h>

//based on javidx9 lerp method of perlin

/*TODO:
*
*/

  int w = 512;
  int h = 512;
  int aSize = w*h;
  float tempr;
  float *aNoise = nullptr;
  float *aPerlinNoise = nullptr;
  float *terrainz = nullptr;


  float *noiseGen(){

    aNoise = new float[aSize];

    for(int i = 0; i < aSize; i++){
      aNoise[i] = (float)rand()/(RAND_MAX);
    }

    return aNoise;
  }

  float *perlinNoiseGen(float *randAr, int oct, float scaleF){


    aPerlinNoise = new float[aSize];

    for(int x = 0; x < w; x++){
      for(int y = 0; y < h; y++){
        float scale = 1;
        float noiseBoi = 0;
        float scaleCount = 0;

        for(int i = 0; i < oct; i++){

          int counter = w >> oct;
          int x1 = (x/counter)*counter;
          int x2 = (x1+counter)%w;
          int y1 = (y/counter)*counter;
          int y2 = (y1+counter)%w;

          float xblend = (float)(x - x1)/(float)counter;
          float yblend = (float)(y - y1)/(float)counter;
          float top = (1-xblend)*randAr[y1*w+x1]+xblend*randAr[y1*w+x2];
          float bottom = (1-xblend)*randAr[y2*w+x1]+xblend*randAr[y2*w+x2];



          scaleCount += scale;
          noiseBoi += scale*(yblend*(bottom-top)+top);
          scale = scale/scaleF;
          }


      aPerlinNoise[(y*w)+x] = noiseBoi/scaleCount;

      }

    }

    return aPerlinNoise;
  }

  float *perlinAdd(int width, int height, float *noise, float *perlin1, float *perlin2, float *perlin3, float *perlin4, float *perlin5, float *perlin6, float *perlin7, float *perlin8, float sca, float sca2, float sca3, float sca4, float sca5, float sca6, float sca7, float sca8){

    terrainz = new float[aSize];

    for(int x = 0; x<w; x++){
      for(int y = 0; y<h; y++){

        terrainz[(y*w)+x] += perlin1[(y*w)+x];
        terrainz[(y*w)+x] += perlin2[(y*w)+x]*sca;
        terrainz[(y*w)+x] += perlin3[(y*w)+x]*sca2;
        terrainz[(y*w)+x] += perlin4[(y*w)+x]*sca3;
        terrainz[(y*w)+x] += perlin5[(y*w)+x]*sca4;
        terrainz[(y*w)+x] += perlin6[(y*w)+x]*sca5;
        terrainz[(y*w)+x] += perlin7[(y*w)+x]*sca6;
        terrainz[(y*w)+x] += perlin8[(y*w)+x]*sca7;
        terrainz[(y*w)+x] += noise[y*w+x]*sca8;

      }
    }
    return terrainz;
  }

  void plyGen(float slope, float cutoff, float zscale, int width, int height, float *zter){
    float max = 0;
    float min = 1000;
    std::ofstream ply;
    ply.open("/home/moss/development/terrainGen/output/output.ply");
    //ply<<"ply\nformat ascii 1.0\nelement vertex "<<aSize<<"\nproperty float x\nproperty float y\nproperty float z\nproperty uchar red\nproperty uchar green\nproperty uchar blue"<<"\nend_header";
    ply<<"ply\nformat ascii 1.0\ncomment TextureFile Ice-0100.jpg\ncomment TextureFile snowtogravel1.jpg\ncomment TextureFile snowtogravel2.jpg\ncomment TextureFile snowtogravel3.jpg\ncomment TextureFile snowtogravel4.jpg\ncomment TextureFile snowtogravel5.jpg\ncomment TextureFile snowtogravel6.jpg\ncomment TextureFile Gravel-2408c.jpg\nelement vertex "<<aSize<<"\nproperty float x\nproperty float y\nproperty float z\nelement face "<<((w-1)*(w-1))*2<<"\nproperty list uchar int vertex_indices\nproperty list uchar float texcoord\nproperty int texnumber\nend_header";
    for(int y = 0; y<h; y++){
      for(int x = 0; x<w; x++){
        if(zter[(y*w)+x]>max){
          max = zter[(y*w)+x];
        }
        if(zter[(y*w)+x]<min){
          min = zter[(y*w)+x];
        }
      }
    }
    std::cout<<"min "<<min*zscale<<" max "<<max*zscale<<std::endl;
    for(int y = 0; y<h; y++){
      for(int x = 0; x<w; x++){
        zter[(y*w)+x] *= -1;
        zter[(y*w)+x] += max;
        ply<<"\n"<<x<<" "<<y<<" "<<zter[(y*w)+x]*zscale;
        // if(zter[(y*w)+x]*zscale > 160){
        //   ply<<" 255 255 255";
        // }
        // else{
        //   ply<<" 0 0 0";
        // }
      }
    }
    for(int y = 0; y<h; y++){
      for(int x = 0; x<w; x++){
        if((!(x == w-1))&&(!(y == h-1))){
          if(-1*(zter[(y*w)+x] - max)*zscale>zscale*max*cutoff){
            ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 0";
            ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 0";
          }
          else{
            if(((-1*(zter[(y*w)+x+1] - max)*zscale>zscale*max*cutoff)||(-1*(zter[((y+1)*w)+x] - max)*zscale>zscale*max*cutoff)||(-1*(zter[((y+1)*w)+x+1] - max)*zscale>zscale*max*cutoff))&&((float)rand()/(RAND_MAX)>.7)){
              ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 0";

              if(((float)rand()/(RAND_MAX))>.4){
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 0";
              }

              else{
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 1";
              }

            }

            else{
              if((-1*(zter[(y*w)+x] - max)*zscale>zscale*max*(cutoff-0.02))){
                ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 1";
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 1";
              }
              if((-1*(zter[(y*w)+x] - max)*zscale>zscale*max*(cutoff-0.03))&&(-1*(zter[(y*w)+x] - max)*zscale<zscale*max*(cutoff-0.02))){
                ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 2";
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 2";
              }
              if((-1*(zter[(y*w)+x] - max)*zscale>zscale*max*(cutoff-0.04))&&(-1*(zter[(y*w)+x] - max)*zscale<zscale*max*(cutoff-0.03))){
                ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 3";
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 3";
              }
              if((-1*(zter[(y*w)+x] - max)*zscale>zscale*max*(cutoff-0.05))&&(-1*(zter[(y*w)+x] - max)*zscale<zscale*max*(cutoff-0.04))){
                ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 4";
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 4";
              }
              if((-1*(zter[(y*w)+x] - max)*zscale>zscale*max*(cutoff-0.06))&&(-1*(zter[(y*w)+x] - max)*zscale<zscale*max*(cutoff-0.05))){
                ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 5";
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 5";
              }
              if((-1*(zter[(y*w)+x] - max)*zscale>zscale*max*(cutoff-0.075))&&(-1*(zter[(y*w)+x] - max)*zscale<zscale*max*(cutoff-0.06))){
                ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 6";
                ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 6";
              }
              else{
              ply<<"\n3 "<<(y*w)+x<<" "<<((y+1)*w)+x<<" "<<((y+1)*w)+x+1<<" 6 0 0 0 1 1 1 7";
              ply<<"\n3 "<<((y+1)*w)+x+1<<" "<<(y*w)+x+1<<" "<<(y*w)+x<<" 6 0 0 0 1 1 1 7";
              }
            }
          }
        }
      }
    }
  }


int main(){
  float ScalingF = 2;
  float zscale = 30;
  float cutoff = .7;
  float slope = 1.8;
  float sca;
  float sca2;
  float sca3;
  float sca4;
  float sca5;
  float sca6;
  float sca7;
  float sca8;
  srand(time(0));

  std::cout<<"oct 2:\n";
  std::cin >> sca;
  std::cout<<"oct 3:\n";
  std::cin >> sca2;
  std::cout<<"oct: 4\n";
  std::cin >> sca3;
  std::cout<<"oct: 5\n";
    std::cin >> sca4;
  std::cout<<"oct: 6\n";
    std::cin >> sca5;
  std::cout<<"oct: 7\n";
    std::cin >> sca6;
  std::cout<<"oct: 8\n";
    std::cin >> sca7;
  std::cout<<"noise: \n";
    std::cin >> sca8;

  float *noise = noiseGen();
  float *perlin1 = perlinNoiseGen(noise, 1, ScalingF);
  float *perlin2 = perlinNoiseGen(noise, 2, ScalingF);
  float *perlin3 = perlinNoiseGen(noise, 3, ScalingF);
  float *perlin4 = perlinNoiseGen(noise, 4, ScalingF);
  float *perlin5 = perlinNoiseGen(noise, 5, ScalingF);
  float *perlin6 = perlinNoiseGen(noise, 6, ScalingF);
  float *perlin7 = perlinNoiseGen(noise, 7, ScalingF);
  float *perlin8 = perlinNoiseGen(noise, 8, ScalingF);


  float *zter = perlinAdd(w, h, noise, perlin1, perlin2, perlin3, perlin4, perlin5, perlin6, perlin7, perlin8, sca, sca2, sca3, sca4, sca5, sca6, sca7, sca8);
  plyGen(slope, cutoff, zscale, w, h, zter);


  // std::ofstream img("tempr = perlin[(y*w)+x]*254;tempr = perlin[(y*w)+x]*254;tempr = perlin[(y*w)+x]*254;perlin.ppm");
  //  img << "P3" << std::endl;
  //  img << w << " " << h  << std::endl;
  //  img<<"512"<< std::endl;
  //
  //
  //  for(int x = 0; x < w; x++){
  //    for(int y = 0; y < h; y++){
  //
  //      tempr = zter[(y*w)+x]*254;
  //
  //      img<<(int)tempr<<" "<<(int)tempr<<" "<<(int)tempr<<std::endl;
  //    }
  //  }
  //  system("xdg-open perlin.ppm");
}
