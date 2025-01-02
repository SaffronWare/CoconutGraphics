#define STB_IMAGE_IMPLEMENTATION
#include "HDRloader.h"


void LoadHDR(GLuint* textureID, const char* path)
{
	int width, height, channels;
	float* data = stbi_loadf(path, &width, &height, &channels, 0);

	if (!data) {
		std::cerr << "Failed to load HDR image: " << stbi_failure_reason() << std::endl;
		return;
	}



	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB32F, 
        width,
        height,
        0, 
        (channels == 3) ? GL_RGB : GL_RGBA, 
        GL_FLOAT,
        data
    );

    

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, 0);
}


