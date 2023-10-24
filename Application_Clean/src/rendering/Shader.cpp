/*\file Shader.cpp*/
#include "rendering/shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

unsigned int Shader::shaderInUse = 0;
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* tcsPath, const char* tesPath, const char* geomPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);

	if (geomPath != nullptr)
	{
		std::ifstream gShaderFile;
		std::string geometryCode;

		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// open files
			gShaderFile.open(geomPath);
			std::stringstream gShaderStream;
			// read file's buffer contents into streams
			gShaderStream << gShaderFile.rdbuf();
			// close file handlers
			gShaderFile.close();
			// convert stream into string
			geometryCode = gShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* gShaderCode = geometryCode.c_str();
		// 2. compile shaders
		unsigned int geom;
		// geometry shader
		geom = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geom, 1, &gShaderCode, NULL);
		glCompileShader(geom);
		checkCompileErrors(geom, "GEOMETRY");

		// shader Program
		glAttachShader(ID, geom);
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(geom);
	}
	if(tcsPath != nullptr && tesPath != nullptr)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string controlCode;
		std::string evaluatorCode;

		std::ifstream tcShaderFile;
		std::ifstream teShaderFile;

		// ensure ifstream objects can throw exceptions:
		tcShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		teShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			tcShaderFile.open(tcsPath);
			teShaderFile.open(tesPath);
			std::stringstream tcShaderStream, teShaderStream;
			// read file's buffer contents into streams
			tcShaderStream << tcShaderFile.rdbuf();
			teShaderStream << teShaderFile.rdbuf();
			// close file handlers
			tcShaderFile.close();
			teShaderFile.close();
			// convert stream into string
			controlCode = tcShaderStream.str();
			evaluatorCode = teShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* tcShaderCode = controlCode.c_str();
		const char* teShaderCode = evaluatorCode.c_str();
		// 2. compile shaders
		unsigned int control, eval;
		// control shader
		control = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(control, 1, &tcShaderCode, NULL);
		glCompileShader(control);
		checkCompileErrors(control, "CONTROL");
		// eval Shader
		eval = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(eval, 1, &teShaderCode, NULL);
		glCompileShader(eval);
		checkCompileErrors(eval, "EVALUATION");

		// shader Program
		glAttachShader(ID, control);
		glAttachShader(ID, eval);
		// delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(control);
		glDeleteShader(eval);
	}

	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const char* computePath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string computeCode;
	std::ifstream cShaderFile;

	// ensure ifstream objects can throw exceptions:
	cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		// open files
		cShaderFile.open(computePath);
		std::stringstream cShaderStream;
		// read file's buffer contents into streams
		cShaderStream << cShaderFile.rdbuf();
		// close file handlers
		cShaderFile.close();
		// convert stream into string
		computeCode = cShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* cShaderCode = computeCode.c_str();

	// 2. compile shaders
	unsigned int compute;
	// vertex shader
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);
	checkCompileErrors(compute, "COMPUTE");

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, compute);
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(compute);
}

int Shader::queryMaxGroupSize(int dimension)
{
	int data;
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, dimension, &data);
	return data;
}

void Shader::dispatch(const int x, const int y, const int z)
{
	glDispatchCompute(x, y, z);
}

void Shader::setMemoryBarrier(GLbitfield barrierBit)
{
	glMemoryBarrier(barrierBit);
}

void Shader::use()
{
	//Check if the shader is already in use before rebinding it
	if(shaderInUse != ID)
	{
		shaderInUse = ID;
		glUseProgram(ID);
	}
}

// ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const
{

	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// --------------------------------------------------------------------------
void Shader::setUniformBlock(const std::string& name, unsigned int uniformBlock) const
{
	unsigned int uniformBlockIndex = glGetUniformBlockIndex(ID, name.c_str());
	glUniformBlockBinding(ID, uniformBlockIndex, uniformBlock);
}


void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}



