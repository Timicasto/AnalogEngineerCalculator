module;
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

export module Renderer:Shader;

import std;
import Logger;

export class Shader {
public:
	Shader(const std::string& vsh, const std::string &fsh) : id(glCreateProgram()){
		int result;
		int length;

		const auto vCode = vsh.c_str();
		const auto fCode = fsh.c_str();

		const unsigned int tmpVsh = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(tmpVsh, 1, &vCode, nullptr);
		glCompileShader(tmpVsh);
		glGetShaderiv(tmpVsh, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderiv(tmpVsh, GL_INFO_LOG_LENGTH, &length);
			char infoLog[length];
			glGetShaderInfoLog(tmpVsh, length, nullptr, infoLog);
			Logger::log(LogLevel::ERROR, "Vertex shader compilation failed: " + std::string(infoLog));
		}

		const unsigned int tmpFsh = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(tmpFsh, 1, &fCode, nullptr);
		glCompileShader(tmpFsh);
		glGetShaderiv(tmpFsh, GL_COMPILE_STATUS, &result);
		if (!result) {
			glGetShaderiv(tmpFsh, GL_INFO_LOG_LENGTH, &length);
			char infoLog[length];
			glGetShaderInfoLog(tmpFsh, length, nullptr, infoLog);
			Logger::log(LogLevel::ERROR, "Fragment shader compilation failed: " + std::string(infoLog));
		}

		glAttachShader(id, tmpVsh);
		glAttachShader(id, tmpFsh);
		glLinkProgram(id);
		glGetShaderiv(id, GL_LINK_STATUS, &result);
		if (!result) {
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char infoLog[length];
			glGetShaderInfoLog(id, length, nullptr, infoLog);
			Logger::log(LogLevel::ERROR, "Failed to link shader program: " + std::string(infoLog));
		}

		glDeleteShader(tmpVsh);
		glDeleteShader(tmpFsh);
	}

	void use() const {
		glUseProgram(id);
	}

	void setBool(const std::string& name, const bool value) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
	}

	void setInt(const std::string& name, const int val) const {
		glUniform1i(glGetUniformLocation(id, name.c_str()), val);
	}

	void setFloat(const std::string& name, const float val) const {
		glUniform1f(glGetUniformLocation(id, name.c_str()), val);
	}

	void setVec3(const std::string& name, const glm::vec3& val) const {
		glUniform3f(glGetUniformLocation(id, name.c_str()), val.x, val.y, val.z);
	}

	void setMat4(const std::string& name, const glm::mat4& val) const {
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
	}
private:
	unsigned int id;
};

