#pragma once

#include "GLHandles.h"

#include <GL/glew.h>

#include <string>

class ShaderProgram;

class Shader {

public:
	Shader(const std::string& path, GLenum type);

	// Because we're using the ShaderHandle to do RAII for the shader for us
	// and our other types are trivial or provide their own RAII
	// we don't have to provide any specialized functions here. Rule of zero
	//
	// https://en.cppreference.com/w/cpp/language/rule_of_three
	// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero

	// Public interface
	std::string getPath() const { return path; }
	GLenum getType() const { return type; }

	void friend attach(ShaderProgram& sp, Shader& s);

private:
	ShaderHandle shaderID;
	GLenum type;

	std::string path;

	bool compile();
};

