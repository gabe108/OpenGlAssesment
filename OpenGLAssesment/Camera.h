#pragma once
#include<glm/ext.hpp>
#include<GLFW/glfw3.h>

class Camera
{
public:
	Camera() 
	{
		m_worldTransform = glm::mat4(1);
		m_viewTransform = glm::mat4(1);
		m_projectionTransform = glm::mat4(1);
		m_projectionViewTransform = glm::mat4(1);
		m_fLastX = 1280.0f / 2.0;
		m_fLastY = 720.0f / 2.0;
		m_bFirstMouse = true;
		m_cameraTarget = glm::vec3(0);
		m_cameraDirection = glm::vec3(0);
		m_cameraRight = glm::vec3(0);
		m_cameraUP = glm::vec3(0);
		m_cameraFront = glm::vec3(0);
		m_sensitivity = 0.1f;
		m_fPitch = 0.f;
		m_fYaw = -90.f;
		m_fXoffset = 0.f;
		m_fYoffset = 0.f;
		m_nwidth = 0.0f;
		m_nheight = 0.0f;
		
		glfwSetWindowUserPointer(glfwGetCurrentContext(), this);
		glfwSetCursorPosCallback(glfwGetCurrentContext(), mouse_callback);
	}
	
	virtual void update(float _dt) = 0;

	void setPerspective(float _fieldOfView, float aspectRatio, float _near, float _far)
	{
		m_projectionTransform = glm::perspective(_fieldOfView,
			aspectRatio, _near, _far);
	}
	void setLookAt(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up) 
	{ 
		glm::vec3 pos(0);
		pos.x = m_worldTransform[3].x;
		pos.y = m_worldTransform[3].y;
		pos.z = m_worldTransform[3].z;
		m_viewTransform = glm::lookAt(_from, _to, _up); 
	}
	void setPosition(glm::vec3 _pos){ m_worldTransform[3] = glm::vec4(_pos, 1);	}
	void setWorldTransform(glm::mat4 _world) { m_worldTransform = _world; }
	void setTarget(glm::vec3 _target) { m_cameraTarget = _target; }

	glm::mat4 getWorldTransform() { return m_worldTransform; }
	glm::mat4 getViewTransform() { return m_viewTransform; }
	glm::mat4 getProjectionTransform() { return m_projectionTransform; }
	glm::mat4 getProjectionViewTransform() { return m_projectionViewTransform; }

	void calculateDirection() 
	{ 
		glm::vec3 pos(0);
		pos.x = m_worldTransform[3].x;
		pos.y = m_worldTransform[3].y;
		pos.z = m_worldTransform[3].z;
		m_cameraDirection = glm::normalize(pos - m_cameraTarget);
	}
	void calculateAxis()
	{
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_cameraRight = glm::normalize(glm::cross(up, m_cameraDirection));
		m_cameraUP = glm::cross(m_cameraDirection, m_cameraRight);
	}
	void updateProjectionViewTransform()
	{
		calculateDirection();
		calculateAxis();
		m_viewTransform = glm::inverse(m_worldTransform);
		m_projectionViewTransform = m_projectionTransform * m_viewTransform;
	}

	static void mouse_callback(GLFWwindow* _main, double _xpos, double _ypos)
	{
		void *data = glfwGetWindowUserPointer(_main);
		Camera* c = static_cast<Camera*>(data);

		if (c->m_bFirstMouse)
		{
			c->m_fLastX = _xpos;
			c->m_fLastY = _ypos;
			c->m_bFirstMouse = false;
		}

		c->m_fXoffset = _xpos - c->m_fLastX;
		c->m_fYoffset = c->m_fLastY - _ypos;

		c->m_fLastX = _xpos;
		c->m_fLastY = _ypos;

		c->m_fXoffset *= c->m_sensitivity;
		c->m_fYoffset *= c->m_sensitivity;

		c->m_fYaw += c->m_fXoffset;
		c->m_fPitch += c->m_fYoffset;

		if (c->m_fPitch > 89.0f)
			c->m_fPitch = 89.0f;
		if (c->m_fPitch < -89.0f)
			c->m_fPitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(c->m_fYaw)) * cos(glm::radians(c->m_fPitch));
		front.y = sin(glm::radians(c->m_fPitch));
		front.z = sin(glm::radians(c->m_fYaw)) * cos(glm::radians(c->m_fPitch));
		c->m_cameraFront = glm::normalize(front);
	}

protected:
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;
	glm::vec3 m_cameraTarget;
	glm::vec3 m_cameraDirection;
	glm::vec3 m_cameraRight;
	glm::vec3 m_cameraUP;
	glm::vec3 m_cameraFront;
	float m_fLastY;
	float m_fLastX;
	float m_fXoffset;
	float m_fYoffset;
	float m_sensitivity;
	float m_fPitch;
	float m_fYaw;
	int m_nwidth, m_nheight;
	bool m_bFirstMouse;
};

