#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <glad/glad.h>
#include <iostream>

const char *vSS = R"(
#version 450 core
layout (location = 0) in vec2 pos;
out vec2 fragCoord;
void main() {
	fragCoord = pos;
	gl_Position = vec4 (pos*2-vec2(1,1), 0.0, 1.0);
}
)";

void loadFragmentShader(std::string& dest) {
	std::stringstream ss;
	ss << R"(
#version 450 core

uniform vec2 uMouse;
uniform vec2 uResolution;
uniform double uTime;
uniform double uTimeDelta;
uniform uint uFrame;

in vec2 fragCoord;
out vec4 fragColor;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

)";
	std::ifstream file("./shader.glsl", std::ios::in | std::ios::binary);
	ss << file.rdbuf();
	

	dest = ss.str();
}

std::string error{};
bool isError = false;

GLuint compileShader(GLenum type, const char* source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		error += "ERROR::SHADER::COMPILATION_FAILED\n";
		(error += infoLog) += '\n';
		isError = true;
	}
	return shader;
}

GLuint linkShaderProgram(GLuint vertexShader, GLuint fragmentShader) {
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		error += "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
		(error += infoLog) += '\n';
		isError = true;
	}
	return shaderProgram;
}


#include "Windows.h"

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
