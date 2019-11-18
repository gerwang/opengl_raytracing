//
// Created by gerw on 11/13/19.
//


#include <iostream>
#include "Texture.h"
#include "Config.h"

// code adapted from http://zarb.org/~gc/html/libpng.html
bool Texture::read_png_file(const char *file_name) {
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        std::cerr << "[read_png_file] File " << file_name << " could not be opened for reading" << std::endl;
        return false;
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp((png_const_bytep) header, 0, 8)) {
        std::cerr << "[read_png_file] File " << file_name << " is not recognized as a PNG file" << std::endl;
        return false;
    }


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

    if (!png_ptr) {
        std::cerr << "[read_png_file] png_create_read_struct failed" << std::endl;
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cerr << "[read_png_file] png_create_info_struct failed" << std::endl;
        return false;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "[read_png_file] Error during init_io" << std::endl;
        return false;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cerr << "[read_png_file] Error during read_image" << std::endl;
        return false;
    }

    row_pointers = (png_bytep *) malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *) malloc(png_get_rowbytes(png_ptr, info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
    return true;
}

Texture::~Texture() {
    if (row_pointers != nullptr) {
        for (int y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
        row_pointers = nullptr;
    }
}

void Texture::upload() {
    int channel = 4;
    buffer.resize(width * height * channel);
    int cnt = 0;
    for (int y = 0; y < height; y++) {
        png_byte *row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_byte *ptr = &(row[x * 4]);
            for (int c = 0; c < channel; c++) {
                buffer[cnt++] = float(ptr[c]) / 255.0f;
            }
        }
    }

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, buffer.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string &name) : name(name) {
    std::string fullPath = Config::assetRoot + "/textures/" + name;
    bool ret = read_png_file(fullPath.c_str());
    if (!ret) {
        std::cerr << "load texture failed!" << std::endl;
    } else {
        upload();
        if (glGetError() == GL_NO_ERROR) {
            loaded = true;
        } else {
            std::cerr << "OpenGL error!" << std::endl;
        }
    }
}

void Texture::use(int idx) {
    glActiveTexture(GL_TEXTURE0 + idx);
    glBindTexture(GL_TEXTURE_2D, handle);
}
