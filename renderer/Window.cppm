/**
******************************************************************************
  * @file    Window.cppm
  * @author  Timicasto
  * @brief   GLFW window class
  *          This file provides a window pointer manager.
  *
  * @attention DO NOT MAKE DIRECT OPENGL FUNCTION CALLS BEFORE READING THE LIBRARY CODE!
  *			   OpenGL is a huge state machine but written in C not C++
  *			   If you load data or modify state when the OpenGL state machine is not present
  *			   for your operation, your program will just crash with a segmentation fault,
  *			   or present a wrong behavior without any warnings,
  *			   nothing will be provided unless glGetError even if you use a breakpoint debugger.
  *			   Make direct GL calls only when you know what you are doing.
  *			   "Window" is just a class, you can also simply extend it and do your customization
  *			   by overriding functions, but always remember C codes are extremely hard to debug!
  *
  ******************************************************************************
  */

// SPDX-License-Identifier: AGPL-3.0-or-later

module;
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

export module Renderer:Window;
import std;
import Logger;

export class Window {
public:
	Window(const glm::uvec2 size, const std::string& title) {
		if (!glfwInit()) {
			const std::string errorInfo = std::to_string(glfwGetError(nullptr));
			Logger::log(LogLevel::FATAL, "Failed to initialize GLFW, error " + errorInfo);
			glfwTerminate();
		} else {
			Logger::log(LogLevel::INFO, "GLFW initialized");
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

		if (window == nullptr) {
			const std::string errorInfo = std::to_string(glfwGetError(nullptr));
			Logger::log(LogLevel::FATAL, "Failed to create window, error " + errorInfo);
			glfwTerminate();
		} else {
			Logger::log(LogLevel::INFO, "Window created with size " + std::to_string(size.x) + "x" + std::to_string(size.y) + " and context version 3.3");
		}

		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
			Logger::log(LogLevel::FATAL, "Failed to initialize GLAD.");
			glfwTerminate();
		} else {
			Logger::log(LogLevel::INFO, "GLAD initialized with GLFW Process Address");
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, size.x, size.y);
		projection = glm::ortho(0.0F, static_cast<float>(size.x), 0.0F, static_cast<float>(size.y));
	}

	[[nodiscard]] bool shouldClose() const {
		return glfwWindowShouldClose(window);
	}

	void post() const {
		glfwSwapBuffers(window);
	}

	void resize(const glm::uvec2 size) {
		glfwSetWindowSize(window, size.x, size.y);
		glViewport(0, 0, size.x, size.y);
		projection = glm::ortho(0.0F, static_cast<float>(size.x), 0.0F, static_cast<float>(size.y));
	}

	void setTitle(const std::string& title) const {
		glfwSetWindowTitle(window, title.c_str());
	}

	static void setVsync(const bool vsync) {
		glfwSwapInterval(vsync ? 1 : 0);
	}

	void maximize() const {
		glfwMaximizeWindow(window);
	}

	void setPos(glm::ivec2 pos) const {
		glfwSetWindowPos(window, pos.x, pos.y);
	}

	void hide() const {
		glfwHideWindow(window);
	}

	void show() const {
		glfwShowWindow(window);
	}

	void focus() const {
		glfwFocusWindow(window);
	}

	[[nodiscard]] glm::mat4 getProjection() const {
		return projection;
	}

private:
	GLFWwindow *window{};
	glm::mat4 projection{};
};
