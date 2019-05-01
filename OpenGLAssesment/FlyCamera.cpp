#include "FlyCamera.h"

FlyCamera::FlyCamera(float _speed, glm::vec3 _up)
{
	m_fSpeed = _speed;
	m_fNormalSpeed = _speed;
	m_cameraUP = _up;
	m_pos = glm::vec3(20.0f, 0.0f, 20.0f);
}

FlyCamera::~FlyCamera()
{
}

void FlyCamera::update(float _dt)
{
	GLFWwindow* win = glfwGetCurrentContext();

	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_fSpeed = (m_fNormalSpeed * 4) * _dt;
	else
		m_fSpeed = m_fNormalSpeed * _dt;

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

	if (!m_bFirstMouse)
	{
		setWorldTransform(glm::inverse(glm::lookAt(m_pos, m_pos + m_cameraFront, m_cameraUP)));
		updateProjectionViewTransform();
	}
	else
	{
		setWorldTransform(glm::inverse(glm::lookAt(m_pos, glm::vec3(0), m_cameraUP)));
		updateProjectionViewTransform();
	}
}
