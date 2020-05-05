#pragma once
#include "GameplayBaseClasses.h"

using namespace ADResource::ADGameplay;
using namespace ADPhysics;

namespace ADResource
{
	namespace ADGameplay
	{
		class Spyro : public Renderable
		{
		
		public:
			virtual void Update(float time_delta);

			void Damage(DAMAGE_TYPE d_type);

			void Remove();
			void GetView(XMMATRIX& view);
			void CheckCollision(AABB& item);
			void CheckCollision(Plane& item);

		private:
			void HandleInput(float delta_time);

		private:
			int health;

			XMFLOAT3 spryo_movement_deltas = XMFLOAT3(0, 0, 0);

			AABB collider;
			PhysicsMaterial mat = PhysicsMaterial(1,1,0.5f);

			// Gameplay
			float jump_time_up = .4;
			float jump_time_length = 1;
			float jump_count_down = 0;
			float jump_height = 15;
			float og_y_pos = 0;
			float gravity = 50;
			bool jumping = false;
			bool gliding = false;

			float floatiness = 0.25f;
			XMFLOAT4 Velocity = XMFLOAT4(0,0,0,0);
			bool buttonup = false;
			XMMATRIX camera;
			// Turning
			float spyro_turn_speed = 5;
			float spyro_move_speed = 30;
		};
	}
}

namespace ADResource
{
	namespace ADGameplay
	{
		
	}
}