#pragma once
#include <vector>
#include <limits>
#include <functional>

#include "Geometry.h"
#include "Texture.h"
#include "ShaderProgram.h"


#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Planet
{
private:
	// SHADERS
	ShaderProgram shader = ShaderProgram("shaders/test.vert", "shaders/test.frag");;
	GLuint VAO;
	std::vector<Vertex> vertexes;
	std::vector<GLuint> Indices;
	glm::mat4 transform;
	glm::mat4 position;
	glm::mat4 rotation;
	glm::mat4 globalRotation;
	glm::mat4 globalPosition;
	glm::vec3 eulerAngles;
	glm::vec3 positionVector;
	unsigned int textureID;



	void updateTransform() {
		transform =  globalPosition * globalRotation * position * rotation;
	}


public:
	double rotationSpeed;
	double movementSpeed;

	Planet (glm::vec3 startingPose, const char* imagePath, bool enableLight){
		this->vertexes = std::vector<Vertex>();
		this->Indices = std::vector<GLuint>();

		if (enableLight == true) {
			this->shader = ShaderProgram("shaders/test.vert", "shaders/test.frag");
		}
		else
		{
			printf("wtfff");
			this->shader = ShaderProgram("shaders/sun.vert", "shaders/sun.frag");//TODO make this with inheritance a subclass sun!
		}

		setPosition(startingPose);
		this->rotation = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		this->eulerAngles = glm::vec3(0.0f);
		
		this->globalRotation = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		this->globalPosition = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		updateTransform();

		textureID  = Texture::loadTexture(imagePath);
		
	}

	std::vector<GLuint> getIndices() {
		return this->Indices;
	}

	glm::vec3 getPoistion() {
		return this->positionVector;
	}



	void drawShpere(int sectorCount, int stackCount, float radius) {
		

		float x, y, z, xy;                              // vertex position
		float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
		float s, t;                                     // vertex texCoord
		float radius2 = radius;

		float sectorStep = (float)(2 * M_PI / sectorCount);
		float stackStep = (float)(M_PI / stackCount);
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = (float)(M_PI / 2 - i * stackStep);       // starting from pi/2 to -pi/2
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius2 * sinf(stackAngle);              // r * sin(u)

			// add (sectorCount+1) vertices per stack
			// the first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				Vertex v;
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
				v.position = glm::vec3(x, y, z);
				v.color = glm::vec3(0, 1, 0);
				// normalized vertex normal (nx, ny, nz)
				nx = x * lengthInv;
				ny = y * lengthInv;
				nz = z * lengthInv;
				v.normal = glm::vec3(nx, ny, nz);


				// vertex tex coord (s, t) range between [0, 1]
				s = 1 - (float)j / sectorCount; //for fliping the planet
				t = (float)i / stackCount;
				v.textco = glm::vec2(s, t);
				this->vertexes.push_back(v);
				
			}
		}


		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					this->Indices.push_back(k1);
					this->Indices.push_back(k2);
					this->Indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					this->Indices.push_back(k1 + 1);
					this->Indices.push_back(k2);
					this->Indices.push_back(k2 + 1);
				}

			}
		}
	}

	void upLoadData() {
		VAO;
		//VAO, VBO, EBO
		//GEN VAO AND BIND

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//GEN VBO AND BIND AND SEND DATA
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexes.size(), vertexes.data(), GL_STATIC_DRAW);

		//GEN EBO AND BIND AND SEND DATA
		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Indices.size(), Indices.data(), GL_STATIC_DRAW);


		//TODO making it optimized by not sending normals for sun as it dont use it!
		
		//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
		glEnableVertexAttribArray(1);
		//Texture Coordinates
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textco));
		glEnableVertexAttribArray(2);
		//Normals
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(3);
		//BIND VAO 0
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(VAO);
		useShader();
	}


	void useShader() {
		shader.use();
	}

	void updateMVP(const glm::mat4& viewMatrix, glm::mat4& projectionMatrix) {
		this->updateTransform();
		glUniformMatrix4fv(glGetUniformLocation(shader, "transform"), 1, GL_FALSE, glm::value_ptr(this->transform));
		glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(glGetUniformLocation(shader, "project"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

	void updateLight(glm::vec3& lightPose) {
		glUniform3fv(glGetUniformLocation(shader, "light"), 1, glm::value_ptr(lightPose));
	}

	void setRotation(float thetax, float thetay, float thetaz) {
		eulerAngles.x = thetax;
		eulerAngles.y = thetay;
		eulerAngles.z = thetaz;

		rotation = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, std::cos(thetax), -std::sin(thetax), 0.0f,
			0.0f, std::sin(thetax), std::cos(thetax), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
		
		rotation = rotation * glm::mat4(
			std::cos(thetay), 0.0f, std::sin(thetay), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-std::sin(thetay), 0.0f, std::cos(thetay), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);

		rotation = rotation * glm::mat4(
			std::cos(thetaz), -std::sin(thetaz), 0.0f, 0.0f,
			std::sin(thetaz), std::cos(thetaz), 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	void rotate(glm::vec3 deltaAngle) {
		this->eulerAngles += deltaAngle;
		setRotation(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	}

	void setPosition(glm::vec3 newPose) {
		this->positionVector = newPose;
		this->position = glm::mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			newPose.x, newPose.y, newPose.z, 1.0f
		);
	}

	void translate(glm::vec3 deltaVector) {
		this->positionVector += deltaVector;
		setPosition(this->positionVector);
	}


	void Draw()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,
			(unsigned int) getIndices().size(),
			GL_UNSIGNED_INT,
			(void*)0);
		glBindVertexArray(0);
	}

};
