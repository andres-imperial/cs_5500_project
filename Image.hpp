#include <cstdlib> 
#include <cstdio> 
#include <fstream>
#include <vector>
#include <string.h>

#ifndef IMAGE_HPP
#define IMAGE_HPP
 
class Image 
{ 
public: 
    // Rgb structure, i.e. a pixel 
    struct Rgb
    { 
        float r;
        float g;
        float b;
        Rgb();
        Rgb(float c);
        Rgb(float _r, float _g, float _b);
        bool operator != (const Rgb &c) const;
        Rgb& operator *= (const Rgb &rgb);
        Rgb operator * (const Rgb &rgb) const;
        Rgb& operator += (const Rgb &rgb);
        Rgb operator+(const Image::Rgb &rgb);
        friend float& operator += (float &f, const Rgb rgb);
    };

    unsigned int w, h; // Image resolution
    Rgb *pixels; // 1D array of pixels
    static const Rgb kBlack, kWhite, kRed, kGreen, kBlue; // Preset colors
    static const std::vector<Rgb> COLORS;
    Image();
    Image(const Image &img);
    Image(const Image &img, const Rgb &c);
    Image(const unsigned int &_w, const unsigned int &_h,
          const Rgb &c = Image::kBlack);
    const Rgb &operator[](const unsigned int &i) const;
    Rgb &operator[](const unsigned int &i);
    Rgb operator()(int column, int row) const;
    Rgb &operator()(int column, int row);
    unsigned int getWidth() const;
    unsigned int getHeight() const;
    bool pixelInView(int column, int row) const;
    ~Image();
};

Image readPPM(const char *filename);
void savePPM(const Image &img, const char *filename);
void grayScale(Image &img);

#endif
