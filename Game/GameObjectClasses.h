#pragma once
#include "ADUserInterface.h"
#include "GameplayBaseClasses.h"
#include "ADPhysics.h"
#include <Types.h>

namespace ADResource
{
	namespace ADGameplay
	{
		enum GAME_OBJECT_TAGS
		{
			NONE = 0,
			SPYRO,
			COLLECTABLE
		};


		class Collectable : public Renderable
		{
		public:
			ADPhysics::AABB colliderSpecific;

			void SetupCollider(XMFLOAT3 position, XMFLOAT3 scale)
			{
				colliderSpecific = ADPhysics::AABB(position, scale);
				collider = &colliderSpecific;
				isTrigger = false;
			}

			virtual void CheckCollision(GameObject& _other) override
			{
				ADPhysics::Manifold m;
				if (_other.collider->ColliderCollision(&colliderSpecific, m))
				{
					ResolveCollision(_other, m);
					_other.ResolveCollision(*this, m);
				}
			};

			//Whatever this object does to the objects it touches goes here. 
			virtual void ResolveCollision(GameObject& _other, ADPhysics::Manifold& m) override
			{
				if (_other.typeID == GAME_OBJECT_TAGS::SPYRO)
				{
					ADResource::AD_UI::UIMessage message;
					message.controllerID = 1;
					message.messageType = 2;
					message.number = 1;

					ADUI::MessageReceiver::SendMessage(&message);
					//Remove();
				}
			}

			//virtual void CheckCollision(ADPhysics::AABB& _object) 
			//{
			//	if (this->active)
			//	{
			//		ADPhysics::Manifold m;

			//		if (AabbToAabbCollision(colliderSpecific, _object, m))
			//		{
			//			//XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
			//			//ADPhysics::PhysicsMaterial temp(0, 0, 0);

			//			ADResource::AD_UI::UIMessage message;
			//			message.controllerID = 1;
			//			message.messageType = 2;
			//			message.number = 1;
			//		
			//			ADUI::MessageReceiver::SendMessage(&message);
			//			Remove();
			//		}
			//	}
			//}
		};

		class Trigger : public Renderable
		{
		public:
			ADPhysics::AABB collider;

			virtual void CheckCollision(ADPhysics::AABB& _object)
			{
				if (this->active)
				{
					ADPhysics::Manifold m;

					if (AabbToAabbCollision(collider, _object, m))
					{
						//XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
						//ADPhysics::PhysicsMaterial temp(0, 0, 0);

						ADResource::AD_UI::UIMessage message;
						message.controllerID = 1;
						message.messageType = 2;
						message.number = 3;

						ADUI::MessageReceiver::SendMessage(&message);
						Remove();
					}
				}
			}
		};

		class Hitboxe : public Renderable
		{

		};

		class Destructable : public Renderable
		{

		};

		class Enemy : public Renderable
		{
			/*ADResource::AD_AI::AI ai;
			int health;
			void Update()
			{
				ai.Update();
			}
			void Damage(ADResource::ADGameplay::DAMAGE_TYPE damageType) override
			{
				if (defenseType != ADResource::ADGameplay::INVULNERABLE && defenseType != damageType)
				{
					health--;
					if (health < 1)
					{
						Remove();
					}
				}
			};*/

		public:
			ADPhysics::AABB collider;

			virtual void CheckCollision(ADPhysics::AABB& _object)
			{
				if (this->active)
				{
					ADPhysics::Manifold m;

					if (AabbToAabbCollision(collider, _object, m))
					{
						//XMFLOAT4 tempV = XMFLOAT4(0, 0, 0, 0);
						//ADPhysics::PhysicsMaterial temp(0, 0, 0);

						ADResource::AD_UI::UIMessage message;
						message.controllerID = 1;
						message.messageType = 2;
						message.number = 2;

						ADUI::MessageReceiver::SendMessage(&message);
						Remove();
					}
				}
			}
		};
	}
}