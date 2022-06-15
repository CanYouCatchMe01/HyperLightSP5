#include "stdafx.h"
#include "Particles.h"
#include <tga2d/drawers/SpriteDrawer.h>
#include <tga2d/sprite/sprite.h>
#include <tga2d/graphics/GraphicsEngine.h>
#include "tga2d/texture/TextureManager.h"
#include <random>
#include "tga2d/graphics/GraphicsEngine.h"
#include "tga2d/graphics/Camera.h"

namespace Util
{
	typedef struct { uint64_t state;  uint64_t inc; } pcg32_random_t;
	#pragma warning(disable:4146)
	uint32_t pcg32_random_r(pcg32_random_t* rng)
	{
		uint64_t oldstate = rng->state;
		// Advance internal state
		rng->state = oldstate * 6364136223846793005ULL + (rng->inc | 1);
		// Calculate output function (XSH RR), uses old state for max ILP
		uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
		uint32_t rot = oldstate >> 59u;
		return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
	}

	// Returns a random float between 0.0 and 1.0
	float RandomFloat()
	{
		thread_local pcg32_random_t state;
		return (float)ldexp(pcg32_random_r(&state), -32);
	}

	float RandomFloat(float aLow, float aHigh)
	{
		//std::random_device rd;
		//std::mt19937 gen(rd());
		//std::uniform_real_distribution<float> dist(aLow, aHigh);
		//return dist(gen);

		return aLow + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (aHigh - aLow)));
	}

	Tga2D::Vector3f RandomUnitVector()
	{
		float z = RandomFloat() * 2.0f - 1.0f;
		float a = RandomFloat() * 2.0f * 3.141592f;
		float r = sqrtf(1.0f - z * z);
		float x = r * cosf(a);
		float y = r * sinf(a);
		return Tga2D::Vector3f(x, y, z);
	}

	std::random_device rd;
	std::minstd_rand minstd;
	std::uniform_real_distribution<float> urd(0.f, 1.f);

	inline Tga2D::Vector3f RandomPointInSphere(float aMin, float aMax)
	{
		Tga2D::Vector3f result (
			RandomFloat(-1, 1)*RandomFloat(aMin, aMax),
			RandomFloat(-1, 1)*RandomFloat(aMin, aMax),
			RandomFloat(-1, 1)*RandomFloat(aMin, aMax)
		);

		float len = result.Length();
		while (len < aMin || len > aMax)
		{
			result = Tga2D::Vector3f (
				RandomFloat(-1, 1)*RandomFloat(aMin, aMax),
				RandomFloat(-1, 1)*RandomFloat(aMin, aMax),
				RandomFloat(-1, 1)*RandomFloat(aMin, aMax)
			);
			len = result.Length();
		}

		return result;

	}
	

	inline float Randf()
	{
		return urd(rd);
	}

	inline Tga2D::Vector3f RandPointInsideUnitSphere() // uniformly
	{
		float x = Randf() - 0.5f;
		float y = Randf() - 0.5f;
		float z = Randf() - 0.5f;
		float r = std::cbrtf(Randf()) / std::sqrtf(x * x + y * y + z * z);
		return { x * r, y * r, z * r };
	}
}

Emitter::Emitter(nlohmann::json& aJsonConfig)
{
	SimulationData& sd = mySimultaionData;
	const nlohmann::json& s = aJsonConfig;

	std::string texturePath = s["texture"];
	std::wstring wstrPath(texturePath.begin(), texturePath.end());

	myTexture = Tga2D::Engine::GetInstance()->GetTextureManager().GetTexture(wstrPath.c_str());

	sd.myStartScale = s["startScale"];
	sd.myEndScale = s["endScale"];
	sd.myMinMass = s["minMass"];
	sd.myMaxMass = s["maxMass"];
	sd.myMinTimeBetweenParticleSpawns = s["minTimeBetweenParticleSpawns"];
	sd.myMaxTimeBetweenParticleSpawns = s["maxTimeBetweenParticleSpawns"];
	sd.myMinStartSpeed = s["minStartSpeed"];
	sd.myMaxStartSpeed = s["maxStartSpeed"];
	sd.myMinAngle = s["minAngle"];
	sd.myMaxAngle = s["maxAngle"];
	sd.myMinLifeTime = s["minLifeTime"];
	sd.myMaxLifeTime = s["maxLifeTime"];
	sd.myMinSpawnOffsetDistance = s["minSpawnOffsetDistance"];
	sd.myMaxSpawnOffsetDistance = s["maxSpawnOffsetDistance"];
	sd.myAcceleration = {
		s["acceleration"]["x"],
		s["acceleration"]["y"],
		s["acceleration"]["z"]
	};
	sd.myStartColor = {
		s["color"][0]["r"],
		s["color"][0]["g"],
		s["color"][0]["b"],
		s["color"][0]["a"]
	};
	sd.myEndColor = {
		s["color"][1]["r"],
		s["color"][1]["g"],
		s["color"][1]["b"],
		s["color"][1]["a"]
	};
	sd.myBlendState = static_cast<Tga2D::BlendState>(s["blendState"]);
	sd.myEmitTime = s["emitTime"];
	sd.myGravity = s["gravity"];

}

void Emitter::Update(float aDeltaTime)
{
	Emit(aDeltaTime);

	const SimulationData& sd = mySimultaionData;

	for (size_t i = 0; i <= myParticles.size() - 1; ++i)
	{
		if (myParticles.size() <= 0)
		{
			break;
		}
		Particle& p = myParticles[i];
		if (p.myTime > p.myLifeTime)
		{
			std::swap(myParticles[i], myParticles.back());
			myParticles.pop_back();
			--i;
			continue;
		}

		p.myTime += aDeltaTime;

		p.myPosition += p.myVelocity * aDeltaTime;
		p.myVelocity += sd.myAcceleration / p.myMass * aDeltaTime;
		p.myVelocity = p.myVelocity * std::pow(0.99f, aDeltaTime) + p.myVelocity * aDeltaTime;

		p.myVelocity.y -= sd.myGravity * aDeltaTime;
	}
}

void Emitter::Render()
{
	const SimulationData& sd = mySimultaionData;
	std::vector<Tga2D::Sprite3DInstanceData> spriteInstances;
	spriteInstances.reserve(myParticles.size());

	Tga2D::Vector3f cameraPos = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetCamera().GetTransform().GetPosition();
	Tga2D::Vector3f up = myTransform.GetMatrix().GetUp();

	for (const auto& p : myParticles)
	{
		Tga2D::Sprite3DInstanceData spritedata;

		Tga2D::Vector3 toEye = (cameraPos - p.myPosition).GetNormalized();
		Tga2D::Vector3f xaxis = up.Cross(toEye).GetNormalized();
		Tga2D::Vector3f yaxis = toEye.Cross(xaxis).GetNormalized();

		spritedata.myTransform(1, 1) = xaxis.x;
		spritedata.myTransform(1, 2) = xaxis.y;
		spritedata.myTransform(1, 3) = xaxis.z;

		spritedata.myTransform(2, 1) = yaxis.x;
		spritedata.myTransform(2, 2) = yaxis.y;
		spritedata.myTransform(2, 3) = yaxis.z;

		spritedata.myTransform(3, 1) = toEye.x;
		spritedata.myTransform(3, 2) = toEye.y;
		spritedata.myTransform(3, 3) = toEye.z;

		spritedata.myColor.myR = sd.myStartColor.myR + ((sd.myEndColor.myR - sd.myStartColor.myR) * p.myTime / p.myLifeTime);
		spritedata.myColor.myG = sd.myStartColor.myG + ((sd.myEndColor.myG - sd.myStartColor.myG) * p.myTime / p.myLifeTime);
		spritedata.myColor.myB = sd.myStartColor.myB + ((sd.myEndColor.myB - sd.myStartColor.myB) * p.myTime / p.myLifeTime);
		spritedata.myColor.myA = sd.myStartColor.myA + ((sd.myEndColor.myA - sd.myStartColor.myA) * p.myTime / p.myLifeTime);

		float scale = sd.myStartScale + ((sd.myEndScale - sd.myStartScale) * p.myTime / p.myLifeTime);

		//Tga2D::Vector3f pos;
		//pos.x = p.myPosition.x;// - (scale * 0.5f);
		//pos.y = p.myPosition.y;// - (scale * 0.5f);
		//pos.z = p.myPosition.z;// - (scale * 0.5f);

		spritedata.myTransform.SetPosition(p.myPosition);

		spritedata.myTransform = Tga2D::Matrix4x4f::CreateScaleMatrix({ scale,scale,scale }) * spritedata.myTransform;

		spriteInstances.push_back(spritedata);
	}

	if (spriteInstances.size() <= 0)
		return;

	Tga2D::SpriteSharedData shareddata;
	shareddata.myTexture = myTexture;
	shareddata.myBlendState = sd.myBlendState;
	Tga2D::SpriteBatchScope batch = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetSpriteDrawer().BeginBatch(shareddata);
	
	batch.Draw(&spriteInstances.front(), spriteInstances.size());
}


void Emitter::Emit(float aDeltaTime)
{
	if (myParticles.size() >= MAX_PARTICLES)
	{
		return;
	}

	const SimulationData& sd = mySimultaionData;

	myEmitTimer += aDeltaTime;
	myEmitDelayTimer += aDeltaTime;

	const float spawnDelay = Util::RandomFloat(sd.myMinTimeBetweenParticleSpawns, sd.myMaxTimeBetweenParticleSpawns);

	while (myEmitDelayTimer >= spawnDelay && myEmitTimer < sd.myEmitTime ||
		myEmitDelayTimer >= spawnDelay && sd.myEmitTime <= 0)
	{
		myEmitDelayTimer -= spawnDelay;
		Particle p;

		//float pX = static_cast<float>(Input.GetMousePosition().x) / Tga2D::Engine::GetInstance()->GetWindowSize().myX;
		//float pY = static_cast<float>(Input.GetMousePosition().y) / Tga2D::Engine::GetInstance()->GetWindowSize().myY;
		
		p.myPosition = myTransform.GetPosition() + Util::RandomPointInSphere(sd.myMinSpawnOffsetDistance, sd.myMaxSpawnOffsetDistance);
		
		p.myMass = Util::RandomFloat(sd.myMinMass, sd.myMaxMass);
		p.myLifeTime = Util::RandomFloat(sd.myMinLifeTime, sd.myMaxLifeTime);

		//const float angle = Util::RandomFloat(sd.myMinAngle, sd.myMaxAngle);
		Tga2D::Vector3f angledVector(0, 1, 0);

		angledVector = Tga2D::Matrix3x3f::CreateRotationAroundX(Util::RandomFloat(sd.myMinAngle, sd.myMaxAngle)) * angledVector;
		angledVector = Tga2D::Matrix3x3f::CreateRotationAroundY(Util::RandomFloat(sd.myMinAngle, sd.myMaxAngle)) * angledVector;
		angledVector = Tga2D::Matrix3x3f::CreateRotationAroundZ(Util::RandomFloat(sd.myMinAngle, sd.myMaxAngle)) * angledVector;

		p.myVelocity = angledVector * Util::RandomFloat(sd.myMinStartSpeed, sd.myMaxStartSpeed);

		myParticles.push_back(p);
	}

	

	aDeltaTime;
}