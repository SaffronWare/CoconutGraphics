#include "Shader.h"


// code for this function taken from https://github.com/VictorGordan/opengl-tutorials/blob/main/YoutubeOpenGL%204%20-%20Organizing
std::string get_file_contents(const char* filename)
{
	
	std::string actual_path = filename;
	std::ifstream in(actual_path, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}


void Shader::Compile(const char* vert_source, const char* frag_source)
{
	std::string vertex_shader_text = get_file_contents(vert_source);
	std::string fragment_shader_text = get_file_contents(frag_source);

	const char* vertex_shader_source = vertex_shader_text.c_str();
	const char* fragment_shader_source = fragment_shader_text.c_str();

	
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, GL_NONE);
	glCompileShader(vertex_shader);


	

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, GL_NONE);
	glCompileShader(fragment_shader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex_shader);
	glAttachShader(shaderProgram, fragment_shader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	
}

void Shader::Use()
{
	glUseProgram(shaderProgram);
}

void Shader::EndUse()
{
	glUseProgram(0);
}

void Shader::Delete()
{
	glDeleteProgram(shaderProgram);
}

GLuint Shader::GetProgram()
{
	return shaderProgram;
}