#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

namespace GlNs
{
	struct GlobalData
	{
		bool enableMouseMove{false};
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		float ang;
		unsigned int VAO, VBO;
		bool show_demo_window = false;
		bool show_another_window = false;
		GLFWwindow* window{};
		int windowH{ 720 }, windowW{ 1280 };
		int lastX{ windowW / 2 }, lastY{ windowH /2 };
		bool firstMouse{true};
		float nearP{ 0.1f }, farP{ 100.f };
		//float fov{ 45.f };
		bool depthTesting{true};
		bool showMore{ false };
		bool rotateByTime{ true };
		struct
		{
			Camera camera{ glm::vec3{0.f, 0.f, 3.f} };
			bool autoRotation{ false };
			//glm::vec3 cameraPos{ 0.f, 0.f, 3.f };
			//glm::vec3 cameraFront{ 0.f, 0.f, -1.f };
			//glm::vec3 cameraUp{ 0.f, 1.f, 0.f };
			float deltaTime{ 0.f };
			float lastFrame{ 0.f };
			
		} camera{};
	};
	inline GlobalData gData{};
}