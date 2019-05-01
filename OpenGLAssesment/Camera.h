#pragma once
#include<glm/ext.hpp>
#include<GLFW/glfw3.h>

class Camera
{
public:
	// Setting all the default values
	Camera() 
	{
		m_worldTransform = glm::mat4(0);
		m_viewTransform = glm::mat4(0);
		m_projectionTransform = glm::mat4(0);
		m_projectionViewTransform = glm::mat4(0);
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
		m_nwidth = 0;
		m_nheight = 0;
		
		// This sets the userPointer to this class so we can access its variable 
			// in a static function without creating the variables static (cool right)
		glfwSetWindowUserPointer(glfwGetCurrentContext(), this);

		// Setting the callback function so the function is called 
			// everytime cursor is moved
		glfwSetCursorPosCallback(glfwGetCurrentContext(), mouse_callback);
	}
	
	virtual void update(float _dt) = 0;

	/// <summary>
	/// Setting the projectTransform making it equal to glm::prespective(with variables passed in)
	/// </summary>
	/// <param name="_fieldOfView"> float </param>
	/// <param name="aspectRatio"> float </param>
	/// <param name="_near"> float </param>
	/// <param name="_far"> float </param>
	void setPerspective(float _fieldOfView, float aspectRatio, float _near, float _far)
	{
		m_projectionTransform = glm::perspective(_fieldOfView,
			aspectRatio, _near, _far);
	}

	/// <summary>
	/// Setting the viewTransform making it equal to glm::LookAt(with variables passed in)
	/// </summary>
	/// <param name="_from"> glm::vec3 </param>
	/// <param name="_to"> glm::vec3 </param>
	/// <param name="_up"> glm::vec3 </param>
	void setLookAt(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up) 
	{ 
		glm::vec3 pos(0);
		pos.x = m_worldTransform[3].x;
		pos.y = m_worldTransform[3].y;
		pos.z = m_worldTransform[3].z;
		m_viewTransform = glm::lookAt(_from, _to, _up); 
	}

	/// Setters
	void setPosition(glm::vec3 _pos){ m_worldTransform[3] = glm::vec4(_pos, 1);	}
	void setWorldTransform(glm::mat4 _world) { m_worldTransform = _world; }
	void setTarget(glm::vec3 _target) { m_cameraTarget = _target; }

	/// Getters
	glm::mat4 getWorldTransform() { return m_worldTransform; }
	glm::mat4 getViewTransform() { return m_viewTransform; }
	glm::mat4 getProjectionTransform() { return m_projectionTransform; }
	glm::mat4 getProjectionViewTransform() { return m_projectionViewTransform; }

	/// <summary>
	/// Getting the current position of the camera and calculating the m_cameraDirection
	/// normalize it also
	/// </summary>
	void calculateDirection() 
	{ 
		glm::vec3 pos(0);
		pos.x = m_worldTransform[3].x;
		pos.y = m_worldTransform[3].y;
		pos.z = m_worldTransform[3].z;
		m_cameraDirection = glm::normalize(pos - m_cameraTarget);
	}

	/// <summary>
	/// Setting up and right of the camera
	/// </summary>
	void calculateAxis()
	{
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_cameraRight = glm::normalize(glm::cross(up, m_cameraDirection));
		m_cameraUP = glm::cross(m_cameraDirection, m_cameraRight);
	}

	/// <summary>
	/// Calculating direction and axis of camera by calling calculateDirection() and calculateAxis()
	/// m_viewTransform is the inverse of the m_worldTransform
	/// while we multiply m_projectionTransform and m_viewTransform to equal it to m_projectionViewTransform
	/// </summary>
	void updateProjectionViewTransform()
	{
		calculateDirection();
		calculateAxis();
		m_viewTransform = glm::inverse(m_worldTransform);
		m_projectionViewTransform = m_projectionTransform * m_viewTransform;
	}

	/// <summary>
	/// This function is to be called when a cursor position changes 
	/// so we can change the view transform of the camera
	/// </summary>
	/// <param name="_main"> GLFWwindow* </param>
	/// <param name="_xpos"> double </param>
	/// <param name="_ypos"> double </param>
	static void mouse_callback(GLFWwindow* _main, double _xpos, double _ypos)
	{
		// Getting the current user pointer
		void *data = glfwGetWindowUserPointer(_main);
		Camera* c = static_cast<Camera*>(data);

		// if the mouse hasn't moved
			// we wanna set the viewTransform to the mouse pos
		if (c->m_bFirstMouse)
		{
			c->m_fLastX = (float)_xpos;
			c->m_fLastY = (float)_ypos;
			c->m_bFirstMouse = false;
		}

		// Calculate the offset according to the mouse pos
		c->m_fXoffset = (float)_xpos - c->m_fLastX;
		c->m_fYoffset = c->m_fLastY - (float)_ypos;

		c->m_fLastX = (float)_xpos;
		c->m_fLastY = (float)_ypos;

		c->m_fXoffset *= c->m_sensitivity;
		c->m_fYoffset *= c->m_sensitivity;

		c->m_fYaw += c->m_fXoffset;
		c->m_fPitch += c->m_fYoffset;

		if (c->m_fPitch > 89.0f)
			c->m_fPitch = 89.0f;
		if (c->m_fPitch < -89.0f)
			c->m_fPitch = -89.0f;

		// Calculating the front of the camera
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

