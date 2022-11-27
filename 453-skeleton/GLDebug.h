#pragma once
#include "GL/glew.h"

//------------------------------------------------------------------------------
// OpenGL has a mechanism where you can turn on debug mode and it will tell you
// all sorts of fun stuff about what you're doing.
//
// We are going to use it (best we can) to give you advanced warning of when you
// are doing something incorrectly.
//------------------------------------------------------------------------------


namespace GLDebug {

	void debugOutputHandler(
		GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei,
		const GLchar *message,
		const void *
	);

	void enable();
}
