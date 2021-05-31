#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "vendor/glm/glm/glm.hpp"


class Shader
{
	unsigned int m_ID;

public:
	Shader(const char* vertexPath, const char* fragmentPath) 
	{
		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;
		
		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);

			std::stringstream vertexShaderStream, fragmentShaderStream;
			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			vertexShaderFile.close();
			fragmentShaderFile.close();

			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::puts("ERROR::SHADER::FAIL_NOT_SUCCESFULLY_READ");
		}

		const char* vertexShaderCode = vertexCode.c_str();
		const char* fragmentShaderCode = fragmentCode.c_str();

		unsigned vertexID, fragmentID;

		vertexID = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexID, 1, &vertexShaderCode, nullptr);
		glCompileShader(vertexID);
		checkCompileErrors(vertexID, "VERTEX");

		fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentID, 1, &fragmentShaderCode, nullptr);
		glCompileShader(fragmentID);
		checkCompileErrors(fragmentID, "FRAGMENT");

		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexID);
		glAttachShader(m_ID, fragmentID);
		glLinkProgram(m_ID);
		checkCompileErrors(m_ID, "PROGRAM");

		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);
	}
	void use() const
	{
		glUseProgram(m_ID);
	}
private:

	void checkCompileErrors(unsigned ID, const std::string& type) const
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(ID, 1024, nullptr, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION::ERROR of type: " << type << '\n' <<
					infoLog << '\n' << std::string(50, '-') << '\n';
			}
		}
		else
		{
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID, 1024, nullptr, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << '\n' <<
					infoLog << '\n' << std::string(50, '-') << '\n';
			}
		}
	}
public:
	void setMat4(const std::string & name, const glm::mat4& matrix) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}
	void setVec3(const std::string& name, const glm::vec3& vector) const
	{
		glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &vector[0]);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

};