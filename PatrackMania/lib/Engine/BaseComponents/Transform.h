#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "cyclone.h"

class GameObject;

class Transform
{
private:
	glm::vec3 m_worldPosition;
	glm::quat m_worldRotation;
	glm::vec3 m_worldScale;

	glm::vec3 m_localPosition;
	glm::quat m_localRotation;
	glm::vec3 m_localScale;

	Transform* m_parent;
	std::vector<Transform*> m_childs;

public:
	GameObject* gameObject;

	Transform();

	glm::vec3 Position() const;
	glm::quat Rotation() const;
	glm::vec3 Scale() const;

	glm::vec3 LocalPosition() const;
	glm::quat LocalRotation() const;
	glm::vec3 LocalScale() const;

	glm::vec3 Up() const;
	glm::vec3 Right() const;
	glm::vec3 Forward() const;

	void SetPosition(const glm::vec3 &position);
	void SetRotation(const glm::quat& rotation);
	//void SetScale(const glm::vec3& scale);

	void SetLocalPosition(const glm::vec3& position);
	void SetLocalRotation(const glm::quat& rotation);
	void SetLocalScale(const glm::vec3& scale);

	void Rotate(float x, float y, float z);
	void Rotate(const glm::vec3 &axis, float angle);

	void SetUp(const glm::vec3& up);
	void SetRight(const glm::vec3& right);
	void SetForward(const glm::vec3& forward);

	void SetParent(Transform* transform);
	std::vector<Transform*> Childs() const;

	Transform* Find(const std::string& name);
	glm::mat4 GetModel() const;

private:
	void UpdateChilds();
	void RecalculateFromLocal();
	void RecalculateFromWorld();
	void Recalculate(const glm::mat4& model);
};

