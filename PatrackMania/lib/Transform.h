#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "cyclone.h"

class GameObject;

class Transform
{
private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;

	Transform* m_parent;
	std::vector<Transform*> m_childs;

public:
	GameObject* gameObject;

	Transform();

	glm::vec3 Position() const;
	glm::quat Rotation() const;
	glm::vec3 Scale() const;

	glm::vec3 Up() const;
	glm::vec3 Right() const;
	glm::vec3 Forward() const;

	void SetPosition(const glm::vec3 &position);
	void SetRotation(const glm::quat& rotation);
	void Rotate(float x, float y, float z);
	void Rotate(const glm::vec3 &axis, float angle);
	void SetScale(const glm::vec3& scale);

	void SetUp(const glm::vec3& up);
	void SetRight(const glm::vec3& right);
	void SetForward(const glm::vec3& forward);

	void SetParent(Transform* transform);
	std::vector<Transform*> Childs() const;

	Transform* Find(const std::string& name);
};

