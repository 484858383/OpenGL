#pragma once

#include<string>
#include<glad/glad.h>

class Shader
{
public:
	Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	~Shader();

	GLuint getProgramID() const { return m_programID; }
	
	void bind() const { glUseProgram(m_programID); }
	void unbind() const { glUseProgram(0); }
private:
	std::string getShaderSource(const std::string& sourceFilename);
	GLuint compileShader(const char* shaderSource, GLenum shaderType);
	void linkProgram(GLuint vertexShader, GLuint fragmentShader);
private:
	GLuint m_programID;
};