#include "Transform.h"

#include "GameObject.h"

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <algorithm>

#include <cmath>

Transform::Transform() :
	m_worldPosition(),
	m_worldRotation(1,0,0,0),
	m_worldScale(1, 1, 1),
	m_localPosition(),
	m_localRotation(1, 0, 0, 0),
	m_localScale(1, 1, 1),
	m_parent(nullptr),
	m_childs()
{
}

glm::vec3 Transform::Position() const { return m_worldPosition; }
glm::quat Transform::Rotation() const { return m_worldRotation; }
glm::vec3 Transform::Scale() const { return m_worldScale; }

glm::vec3 Transform::LocalPosition() const { return m_localPosition; }
glm::quat Transform::LocalRotation() const { return m_localRotation; }
glm::vec3 Transform::LocalScale() const { return m_localScale; }

glm::vec3 Transform::Up() const { return m_worldRotation * glm::vec4(0, 0, 1, 1); }
glm::vec3 Transform::Right() const { return m_worldRotation * glm::vec4(1, 0, 0, 1); }
glm::vec3 Transform::Forward() const { return m_worldRotation * glm::vec4(0, 1, 0, 1); }

void Transform::SetPosition(const glm::vec3& position) { m_worldPosition = position; }
void Transform::SetRotation(const glm::quat& rotation) { m_worldRotation = rotation; }
void Transform::SetScale(const glm::vec3& scale) { m_worldScale = scale; }

void Transform::SetLocalPosition(const glm::vec3& position)
{
	m_localPosition = position;
}
void Transform::SetLocalRotation(const glm::quat& rotation) { m_localRotation = rotation; }
void Transform::SetLocalScale(const glm::vec3& scale) { m_localScale = scale; }

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

void Transform::SetUp(const glm::vec3& up) { m_worldRotation = FromToRotation(glm::vec3(0, 1, 0), up); }
void Transform::SetRight(const glm::vec3& right) { m_worldRotation = FromToRotation(glm::vec3(1, 0, 0), right); }
void Transform::SetForward(const glm::vec3& forward) { m_worldRotation = FromToRotation(glm::vec3(0, 0, 1), forward); }

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
	m_worldRotation *= glm::quat(glm::vec3(x, y, z));
}

void Transform::Rotate(const glm::vec3& axis, float angle)
{
	throw new std::runtime_error("not implemented");
}

void Transform::UpdateChilds()
{
	for (auto child : m_childs) {
		child->RecalculateFromLocal();
	}
}

glm::mat4 Transform::GetModel() const
{
	glm::mat4 model(1.f);

	model = glm::translate(model, Position());
	model = glm::scale(model, Scale());
	model = model * glm::toMat4(Rotation());
	return model;
}

void Transform::RecalculateFromLocal()
{
	glm::mat4 model(1.f);

	if (m_parent) {
		model = glm::translate(model, m_parent->Position());
		model = glm::scale(model, m_parent->Scale());
		model = model * glm::toMat4(m_parent->Rotation());
	}

	model = glm::translate(model, LocalPosition());
	model = glm::scale(model, LocalScale());
	model = model * glm::toMat4(LocalRotation());
	Recalculate(model);
}

void Transform::RecalculateFromWorld()
{
	Recalculate(GetModel());
}

void Transform::Recalculate(const glm::mat4& model)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(model, m_worldScale, m_worldRotation, m_worldPosition, skew, perspective);

	if (m_parent) {
		m_localPosition = m_worldPosition - m_parent->Position();
		m_localRotation = m_worldRotation - m_parent->Rotation();
		m_localScale = m_worldScale - m_parent->Scale();
	}
	else {
		m_localPosition = m_worldPosition;
		m_localRotation = m_worldRotation;
		m_localScale = m_worldScale;
	}

	UpdateChilds();
}