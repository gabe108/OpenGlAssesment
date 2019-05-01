#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	/// <summary>
	/// Setting default values of member variables
	/// </summary>
	/// <param name="_speed"> float </param>
	/// <param name="_up"> glm::vec3 </param>
	FlyCamera(float _speed, glm::vec3 _up);
	~FlyCamera();

	/// <summary>
	/// Updating the camera every frame
	/// </summary>
	/// <param name="_dt"> float </param>
	virtual void update(float _dt);

	/// Setters
	void setSpeed(float _speed) { m_fSpeed = _speed; }
	
	/// Getters
	float getSpeed() { return m_fSpeed; }
	glm::vec3 getUP() { return m_cameraUP; }
	glm::vec3 getPos() { return m_pos; }

protected:
	float m_fSpeed;
	float m_fNormalSpeed;
	bool m_lookDone;
	glm::vec3 m_pos;
};

