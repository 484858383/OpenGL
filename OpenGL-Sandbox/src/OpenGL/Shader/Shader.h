#pragma once

#include<string>
#include<unordered_map>
#include<glad/glad.h>

class Shader
{
public:
	Shader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& rhs) = delete;

	Shader(Shader&& other);
	Shader& operator=(Shader&& rhs);

	~Shader();

	GLuint getProgramID() const { return m_programID; }
	
	void bind() const { glUseProgram(m_programID); }
	void unbind() const { glUseProgram(0); }


	template<typename... Ts>
	void loadUniform(const std::string& name, Ts&&... args)
	{
		if(typeid(std::common_type_t<Ts...>) == typeid(int))
		{
			if constexpr(sizeof...(Ts) == 1)
				glUniform1i(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 2)
				glUniform2i(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 3)
				glUniform3i(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 4)
				glUniform4i(getUniformLocation(name), std::forward<Ts>(args)...);
		}
		else if(typeid(std::common_type_t<Ts...>) == typeid(float))
		{
			if constexpr(sizeof...(Ts) == 1)
				glUniform1f(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 2)
				glUniform2f(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 3)
				glUniform3f(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 4)
				glUniform4f(getUniformLocation(name), std::forward<Ts>(args)...);
		}
		else if(typeid(std::common_type_t<Ts...>) == typeid(unsigned))
		{
			if constexpr(sizeof...(Ts) == 1)
				glUniform1ui(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 2)
				glUniform2ui(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 3)
				glUniform3ui(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 4)
				glUniform4ui(getUniformLocation(name), std::forward<Ts>(args)...);
		}
		else if(typeid(std::common_type_t<Ts...>) == typeid(bool))
		{
			if constexpr(sizeof...(Ts) == 1)
				glUniform1i(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 2)
				glUniform2i(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 3)
				glUniform3i(getUniformLocation(name), std::forward<Ts>(args)...);
			if constexpr(sizeof...(Ts) == 4)
				glUniform4i(getUniformLocation(name), std::forward<Ts>(args)...);
		}
	}
private:
	std::string getShaderSource(const std::string& sourceFilename);
	GLuint compileShader(const char* shaderSource, GLenum shaderType);
	void linkProgram(GLuint vertexShader, GLuint fragmentShader);

	GLint getUniformLocation(const std::string& name);
	void deleteData();
private:
	GLuint m_programID;
	std::unordered_map<std::string, GLint> m_uniformLocations;
};