#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	// The program ID
	GLuint Program;
	//Contructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		//Retrieve the vertex/fragment source code from filepath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Ensures ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			//open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//Read the file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//close file handlers
			vShaderFile.close();
			fShaderFile.close();

			//convert streams into GLchar array
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();


		//Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		//Vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);

		//print compile errors if any
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}


		//Fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		//print compile errors if any
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}



		//shader program
		this->Program = glCreateProgram();
		glAttachShader(this->Program, vertex);
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program);

		//print linking errors if any
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		//Delete the shaders as they're linked
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}	
	//use the program
	void Use() { glUseProgram(this->Program); }
};

#endif