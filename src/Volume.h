#pragma once

class Shader;

struct Volume
{
	static Volume& instance();
	Volume();
	~Volume();
	void Draw();

	int shaderProgram;
	unsigned int vao, vbo/*, ebo;*/;
	unsigned int texture0;
	Shader* pShader;
	float angle;
	float m_rotateY{};
	float m_rotateX{};
	float m_rotateZ{};
};

//extern Volume* pVolume;