#pragma once
#include "Camera.h"

class FlyCamera : public Camera
{
public:
	FlyCamera(float _speed, glm::vec3 _up);
	~FlyCamera();

	virtual void update(float _dt);
	void setSpeed(float _speed) { m_fSpeed = _speed; }
	
	float getSpeed() { return m_fSpeed; }
	glm::vec3 getUP() { return m_cameraUP; }
	glm::vec3 getPos() { return m_pos; }

protected:
	float m_fSpeed;
	float m_fNormalSpeed;
	glm::vec3 m_pos;
};

