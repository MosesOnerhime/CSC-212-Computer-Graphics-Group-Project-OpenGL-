#include "Texture.h"
#include <iostream> // Include iostream for error logging

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
    : ID(0) // Initialize ID to 0
{
    type = texType;
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    if (bytes) // Check if image loading was successful
    {
        glGenTextures(1, &ID);
        glActiveTexture(slot);
        glBindTexture(texType, ID);

        glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Determine the format based on the number of color channels
        GLenum internalFormat = 0;
        GLenum dataFormat = 0;
        switch (numColCh)
        {
        case 1:
            internalFormat = dataFormat = GL_RED;
            break;
        case 2:
            internalFormat = dataFormat = GL_RG;
            break;
        case 3:
            internalFormat = dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = dataFormat = GL_RGBA;
            break;
        default:
            std::cerr << "ERROR::TEXTURE::INVALID_FORMAT: Number of color channels is " << numColCh << std::endl;
            stbi_image_free(bytes);
            return;
        }

        glTexImage2D(texType, 0, internalFormat, widthImg, heightImg, 0, dataFormat, pixelType, bytes);
        glGenerateMipmap(texType);

        stbi_image_free(bytes);
    }
    else
    {
        std::cerr << "ERROR::TEXTURE::TEXTURE_LOADING_FAILED: " << image << std::endl;
    }

    glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}

// ... [rest of your Texture class methods]
