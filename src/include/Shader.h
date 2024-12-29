#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <glad/glad.h>

#ifdef SHDR_DIR
#endif 

class Shader
{
private:
	GLuint shaderProgram = -1;
public:
	Shader() = default;
	void Compile(const char* vert_source, const char* frag_source);
	void Use();
	void EndUse();
	void Delete();
	GLuint GetProgram();
};