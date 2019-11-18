//
// Created by gerw on 11/13/19.
//

#ifndef OPENGL_RAYTRACING_TEXTURE_H
#define OPENGL_RAYTRACING_TEXTURE_H


#include <string>
#include <png.h>
#include <glad/glad.h>
#include <vector>

class Texture {
public:
    explicit Texture(const std::string &name);

    virtual ~Texture();

    void use(int idx);

private:

    bool read_png_file(const char *file_name);

    void upload();

    std::string name;
    bool loaded = false;

    GLuint handle{};
    std::vector<float> buffer;

    png_structp png_ptr{};
    png_infop info_ptr{};
    int width{}, height{};
    png_byte color_type{};
    png_byte bit_depth{};
    int number_of_passes{};
    png_bytep *row_pointers{};
};


#endif //OPENGL_RAYTRACING_TEXTURE_H
