#pragma once
#include <tga2d/engine.h>
#include <tga2d/math/Vector3.h>
#include <tga2d/math/Transform.h>
#include <json.hpp>

#define MAX_PARTICLES 1000000

namespace Tga2D
{
	class TextureResource;
};

class SimulationData
{
public:
	friend class Emitter;
private:
	float myStartScale;
	float myEndScale;
	float myMinMass;
	float myMaxMass;
	float myMinTimeBetweenParticleSpawns;
	float myMaxTimeBetweenParticleSpawns;
	float myMinStartSpeed;
	float myMaxStartSpeed;
	float myMinAngle;
	float myMaxAngle;
	float myMinLifeTime;
	float myMaxLifeTime;
	float myMinSpawnOffsetDistance;
	float myMaxSpawnOffsetDistance;
	Tga2D::Vector3f myAcceleration;
	Tga2D::Color myStartColor;
	Tga2D::Color myEndColor;
	Tga2D::BlendState myBlendState;
	float myEmitTime;
	float myGravity;
};

struct Particle
{
	Tga2D::Vector3f myVelocity;
	Tga2D::Vector3f myPosition;
	float myTime = 0.0f;
	float myLifeTime;
	float myMass;
};

class Emitter
{
public:
	Emitter() = default;
	Emitter(nlohmann::json& aJsonConfig);
	void Update(float aDeltaTime);
	void Render();
	inline void SetTransform(Tga2D::Transform& aTransform) { myTransform = aTransform; }
private:
	void Emit(float aDeltaTime);

	float myEmitTimer = 0.0f;
	float myEmitDelayTimer = 0.0f;

	Tga2D::Transform myTransform;
	Tga2D::TextureResource* myTexture{nullptr};

	SimulationData mySimultaionData;
	std::vector<Particle> myParticles;
};