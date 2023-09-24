#include "Physics.h"
#include "../BaseComponents/RigidBody.h"
#include <glm/gtx/quaternion.hpp>

using namespace Module;

void Physics::Init()
{
	forceRegistry = new cyclone::ForceRegistry();
}

void Physics::Update(float deltaTime, const std::vector<GameObject*>& objects)
{
	Console.Log("hello world");
	forceRegistry->updateForces(deltaTime);
	int nbodies = 0;
	std::vector<cyclone::CollisionBox*> boxes;
	for (auto object : objects)
	{
		object->ApplyOnHierarchy([&](GameObject* obj) {
			if (!obj->activeInHierarchy) return;
			RigidBody* rb = obj->GetComponent<RigidBody>();
			if (rb) {
				rb->Update(deltaTime);
				nbodies += 1;
				boxes.push_back(rb->boxCollider);
			}
		});
	}
	unsigned MaxContacts = (nbodies * (nbodies - 1)) / 2;
	contacts.resize(MaxContacts);
	resolver = new cyclone::ContactResolver(MaxContacts);

	unsigned limit = MaxContacts;
	auto collision = new cyclone::CollisionData();
	collision->contacts = contacts.data();
	for (auto b1 : boxes)
	{
		for (auto b2 : boxes)
		{
			if (b1 == b2) continue;
			unsigned used = cyclone::CollisionDetector::boxAndBox(*b1, *b2, collision);
			limit -= used;
			if (limit <= 0) break;
		}
		if (limit <= 0) break;
	}
	int num = MaxContacts - limit;
	if (num > 0) {
		resolver->setIterations(num * MaxContacts);
		resolver->resolveContacts(contacts.data(), num, deltaTime);
	}
	for (auto object : objects)
	{
		object->ApplyOnHierarchy([&](GameObject* obj) {
			if (!obj->activeInHierarchy) return;
			RigidBody* rb = obj->GetComponent<RigidBody>();
			if (rb) {
				cyclone::Vector3 p = rb->rigidBody->getPosition();
				cyclone::Vector3 r = rb->rigidBody->getRotation();
				rb->transform->SetPosition(glm::vec3(p.x, p.y, p.z));
				rb->transform->SetLocalRotation(glm::quat(glm::vec3(r.x, r.y, r.z)));
				rb->Update(deltaTime);
				nbodies += 1;
				boxes.push_back(rb->boxCollider);
			}
		});
	}
}
