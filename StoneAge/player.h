#ifndef PLAYER_H__
#define PLAYER_H__

#include "vector3.h"
#include "transformation.h"
#include <string>

class Player {
public:
	Player();
	Player(const Vector3f& position, float rotX = 0, float rotY = 0);
	~Player();
	void TurnLeftRight(float value);
	void TurnTopBottom(float value);
	Vector3f SimulateMove(bool front, bool back, bool left, bool right, bool run, float elapsedTime);
	void ApplyTransformation(Transformation& transformation) const;
	std::string Position();
	void SetGodmode();
	Vector3f GetPosition() const;
	void SetPosition(Vector3f pos);
	float GetHeight() const;
	float GetWidth() const;
	Vector3f GetVelocity() const;
	void SetVelocity(Vector3f velocity);
	Vector3f GetGravity() const;
	void Falling(bool state);
	void SetJumping(bool state);
	bool IsJumping() const;
	void Jump();
private:
	float m_height = 1.7f;
	float m_width = .75f;
	float m_rotX = 0;
	float m_rotY = 0;
	float m_speed = 7;
	float m_run = 1.5f;
	Vector3f m_position = Vector3f(1, 80, 1);
	Vector3f m_velocity = Vector3f(0, 0, 0);
	Vector3f m_gravity = Vector3f(0, -9, 0);
	bool m_isFalling = false;
	bool m_isJumping = false;
public:
	bool m_godmode = true;
};

#endif