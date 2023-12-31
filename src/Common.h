#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include <imgui.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

enum class LightType : int
{
	Directional = 0, Point, Spot,
};

namespace GlNs
{
	struct GlobalData
	{
		// positions of the point lights
		glm::vec3 pointLightPositions[4]{
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};
		bool enableMouseMove{false};
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImVec4 lamp_color = ImVec4(1.f, 1.f, 1.f, 1.f);
		float ang;
		unsigned int VAO, VBO;
		unsigned int lightCubeVAO;
		bool show_demo_window = false;
		bool show_another_window = false;
		GLFWwindow* window{};
		int windowH{ 720 }, windowW{ 1280 };
		int lastX{ windowW / 2 }, lastY{ windowH /2 };
		bool firstMouse{true};
		float nearP{ 0.1f }, farP{ 100.f };
		//float fov{ 45.f };
		bool depthTesting{true};
		bool LinearizeDepth{ false };
		int depthFunc{GL_LESS};
		bool showMore{ false };
		bool rotateByTime{ true };
		bool showSmile{false};
		float shininess{32};
		struct
		{
			//glm::vec3 pos{ 2.f, 2.0f, -2.0f };//??
			glm::vec3 pos{ 0.65f, 0.2f, 1.75f };//??
			bool autoRotation{false};
			//bool directional{false};
			//LightType type{ LightType::Directional };
			bool dir, point{true}, spot;
			bool attenuation{false};
			bool softEdges{false};
		} lamp;
		struct
		{
			Camera camera{ glm::vec3{0.f, 0.f, 3.f} };
			bool autoRotation{ false };
			//glm::vec3 cameraPos{ 0.f, 0.f, 3.f };
			//glm::vec3 cameraFront{ 0.f, 0.f, -1.f };
			//glm::vec3 cameraUp{ 0.f, 1.f, 0.f };
			float deltaTime{ 0.f };
			float lastFrame{ 0.f };
			glm::mat4 view{ 1.f };
			glm::mat4 projection{ 1.f };
			
		} camera{};

		struct
		{
			bool visible{false};
			glm::vec3 pos{1.2f, 0.f, 0.15f};
			float scale{.25f};
			float rotate{0.f};
		} model;

		struct
		{
			bool visible{false};
		} volume;
	};
	inline GlobalData gData{};
}