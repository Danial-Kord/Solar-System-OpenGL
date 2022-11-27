#define _USE_MATH_DEFINES



//#include<include/soil2.h>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <filesystem>
#include <list>
#include <vector>
#include <limits>
#include <functional>

#include "GLDebug.h"
#include "Planet.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"
#include "Texture.h"
#include "SimpleCamera.h"

#include "Geometry.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <ctime>




struct State {
	int sectors = 72;
	int stacks = 36;
	static constexpr const float radius = 10.0f;
	float deltaTime = 0.025f;
	float cameraSpeed = 0.025f; 

	float theta = M_PI / 360.0f; // rotation speed
	glm::vec3 lightPose = glm::vec3(38, -5, 10); //position of the light
};

//State
State state;

//Simple Camera
SimpleCamera simpleCam = SimpleCamera(glm::vec3(0, 60, -20), -90,90, glm::vec3(0, 0, -1));




//perspective
float fov = 90.0f;
float nearPlane = 0.1f;
float farPlane = 1000.f;
glm::mat4 projectionMatrix(1.0f);





// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	//camera change pose
	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_D) {
			simpleCam.move(state.cameraSpeed * state.deltaTime, direction::RIGHT);
		}
		if (key == GLFW_KEY_A) {
			simpleCam.move(state.cameraSpeed * state.deltaTime, direction::LEFT);
		}
		if (key == GLFW_KEY_W) {
			simpleCam.move(state.cameraSpeed * state.deltaTime, direction::FORWARD);
		}
		if (key == GLFW_KEY_S) {
			simpleCam.move(state.cameraSpeed * state.deltaTime, direction::BACKWARD);
		}

	}
	virtual void mouseButtonCallback(int button, int action, int mods) {
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			if (action == GLFW_PRESS) {
				rightMouseDown = true;
			}
			else if (action == GLFW_RELEASE) {
				rightMouseDown = false;
			}
		}
	}
	//camera change rotation
	virtual void cursorPosCallback(double xpos, double ypos) {
		if (rightMouseDown) {
			//std::cout << "front: " << simpleCam.getFront() << '\n'; // (1)
			//std::cout << "right: " << simpleCam.getRight() << '\n'; // (1)
			//std::cout << "up: " << simpleCam.getUp() << '\n'; // (1)
			double dx = xpos - mouseOldX;
			double dy = ypos - mouseOldY;
			simpleCam.updateMouseInput(state.cameraSpeed * state.deltaTime, dx, dy);
		}
		mouseOldX = xpos;
		mouseOldY = ypos;
	}
	virtual void scrollCallback(double xoffset, double yoffset) {
		//camera.incrementR(yoffset);
	}
	virtual void windowSizeCallback(int width, int height) {
		// The CallbackInterface::windowSizeCallback will call glViewport for us
		CallbackInterface::windowSizeCallback(width, height);
		
		projectionMatrix = glm::perspective(
			glm::radians(fov),
			float(width) / float(height),
			nearPlane,
			farPlane
		);
	}

	void viewPipeline(ShaderProgram& sp) {

		//glm::mat4 M = glm::mat4(1.0);
		//glm::mat4 V = camera.getView();
		//glm::mat4 P = glm::perspective(glm::radians(45.0f), aspect, 0.01f, 1000.f);

		//GLint location = glGetUniformLocation(sp, "light");
		//glm::vec3 light = camera.getPos();
		//glUniform3fv(location, 1, glm::value_ptr(light));


		//GLint uniMat = glGetUniformLocation(sp, "view");
		//glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(V));
		//uniMat = glGetUniformLocation(sp, "project");
		//glUniformMatrix4fv(uniMat, 1, GL_FALSE, glm::value_ptr(P));
	}


private:
	bool rightMouseDown = false;
	double mouseOldX;
	double mouseOldY;
};









int main() {
	
	std::cout << "Current path is " << std::filesystem::current_path() << '\n'; // (1)
	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired



	GLDebug::enable();


	// CALLBACKS
	std::shared_ptr<MyCallbacks> callbacks = std::make_shared<MyCallbacks>();
	window.setCallbacks(callbacks); // can also update callbacks to new ones





	//projection
	projectionMatrix = glm::perspective(
		glm::radians(fov),
		(float)window.getWidth() / window.getHeight(),
		nearPlane,
		farPlane
	);




	//TEST

	//vertexes.push_back({ glm::vec3(-0.5f, 0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 1.f), glm::vec3(0.f, 0.f, 1.f) });
	//vertexes.push_back({ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 0.f), glm::vec3(0.f, 0.f, 1.f) });
	//vertexes.push_back({ glm::vec3(0.5f, -0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 0.f), glm::vec3(0.f, 0.f, 1.f) });
	//vertexes.push_back({ glm::vec3(0.5f, 0.5f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(1.f, 1.f), glm::vec3(0.f, 0.f, 1.f) });
	//Indices.push_back(0);
	//Indices.push_back(1);
	//Indices.push_back(2);
	//Indices.push_back(0);
	//Indices.push_back(2);
	//Indices.push_back(3);


	//LIGHTS!
	//glm::vec3 lightPose = simpleCam.getPosition();//setting the position of light in camera spawn pos
	//glm::vec3 lightPose(state.lightPose);//setting the position of light in camera spawn pos
	glm::vec3 lightPose(0.0f); //where sun is located

	//Earth
	Planet planetEarth = Planet(glm::vec3(10.0f), "images/earth2k.jpg",true);
	planetEarth.drawShpere(state.sectors, state.stacks, state.radius);
	planetEarth.setRotation(glm::radians(23.5f), 0, 0);
	planetEarth.upLoadData();
	planetEarth.updateLight(lightPose);
	planetEarth.useShader();
	planetEarth.movementSpeed = 0.2f;
	planetEarth.rotationSpeed = M_PI / 3.0f;


	//Moon
	Planet moon = Planet(glm::vec3(1.0f), "images/2k_moon.jpg",true);
	moon.drawShpere(state.sectors, state.stacks, 2);
	moon.upLoadData();
	moon.updateLight(lightPose);
	moon.useShader();
	moon.movementSpeed = 1.5f;


	//Sun
	Planet sun = Planet(glm::vec3(0.0f), "images/2k_sun.jpg",false);
	sun.drawShpere(state.sectors, state.stacks, 14);
	sun.upLoadData();
	sun.useShader();



	glm::vec3 newP(10.0f);

	float earthPose = 0;
	float moonPose = 0;
	float tilt = 0;
	float tiltAmount = 0.0005f;

	clock_t begin = clock();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	// RENDER LOOP
	while (!window.shouldClose()) {

		end = clock();

		state.deltaTime = double(end - begin) / CLOCKS_PER_SEC;

		begin = clock();

		glfwPollEvents();


		glEnable(GL_FRAMEBUFFER_SRGB);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glDrawArrays(GL_TRIANGLES, 0, sizeof(vertexes)/sizeof(Vertex));

		earthPose += planetEarth.movementSpeed * state.deltaTime;
		newP.x = 40 * glm::cos(earthPose);
		newP.y = 32 * glm::sin(earthPose);
		newP.z = 0;


		planetEarth.useShader();
		planetEarth.setPosition(newP);
		planetEarth.rotate(glm::vec3(0, 0, planetEarth.rotationSpeed * state.deltaTime));
		planetEarth.updateMVP(simpleCam.getViewMatrix(), projectionMatrix);
		planetEarth.Draw();


		
		moonPose += moon.movementSpeed * state.deltaTime;

		newP.x = 15 * glm::cos(moonPose);
		newP.y = 15 * glm::sin(moonPose);
		newP.z = 0;
		moon.useShader();
		moon.setPosition(planetEarth.getPoistion() + newP);
		moon.updateMVP(simpleCam.getViewMatrix(), projectionMatrix);
		moon.Draw();

		sun.useShader();
		sun.updateMVP(simpleCam.getViewMatrix(), projectionMatrix);
		sun.Draw();

		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
		glFlush();





	}

	glfwTerminate();
	return 0;
}
