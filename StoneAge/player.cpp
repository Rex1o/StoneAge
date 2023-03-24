#include "player.h"
#include "vector3.h"
Player::Player()
{
}

Player::Player(const Vector3f& position, float rotX, float rotY) : m_rotX(rotX), m_rotY(rotY)
{
	m_position = position;
}

Player::~Player()
{
}

void Player::TurnLeftRight(float value)
{
	m_rotX += value / 20;
}

void Player::TurnTopBottom(float value)
{
	m_rotY += value / 20;
	if (m_rotX >= 90)
	{
		m_rotX = 90;
	}
	else if (m_rotX <= -90)
	{
		m_rotX = -90;
	}
}

Vector3f Player::SimulateMove(bool front, bool back, bool left, bool right, bool run, float elapsedTime)
{
	//angle of rotation
	float xrot = m_rotX;
	float yrot = m_rotY;
	float angle = 0.0;
	float move;

	Vector3f delta = Vector3f(0, 0, 0);

	//speed gestion
	if (run && !m_isFalling)
	{
		move = m_speed * m_run;
	}
	else if (m_isFalling) {
		move = m_speed / 2;
	}
	else {
		move = m_speed;
	}

	if (front)
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		delta.x += float(sin(yrotrad)) * elapsedTime * move;
		delta.z -= float(cos(yrotrad)) * elapsedTime * move;
		if (m_godmode) {
			delta.y -= float(sin(xrotrad)) * elapsedTime * move;
		}
	}

	if (back)
	{
		float xrotrad, yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		xrotrad = (xrot / 180 * 3.141592654f);
		delta.x -= float(sin(yrotrad)) * elapsedTime * move;
		delta.z += float(cos(yrotrad)) * elapsedTime * move;
		if (m_godmode) {
			delta.y += float(sin(xrotrad)) * elapsedTime * move;
		}
	}

	if (right)
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		delta.x += float(cos(yrotrad)) * elapsedTime * move;
		delta.z += float(sin(yrotrad)) * elapsedTime * move;
	}

	if (left)
	{
		float yrotrad;
		yrotrad = (yrot / 180 * 3.141592654f);
		delta.x -= float(cos(yrotrad)) * elapsedTime * move;
		delta.z -= float(sin(yrotrad)) * elapsedTime * move;
	}

	return delta;
}

void Player::ApplyTransformation(Transformation& transformation) const
{
	transformation.ApplyRotation(-m_rotX, 1.f, 0, 0);
	transformation.ApplyRotation(-m_rotY, 0, 1.f, 0);
	transformation.ApplyTranslation(-m_position);
}

std::string Player::Position()
{
	return " [" + std::to_string(m_position.x) + ", " + std::to_string(m_position.y) + ", " + std::to_string(m_position.z) + "]";
}

void Player::SetGodmode()
{
	if (m_godmode == true)
		m_godmode = false;
	else
		m_godmode = true;
}

Vector3f Player::GetPosition() const
{
	return m_position;
}

void Player::SetPosition(Vector3f pos) {
	m_position = pos;
}

float Player::GetHeight() const
{
	return m_height;
}

float Player::GetWidth() const
{
	return m_width;
}

Vector3f Player::GetVelocity() const
{
	return m_velocity;
}

void Player::SetVelocity(Vector3f velocity)
{
	m_velocity = velocity;
}

Vector3f Player::GetGravity() const
{
	return m_gravity;
}

void Player::Falling(bool state)
{
	m_isFalling = state;
}

void Player::SetJumping(bool state)
{
	m_isJumping = state;
}

bool Player::IsJumping() const
{
	return m_isJumping;
}

void Player::Jump() {
	if (!m_isJumping) {
		m_isJumping = true;
		m_velocity = Vector3f(0, 4.2, 0);
	}
}