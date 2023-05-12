#include "Transform.h"

#include "GameObject.h"

#include <glm/gtx/vector_angle.hpp>
#include <algorithm>

#include <cmath>

Transform::Transform() :
	m_position(),
	m_rotation(1,0,0,0),
	m_scale(1, 1, 1),
	m_parent(nullptr),
	m_childs()
{
}

glm::vec3 Transform::Position() const { return m_position; }
glm::quat Transform::Rotation() const { return m_rotation; }
glm::vec3 Transform::Scale() const { return m_scale; }

glm::vec3 Transform::Up() const { return m_rotation * glm::vec4(0, 0, 1, 1); }
glm::vec3 Transform::Right() const { return m_rotation * glm::vec4(1, 0, 0, 1); }
glm::vec3 Transform::Forward() const { return m_rotation * glm::vec4(0, 1, 0, 1); }

void Transform::SetPosition(const glm::vec3& position) { m_position = position; }
void Transform::SetRotation(const glm::quat& rotation) { m_rotation = rotation; }
void Transform::SetScale(const glm::vec3& scale) { m_scale = scale; }

glm::quat AngleAxis(float angle, const glm::vec3 &axis)
{
	glm::vec3 nAxis = glm::normalize(axis);
	float rad = glm::radians(angle * 0.5f);
	nAxis *= std::sin(rad);
	return glm::quat(nAxis.x, nAxis.y, nAxis.z, std::cos(rad));
}

glm::quat FromToRotation(glm::vec3 aFrom, glm::vec3 aTo)
{
	glm::vec3 axis = glm::cross(aFrom, aTo);
	float angle = glm::angle(aFrom, aTo);
	return glm::angleAxis(angle, glm::normalize(axis));
}

void Transform::SetUp(const glm::vec3& up) { m_rotation = FromToRotation(glm::vec3(0, 1, 0), up); }
void Transform::SetRight(const glm::vec3& right) { m_rotation = FromToRotation(glm::vec3(1, 0, 0), right); }
void Transform::SetForward(const glm::vec3& forward) { m_rotation = FromToRotation(glm::vec3(0, 0, 1), forward); }

void Transform::SetParent(Transform* parent)
{
	if (parent)
	{
		m_parent = parent;
		parent->m_childs.push_back(this);
	}
	else if (m_parent)
	{
		auto position = std::find(
			m_parent->m_childs.begin(), 
			m_parent->m_childs.end(), 
			this
		);
		if (position != m_parent->m_childs.end()) {
			parent->m_childs.erase(position);
		}
		m_parent = nullptr;
	}
}

std::vector<Transform*> Transform::Childs() const
{
	return m_childs;
}


Transform* Transform::Find(const std::string& name)
{
	if (gameObject->name == name) return this;

	for (auto child : m_childs) {
		Transform *tmp = child->Find(name);
		if (tmp) return tmp;
	}
	return nullptr;
}

void Transform::Rotate(float x, float y, float z)
{
	m_rotation *= glm::quat(glm::vec3(x, y, z));
}

void Transform::Rotate(const glm::vec3& axis, float angle)
{
	throw new std::runtime_error("not implemented");
}
