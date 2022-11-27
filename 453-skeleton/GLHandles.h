#pragma once

#include <GL/glew.h>


// An RAII class for managing a Shader GLuint for OpenGL.
//
// RAII stands for Resource Acquisition Is Initialization, which
// is a hard to understand name, but a simple to understand concept.
// Basically it means that if you use a C++ class to represent a
// resource, then its constructor should allocate the resource, and its
// destructor should deallocate the resource and you should carefully
// implement the copy and move facilities or remove them.
//
// Also, it usually means that you have a single wrapper (also called
// handle) class that does only one thing: manage that resource.
//
// This class is an example of this for shaders.
//
// https://en.cppreference.com/w/cpp/language/raii
// https://www.youtube.com/watch?v=7Qgd9B1KuMQ
//
class ShaderHandle {

public:
	ShaderHandle(GLenum type);

	// There are some guidelines for when to know what to implement
	// related to a class here:
	// https://en.cppreference.com/w/cpp/language/rule_of_three

	// Copying a shader ID doesn't necessarily make sense, because
	// the new version isn't really a copy of the original, it's
	// a brand new things. So we disable both the copy constructor
	// https://en.cppreference.com/w/cpp/language/copy_constructor
	ShaderHandle(const ShaderHandle&) = delete;
	// and the copy assignment operator
	// https://en.cppreference.com/w/cpp/language/copy_assignment
	ShaderHandle operator=(const ShaderHandle&) = delete;

	// Moving a shader does make sense, as the old one can be empty
	// and the new one can be the ID of the old one. So we implement
	// the move constructor.
	// https://en.cppreference.com/w/cpp/language/move_constructor
	ShaderHandle(ShaderHandle&& other) noexcept;
	// and the move assignment operator:
	// https://en.cppreference.com/w/cpp/language/move_assignment
	ShaderHandle& operator=(ShaderHandle&& other) noexcept;

	// Destructor to cleanup resources on GPU
	~ShaderHandle();


	// Allow casting from this type into a GLuint
	// This allows usage in situations where a function expects a GLuint
	operator GLuint() const;
	GLuint value() const;

private:
	GLuint shaderID;

};



// An RAII class for managing a ShaderProgram GLuint for OpenGL.
//
// NOTE: the rest of these won't be as well documented, because they
// are essentially the same pattern over and over.
//
// NOTE: There is a facility for avoiding this kind of repetition in C++
// which are called templates. Using templates and inheritance you can
// significantly reduce this repetition if you want. Give it a try.
class ShaderProgramHandle {

public:
	ShaderProgramHandle();

	// Disallow copying
	ShaderProgramHandle(const ShaderProgramHandle&) = delete;
	ShaderProgramHandle operator=(const ShaderProgramHandle&) = delete;

	// Allow moving
	ShaderProgramHandle(ShaderProgramHandle&& other) noexcept;
	ShaderProgramHandle& operator=(ShaderProgramHandle&& other) noexcept;

	// Clean up after ourselves.
	~ShaderProgramHandle();


	// Allow casting from this type into a GLuint
	// This allows usage in situations where a function expects a GLuint
	operator GLuint() const;
	GLuint value() const;

private:
	GLuint programID;

};


// An RAII class for managing a VertexArray GLuint for OpenGL.
class VertexArrayHandle {

public:
	VertexArrayHandle();

	// Disallow copying
	VertexArrayHandle(const VertexArrayHandle&) = delete;
	VertexArrayHandle operator=(const VertexArrayHandle&) = delete;

	// Allow moving
	VertexArrayHandle(VertexArrayHandle&& other) noexcept;
	VertexArrayHandle& operator=(VertexArrayHandle&& other) noexcept;

	// Clean up after ourselves.
	~VertexArrayHandle();


	// Allow casting from this type into a GLuint
	// This allows usage in situations where a function expects a GLuint
	operator GLuint() const;
	GLuint value() const;

private:
	GLuint vaoID;

};

// An RAII class for managing a VertexBuffer GLuint for OpenGL.
class VertexBufferHandle {

public:
	VertexBufferHandle();

	// Disallow copying
	VertexBufferHandle(const VertexBufferHandle&) = delete;
	VertexBufferHandle operator=(const VertexBufferHandle&) = delete;

	// Allow moving
	VertexBufferHandle(VertexBufferHandle&& other) noexcept;
	VertexBufferHandle& operator=(VertexBufferHandle&& other) noexcept;

	// Clean up after ourselves.
	~VertexBufferHandle();


	// Allow casting from this type into a GLuint
	// This allows usage in situations where a function expects a GLuint
	operator GLuint() const;
	GLuint value() const;

private:
	GLuint vboID;

};

// An RAII class for managing a VertexBuffer GLuint for OpenGL.
class TextureHandle {

public:
	TextureHandle();


	// Disallow copying
	TextureHandle(const TextureHandle&) = delete;
	TextureHandle operator=(const TextureHandle&) = delete;

	// Allow moving
	TextureHandle(TextureHandle&& other) noexcept;
	TextureHandle& operator=(TextureHandle&& other) noexcept;

	// Clean up after ourselves.
	~TextureHandle();

	// Allow casting from this type into a GLuint
	// This allows usage in situations where a function expects a GLuint
	operator GLuint() const;
	GLuint value() const;

private:
	GLuint textureID;

};
