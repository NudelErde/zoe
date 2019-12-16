/*
 * OpenGLShaderImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

#include "OpenGLShaderImpl.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "File.h"
#include "../../zoe/render/GraphicsContext.h"
#include "../../zoe/math/mat.h"

namespace Zoe {

struct layoutLocation {
	unsigned int location;
	std::string varName;
};

struct ShaderSource {
	std::string vertexShader;
	std::string fragmentShader;
	std::vector<layoutLocation> vertexLayoutLocation;
	std::vector<layoutLocation> fragmentLayoutLocation;
};

static ShaderSource parseShader(const File& file) {
	std::istream* stream = file.getInputStream();
	ShaderSource shso;
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::VERTEX;
	std::string line;
	std::stringstream ss[2];
	unsigned int index = 0;
	while (getline(*stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		} else if ((line.find("layout(location=")) != std::string::npos) {
			index = line.find("layout(location=");
			int substringStart = 0;
			const char* str = line.c_str();
			layoutLocation ll;
			bool start = false;
			std::stringstream strStream;
			while (true) {
				if (str[index] <= '9' && str[index] >= '0') {
					start = true;
					strStream << str[index];
				} else if (start) {
					break;
				}
				index++;
			}
			std::istringstream(strStream.str()) >> ll.location;

			int spaces = 0;
			bool layoutClosed = false;
			std::stringstream strStream2;
			while (index < line.length()) {
				if (str[index] == ';') {
					break;
				} else if (str[index] == ')') {
					layoutClosed = true;
					substringStart = index;
				} else if (layoutClosed && str[index] == ' ') {
					spaces++;
				} else if (spaces == 3) {
					strStream2 << str[index];
				}
				index++;
			}

			ll.varName = strStream2.str();
			if (type == ShaderType::VERTEX) {
				shso.vertexLayoutLocation.push_back(ll);
			} else {
				shso.fragmentLayoutLocation.push_back(ll);
			}
			ss[(int) type] << line.substr(substringStart + 1);
		} else {
			ss[(int) type] << line << '\n';
		}
	}
	shso.fragmentShader = ss[1].str();
	shso.vertexShader = ss[0].str();
	return shso;
}

static unsigned int compileShader(unsigned int type,
		const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*) alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile "
				<< (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
				<< " shader! " << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShaderProgram(ShaderSource& ss) {
	unsigned int prg = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, ss.vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, ss.fragmentShader);

	glAttachShader(prg, vs);
	glAttachShader(prg, fs);

	for (layoutLocation& ll : ss.vertexLayoutLocation) {
		glBindAttribLocation(prg, ll.location, ll.varName.c_str());
	}
	for (layoutLocation& ll : ss.fragmentLayoutLocation) {
		glBindAttribLocation(prg, ll.location, ll.varName.c_str());
	}

	glLinkProgram(prg);
	glValidateProgram(prg);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return prg;
}

OpenGLShaderImpl::OpenGLShaderImpl(const File& file, GraphicsContext* context) :
		ShaderImpl(context) {
	ShaderSource ss = parseShader(file);
	renderID = createShaderProgram(ss);
}

OpenGLShaderImpl::~OpenGLShaderImpl() {
	glDeleteShader(renderID);
}

void OpenGLShaderImpl::setUniform1f(const std::string& name, float v0) {
	bind();
	glUniform1f(glGetUniformLocation(renderID, name.c_str()), v0);
}
void OpenGLShaderImpl::setUniform2f(const std::string& name, float v0,
		float v1) {
	bind();
	glUniform2f(glGetUniformLocation(renderID, name.c_str()), v0, v1);
}
void OpenGLShaderImpl::setUniform3f(const std::string& name, float v0, float v1,
		float v2) {
	bind();
	glUniform3f(glGetUniformLocation(renderID, name.c_str()), v0, v1, v2);
}
void OpenGLShaderImpl::setUniform4f(const std::string& name, float v0, float v1,
		float v2, float v3) {
	bind();
	glUniform4f(glGetUniformLocation(renderID, name.c_str()), v0, v1, v2, v3);
}

void OpenGLShaderImpl::setUniform2m(const std::string& name,
		const mat2x2& mat) {
	bind();
	glUniformMatrix2fv(glGetUniformLocation(renderID, name.c_str()), 1,
	GL_FALSE, &(const_cast<mat2x2&>(mat)[0][0]));
}

void OpenGLShaderImpl::setUniform3m(const std::string& name,
		const mat3x3& mat) {
	bind();
	glUniformMatrix3fv(glGetUniformLocation(renderID, name.c_str()), 1, GL_TRUE,
			&(const_cast<mat3x3&>(mat)[0][0]));
}

void OpenGLShaderImpl::setUniform4m(const std::string& name,
		const mat4x4& mat) {
	bind();
	glUniformMatrix4fv(glGetUniformLocation(renderID, name.c_str()), 1, GL_TRUE,
			&(const_cast<mat4x4&>(mat)[0][0]));
}

void OpenGLShaderImpl::bind() {
	if (this->context->boundShader != this) {
		glUseProgram(renderID);
		this->context->boundShader = this;
	}
}

void OpenGLShaderImpl::unbind() {
	if (this->context->boundShader != 0) {
		glUseProgram(0);
		this->context->boundShader = 0;
	}
}

}
