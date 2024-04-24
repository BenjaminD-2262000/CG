#include"Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	type = texType;

	int widthImg, heightImg, numColCh;
	// Flips the image so it appears right side up
	stbi_set_flip_vertically_on_load(true);


	// Reads the image from a file and stores it in binary form
	unsigned char* binImg = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generates an OpenGL texture object
	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	uint = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Configure texture resizement strategies
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// repeats the texture when the UVs are out of the range of 0 to 1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	// Check what type of color channels the texture has and load it accordingly
	GLenum format;

	if (numColCh == 4)
		format = GL_RGBA;
	else if (numColCh == 3)
		format = GL_RGB;
	else if (numColCh == 1)
		format = GL_RED;
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");

	glTexImage2D
	(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		widthImg,
		heightImg,
		0,
		format,
		GL_UNSIGNED_BYTE,
		binImg
	);

	// Generates MipMaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(binImg);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + uint);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}