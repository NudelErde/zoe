/*
 * OpenGLShaderImpl.cpp
 *
 *  Created on: 20.04.2019
 *      Author: florian
 */

//TODO: read check for uniforms from source -> optimisation
//TODO: do not reassign uniform if it has the same value
//TODO: special optimisation for samplers -> sampler always has same slot
//TODO: warn in constructor for unused uniforms

#include "OpenGLShaderImpl.h"

#include <GL/glew.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "../../zoe/core/String.h"
#include "../../zoe/render/GraphicsContext.h"

namespace Zoe {

struct layoutLocation {
    unsigned int location{};
    std::string varName;
};

struct ShaderSource {
    std::string vertexShader;
    std::string fragmentShader;
    std::vector<layoutLocation> vertexLayoutLocation;
    std::vector<layoutLocation> fragmentLayoutLocation;
    std::map<std::string, unsigned int> samplerSlots;
    std::map<std::string, std::string> tags;
};

static ShaderSource parseShader(const File &file, const std::set<std::string> &options) {
    unsigned int version = 0;
    std::unique_ptr<std::istream> stream = file.createIStream(false);
    ShaderSource shso = ShaderSource();
    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[2];
    unsigned int samplerSlot = 0;
    size_t index;
    bool optionSkipToEnd = false;
    while (getline(*stream, line)) {
        trim(line);
        if(optionSkipToEnd){
            toLower(line);
            if (line == "#zoe optionend") {
               optionSkipToEnd = false;
            }
            continue;
        }
        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        } else if (line.rfind("#zoe", 0) == 0) {
            std::vector<std::string> args = split(line, " ");
            toLower(args[1]);
            if (args.size() > 2 && args[1] == "optionbegin" && !options.count(args[2])) {
                optionSkipToEnd = true;
            }
        } else if (line.find("#version") != std::string::npos) {
            size_t start = line.find("#version");
            const char *cstr = line.c_str();
            int spaceCounter = 3;
            size_t end = start;
            while (spaceCounter && end < line.length()) {
                if (cstr[end] == ' ' || cstr[end] == '\n') {
                    --spaceCounter;
                }
                if (spaceCounter == 2) {
                    start = end;
                    --spaceCounter;
                }
                ++end;
            }
            try {
                version = std::stoi(line.substr(start, end));
            }
            catch (std::invalid_argument&) {
                error("Could not read version of shader ", file.getPath(), ": ", line.substr(start, end));
            }
            catch (std::out_of_range&) {
                error("Could not read version of shader ", file.getPath(), ": ", line.substr(start, end));
            }
            if (type != ShaderType::NONE)
                ss[(int) type] << line << '\n';
        } else if (line.rfind("#tag", 0) == 0) {
            size_t start = line.find("#tag");
            const char *cstr = line.c_str();
            int spaceCounter = 3;
            size_t end = start;
            while (spaceCounter && end < line.length()) {
                if (cstr[end] == ' ' || cstr[end] == '\n') {
                    --spaceCounter;
                }
                if (spaceCounter == 2) {
                    start = end;
                    --spaceCounter;
                }
                ++end;
            }

            shso.tags[line.substr(start + 1, end - start - 2)] = line.substr(end,
                                                                             line.length() - end);//TODO: trim value
        } else if (version < 330 && (line.rfind("layout(location=", 0)) == 0) {
            index = line.find("layout(location=");
            size_t substringStart = 0;
            const char *str = line.c_str();
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
            if (type != ShaderType::NONE)
                ss[(int) type] << line.substr(substringStart + 1) << '\n';
        } else if (version < 330 && line.rfind("uniform sampler2D", 0) == 0) {
            index = line.find("uniform sampler2D");
            const char *cstr = line.c_str();
            int spaceCounter = 2;
            while (spaceCounter) {
                if (cstr[index] == ' ') {
                    --spaceCounter;
                }
                ++index;
            }
            shso.samplerSlots[line.substr(index, line.length() - index - 1)] =
                    samplerSlot;
            ++samplerSlot;
            if (type != ShaderType::NONE)
                ss[(int) type] << line << '\n';
        } else {
            if (type != ShaderType::NONE)
                ss[(int) type] << line << '\n';
        }
    }
    shso.fragmentShader = ss[1].str();
    shso.vertexShader = ss[0].str();
    return shso;
}

static unsigned int compileShader(unsigned int type, const std::string &source) {
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        error("Failed to compile ", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), " shader! \n", message);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShaderProgram(ShaderSource &ss) {
    unsigned int prg = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, ss.vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, ss.fragmentShader);

    if (vs == 0 || fs == 0)
        return 0;

    glAttachShader(prg, vs);
    glAttachShader(prg, fs);

    for (layoutLocation &ll : ss.vertexLayoutLocation) {
        glBindAttribLocation(prg, ll.location, ll.varName.c_str());
    }
    for (layoutLocation &ll : ss.fragmentLayoutLocation) {
        glBindAttribLocation(prg, ll.location, ll.varName.c_str());
    }

    glLinkProgram(prg);
    glValidateProgram(prg);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return prg;
}

OpenGLShaderImpl::OpenGLShaderImpl(const File &file, const std::set<std::string> &options, GraphicsContext *context)
        : ShaderImpl(context) {
    ShaderSource ss = parseShader(file, options);
    samplerSlot = ss.samplerSlots;
    tags = ss.tags;
    renderID = createShaderProgram(ss);
    if (renderID == 0) {
        std::stringstream sstream;
        sstream << "Failed to construct Shader for file: " << file.getPath();
        throw std::runtime_error(sstream.str());
    }
}

OpenGLShaderImpl::~OpenGLShaderImpl() {
    if (context->boundShader != nullptr && context->boundShader->getID() == id) {
        glUseProgram(0);
        context->boundShader = nullptr;
    }
    glDeleteProgram(renderID);
}

void OpenGLShaderImpl::setUniform1f(const std::string &name, float v0) {
    bind();
    glUniform1f(getUniformLocation(name), v0);
}

void OpenGLShaderImpl::setUniform2f(const std::string &name, float v0, float v1) {
    bind();
    glUniform2f(getUniformLocation(name), v0, v1);
}

void OpenGLShaderImpl::setUniform3f(const std::string &name, float v0, float v1, float v2) {
    bind();
    glUniform3f(getUniformLocation(name), v0, v1, v2);
}

void OpenGLShaderImpl::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3) {
    bind();
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void OpenGLShaderImpl::setUniform2m(const std::string &name, const mat2x2 &mat) {
    bind();
    float fMat[4];
    for(uint8_t y = 0; y < 2; ++y) {
        for(uint8_t x = 0; x < 2; ++x) {
            fMat[y*2+x] = (float) mat[x][y];
        }
    }
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, fMat);
}

void OpenGLShaderImpl::setUniform3m(const std::string &name, const mat3x3 &mat) {
    bind();
    float fMat[9];
    for(uint8_t y = 0; y < 3; ++y) {
        for(uint8_t x = 0; x < 3; ++x) {
            fMat[y*3+x] = (float) mat[x][y];
        }
    }
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, fMat);
}

void OpenGLShaderImpl::setUniform4m(const std::string &name, const mat4x4 &mat) {
    bind();
    float fMat[16];
    for(uint8_t y = 0; y < 4; ++y) {
        for(uint8_t x = 0; x < 4; ++x) {
            fMat[y*4+x] = (float) mat[x][y];
        }
    }
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, fMat);
}

void OpenGLShaderImpl::setTexture(const std::string &name, Texture &texture) {
    unsigned int slot = samplerSlot[name];
    texture.bind(slot);
    bind();
    glUniform1i(getUniformLocation(name), slot);
}

void OpenGLShaderImpl::bind() {
    if (context->boundShader == nullptr || context->boundShader->getID() != id) {
        glUseProgram(renderID);
        context->boundShader = this;
    }
}

void OpenGLShaderImpl::unbind() {
    if (context->boundShader != nullptr || context->boundShader->getID() != 0) {
        glUseProgram(0);
        context->boundShader = nullptr;
    }
}

const std::map<std::string, std::string> &OpenGLShaderImpl::getTags() {
    return tags;
}

int OpenGLShaderImpl::getUniformLocation(const std::string &uniformName) const {
    int location = glGetUniformLocation(renderID, uniformName.c_str());
    if (location == -1) {
        if (!undefinedUniformsLogged.count(uniformName)) {
            warning("Uniform ", uniformName, " is not found.");
            undefinedUniformsLogged.insert(uniformName);
        }
    }
    return location;
}

}
