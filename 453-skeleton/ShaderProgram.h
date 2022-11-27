#pragma once

#include "Shader.h"

#include "GLHandles.h"

#include <GL/glew.h>

#include <string>


class ShaderProgram {

public:
	
	ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

	// Because we're using the ShaderProgramHandle to do RAII for the shader for us
	// and our other types are trivial or provide their own RAII
	// we don't have to provide any specialized functions here. Rule of zero
	//
	// https://en.cppreference.com/w/cpp/language/rule_of_three
	// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rc-zero

	// Public interface
	bool recompile();
	void use() const { glUseProgram(programID); }

	void friend attach(ShaderProgram& sp, Shader& s);

	operator GLuint() const {
		return programID;
	}

private:
	ShaderProgramHandle programID;

	Shader vertex;
	Shader fragment;

	bool checkAndLogLinkSuccess() const;
};
