#include "FlyCamera.h"

/// <summary>
/// Setting default values of member variables
/// </summary>
/// <param name="_speed"> float </param>
/// <param name="_up"> glm::vec3 </param>
FlyCamera::FlyCamera(float _speed, glm::vec3 _up)
{
	m_fSpeed = _speed;
	m_fNormalSpeed = _speed;
	m_cameraUP = _up;
	m_pos = glm::vec3(-0.04f, 2.27f, 4.62f);
	m_lookDone = false;
}

FlyCamera::~FlyCamera()
{
}

/// <summary>
/// Updating the camera every frame
/// </summary>
/// <param name="_dt"> float </param>
void FlyCamera::update(float _dt)
{
	// Getting the current Window 
	GLFWwindow* win = glfwGetCurrentContext();

	// Setting speed to double if shift is pressed
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_fSpeed = (m_fNormalSpeed) * _dt;
	else
		m_fSpeed = m_fNormalSpeed * 4 * _dt;

	// Modify m_pos matrix to change the position of camera
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_pos += m_fSpeed * m_cameraFront;
	}
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_pos -= m_fSpeed * m_cameraFront;
	}
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_pos -= (glm::cross(m_cameraFront, m_cameraUP)) * m_fSpeed;
	}
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_pos += glm::normalize(glm::cross(m_cameraFront, m_cameraUP)) * m_fSpeed;
	}
	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		m_pos.y -= m_fSpeed;
	}
	if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		m_pos.y += m_fSpeed;
	}

	// If the camera has been moved
	if (!m_bFirstMouse)
	{
		// Set the world Transform and update the projectviewtransform
		setWorldTransform(glm::inverse(glm::lookAt(m_pos, m_pos + m_cameraFront, m_cameraUP)));
		updateProjectionViewTransform();
	}
	else if(!m_lookDone)
	{
		// Set the world Transform and update the projectviewtransform
		setWorldTransform(glm::inverse(glm::lookAt(m_pos, glm::vec3(0, 1.75, 0), m_cameraUP)));
		updateProjectionViewTransform();
		m_lookDone = true;
	}
}
