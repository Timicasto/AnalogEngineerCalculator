#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

import Renderer;
import std;


int main() {
	const auto window = Window({1920, 1080}, "AnalogEngineerCalculator");

	window.show();

	/*
	auto renderer = FontRenderer();
	auto font = renderer.parseOpenType("MapleMono-CN-SemiBold.ttf", 24);
	*/

	while (!window.shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
		renderer.render(font, L"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec et mi in elit rutrum accumsan rutrum ac urna. Vestibulum purus justo, iaculis at nisl id, vehicula finibus ipsum. Nunc ultrices nisi lorem, non egestas est tincidunt eget. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Morbi et felis et lorem volutpat pretium. Etiam ultricies imperdiet justo non feugiat. Quisque ac ullamcorper tortor. Vestibulum tristique gravida augue, et facilisis orci. Fusce vehicula nec tortor quis aliquam.",
			100, 100, 1.0, {0.8F, 0.3F, 0.1F}, window.getProjection());
			*/


		window.post();
		glfwPollEvents();
	}
}