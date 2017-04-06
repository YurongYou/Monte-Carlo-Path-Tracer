//
// Created by Ficos on 06/04/2017.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H


#include "../utility/Vector.h"
#include <png.h>
#include <cstdio>

// ref: http://stackoverflow.com/questions/12276675/modulus-with-negative-numbers-in-c
inline int my_mod(int a, int mod){
    return (a % mod + mod) % mod;
}

class Texture {
public:
    virtual Color get_color(float u, float v) const = 0;
    virtual ~Texture() {}
};

class PureTexture: public Texture{
public:
    PureTexture(const Color &color = WHITE) : color(color) {}

    Color get_color(float u, float v) const override {
        return color;
    }

    ~PureTexture() {}

private:
    Color color;
};

class GridTexture: public Texture {
public:
    GridTexture(const Color &c0, const Color &c1, float scale) : c0(c0), c1(c1), scale(scale) {}

    ~GridTexture(){}

    Color get_color(float u, float v) const override {
        int a0 = ((u * scale) - std::floor(u * scale)) < 0.5;
        int a1 = ((v * scale) - std::floor(v * scale)) < 0.5;
        return a0 ^ a1 ? c0 : c1;
    }

private:
    Color c0, c1;
    float scale;
};

class ImgTexture: public Texture {
public:
    // ref:: https://gist.github.com/niw/5963798, https://github.com/abcdabcd987/ray-tracing/blob/master/src/geometry.hpp#L751
    ImgTexture(const char *filename, const float& scale): scale(scale) {
        FILE *fp = fopen(filename, "rb");
        if(!fp) {
            std::cerr << "texture not found";
            exit(1);
        }
        png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if(!png) {
            std::cerr << "texture not found";
            exit(1);
        }
        png_infop info = png_create_info_struct(png);
        if(!info) {
            std::cerr << "file error";
            exit(1);
        }
        if(setjmp(png_jmpbuf(png))) {
            std::cerr << "file error";
            exit(1);
        }
        png_init_io(png, fp);
        png_read_info(png, info);
        width = static_cast<int>(png_get_image_width(png, info));
        height = static_cast<int>(png_get_image_height(png, info));
        png_byte color_type = png_get_color_type(png, info);
        png_byte bit_depth  = png_get_bit_depth(png, info);
        if(bit_depth == 16) png_set_strip_16(png);
        if(color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
        if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
        if(png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
        if(color_type == PNG_COLOR_TYPE_RGB ||
           color_type == PNG_COLOR_TYPE_GRAY ||
           color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
        if(color_type == PNG_COLOR_TYPE_GRAY ||
           color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png);
        png_read_update_info(png, info);
        png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for(int y = 0; y < height; y++)
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
        png_read_image(png, row_pointers);

        // save to Color array
        img = new Color[width * height];
        for (int y = 0, k = 0; y < height; ++y) {
            png_byte *row = row_pointers[y];
            for (int x = 0; x < width; ++x, ++k) {
                png_byte *rgba = &row[x * 4];
                img[k].x = rgba[0] / 255.0f;
                img[k].y = rgba[1] / 255.0f;
                img[k].z = rgba[2] / 255.0f;
            }
        }

        // clean up
        for (int y = 0; y < height; ++y)
            free(row_pointers[y]);
        free(row_pointers);
        fclose(fp);
    }

    ~ImgTexture(){
        delete[] img;
    }

    // ref: http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_6_Textures_Cameras_and_Speed.shtml
    Color get_color(float u, float v) const override {
        if (!img) return WHITE;
        int x = static_cast<int>(round(scale * u) - width / 2);
        int y = static_cast<int>(round(scale * v) - height / 2);
        return img[my_mod(y, height) * width + my_mod(x, width)];
    }

private:
    Color* img;
    int width;
    int height;
    float scale;
};


#endif //RAYTRACING_TEXTURE_H
