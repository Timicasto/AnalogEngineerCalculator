#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

import Renderer;
import std;


int main() {
	const auto window = Window({1920, 1080}, "AnalogEngineerCalculator");

	window.show();

	while (!window.shouldClose()) {

		window.post();
		glfwPollEvents();
	}
}