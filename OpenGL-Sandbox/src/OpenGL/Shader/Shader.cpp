#include "Shader.h"

#include<fstream>
#include<sstream>
#include<cassert>

#include<GLCore/Core/Log.h>

Shader::Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
	:m_programID(glCreateProgram())
{
	auto vertexSource = getShaderSource("res/shaders/" + vertexShaderFilename + ".glsl");
	auto fragmentSource = getShaderSource("res/shaders/" + fragmentShaderFilename + ".glsl");

	auto vertexShader = compileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
	auto fragmentShader = compileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

	linkProgram(vertexShader, fragmentShader);
}

Shader::Shader(Shader&& other)
	:m_programID(other.m_programID), m_uniformLocations(std::move(other.m_uniformLocations))
{
	other.m_programID = 0;
	other.m_uniformLocations.clear();
}

Shader& Shader::operator=(Shader&& rhs)
{
	if(this == &rhs)
		return;

	deleteData();

	m_programID = rhs.m_programID;
	m_uniformLocations = std::move(rhs.m_uniformLocations);

	rhs.m_programID = 0;
	rhs.m_uniformLocations.clear();

	return *this;
}

Shader::~Shader()
{
	deleteData();
}

std::string Shader::getShaderSource(const std::string& sourceFilename)
{
	std::fstream inFile;
	inFile.open(sourceFilename);
	if(!inFile.is_open())
	{
		//std::string errorMessage = "failed to open shader source file: " + sourceFilename;
		//assert(false, errorMessage.c_str());
		LOG_CRITICAL("failed to open shader source file: {}", sourceFilename);
	}

	std::stringstream buffer;
	buffer << inFile.rdbuf();
	return buffer.str();
}

GLuint Shader::compileShader(const char* shaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	int success;
	char log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if(!success)
	{
		glGetShaderInfoLog(shader, 512, nullptr, log);
		LOG_CRITICAL("failed to compile shader: {}", log);
	}
	return shader;
}

void Shader::linkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	glAttachShader(m_programID, vertexShader);
	glAttachShader(m_programID, fragmentShader);

	glLinkProgram(m_programID);

	int success;
	char log[512];
	glGetProgramiv(m_programID, GL_LINK_STATUS, &success);

	if(!success)
	{
		glGetProgramInfoLog(m_programID, 512, nullptr, log);
		LOG_CRITICAL("failed to link shader: {}", log);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLint Shader::getUniformLocation(const std::string& name)
{
	if(m_uniformLocations.find(name) != m_uniformLocations.end())
		return m_uniformLocations[name];

	GLint uniformLocation = glGetUniformLocation(m_programID, name.c_str());
	m_uniformLocations[name] = uniformLocation;
	if(uniformLocation == 1)
		LOG_WARN("uniform: {}, is invalid", name);


	return uniformLocation;
}

void Shader::deleteData()
{
	glDeleteProgram(m_programID);
	m_uniformLocations.clear();
	m_programID = 0;
}
