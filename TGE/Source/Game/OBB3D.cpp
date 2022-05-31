#include "stdafx.h"
#include "OBB3D.h"
#include <tga2d/graphics/GraphicsEngine.h>
#include <tga2d/graphics/Camera.h>

OBB3D::OBB3D(Vector3 aSize, Vector3 anOffset, bool aIsStatic, bool aIsTrigger, GameObject* aParent) : myParent(aParent), myOffset(anOffset), myIsTrigger(aIsTrigger)
{
    myIsStatic = aIsStatic;
    mySize = aSize;

    myLastPos = myParent->GetTransform().GetPosition();

    Calculate();
}

bool OBB3D::Collides(OBB3D& anOther)
{
    if (myParent == anOther.myParent || myIsStatic && anOther.myIsStatic) // Skip if colliders are on the same parent
        return false;

    Vector3 mtv;
    Vector3 maxTv;
    float maximumOverlap = -FLT_MAX;
    float minimumOverlap = FLT_MAX;

    if (!GetMTVTranslation(anOther, mtv, minimumOverlap, maxTv, maximumOverlap))
    {
        SetCollisionEvent(false, anOther);
        anOther.SetCollisionEvent(false, *this);
        return false;
    }
        

    if (!anOther.GetMTVTranslation(*this, mtv, minimumOverlap, maxTv, maximumOverlap))
    {
        SetCollisionEvent(false, anOther);
        anOther.SetCollisionEvent(false, *this);
        return false;
    }

    SetCollisionEvent(true, anOther);
    anOther.SetCollisionEvent(true, *this);

    if (myIsTrigger || anOther.myIsTrigger)
    {
        return true;
    }

    Tga2D::Vector3f correction = mtv * minimumOverlap;
    Tga2D::Vector3f vecToObstacle = anOther.myTransform.GetPosition() - myTransform.GetPosition();


     //OLD
    /*if (minimumOverlap > 0)
    {
        correction = maxTv * maximumOverlap;
        Vector3 n = correction.GetNormalized();

        if (n != Vector3(1,0,0) || n != Vector3(-1,0,0) || n != Vector3(0,0,1) || n != Vector3(0,0,-1))
        {
            correction.x = 0;
            correction.z = 0;
        }
    }*/

    if (maximumOverlap <= myMaxStepHeight && maximumOverlap > 0)
        correction = maxTv * maximumOverlap;

    if (vecToObstacle.Dot(correction) < 0)
        correction *= -1.0f;

    // NEW FIX FOR SLOPES AND EDGES, check if not a level plane and if the length is smaller than max step height

    //bool walkingIntoWall = myTransform.GetMatrix().GetUp().GetNormalized().Dot(correction.GetNormalized()) == 0;

    Vector3 thisUp = myTransform.GetMatrix().GetUp();
    Vector3 thatUp = anOther.myTransform.GetMatrix().GetUp();

    float dot = thisUp.GetNormalized().Dot(thatUp.GetNormalized());
    bool slope = dot < 1.5f && dot > 0.5f && !(dot > 0.999f && dot < 1.001f);



    bool stair = maximumOverlap <= myMaxStepHeight && maximumOverlap > 0 && !slope;
    stair = maximumOverlap <= myMaxStepHeight && maximumOverlap > 0 && !slope;

    //std::cout << slope << '\n';

    if (slope && !stair)
    {
        correction.x = 0;
        correction.z = 0;
    }
    /*else if (stair)
    {
        correction = maxTv * maximumOverlap;
        if (vecToObstacle.Dot(correction) < 0)
            correction *= -1.0f;
    }*/

    if (myIsStatic)
    {
        anOther.myParent->GetTransform().SetPosition(anOther.myParent->GetTransform().GetPosition() + correction);
        anOther.SetPosition(anOther.myTransform.GetPosition() + correction);
        return true;
    }
    myParent->GetTransform().SetPosition(myParent->GetTransform().GetPosition() - correction);
    SetPosition(myTransform.GetPosition() - correction);
    return true;
}

void OBB3D::SetPosition(Tga2D::Vector3f aPos)
{
    myTransform.SetPosition(aPos);
    Calculate();
}

void OBB3D::SATTest(const Vector3 anAxis, const SetOfCorners& aPtSet, float& aMinExtent, float& aMaxExtent)
{
    aMinExtent = FLT_MAX;
    aMaxExtent = -FLT_MAX;

    for (auto& point : aPtSet)
    {
        float dotVal = point.Dot(anAxis);
        aMinExtent = std::min(aMinExtent, dotVal);
        aMaxExtent = std::max(aMaxExtent, dotVal);
    }
}

bool OBB3D::Overlaps(float aMin1, float aMax1, float aMin2, float aMax2)
{
    if (aMax2 >= aMin1 && aMax1 >= aMin2) { return true; }
    return false;
}

void OBB3D::SetCollisionEvent(bool aCollided, OBB3D& aOther)
{
    if (!aOther.myIsTrigger || myIsTrigger && !myAlwaysSendEvent)
        return;

    eCollisionState state = myCurrentlyColliding[aOther.myParent];
    if (aCollided && aOther.myIsTrigger)
    {
        if (state == eCollisionState::eNone)
        {
            myCurrentlyColliding[aOther.myParent] = eCollisionState::eEnter;
            myParent->OnCollisionEnter(aOther.myParent);
            return;
        }

        if (state == eCollisionState::eEnter)
        {
            state = eCollisionState::eStay;
            myParent->OnCollisionStay(aOther.myParent);
            return;
        }

        if (state == eCollisionState::eStay)
        {
            myParent->OnCollisionStay(aOther.myParent);
            return;
        }

        if (state == eCollisionState::eExit)
        {
            myCurrentlyColliding[aOther.myParent] = eCollisionState::eEnter;
            myParent->OnCollisionEnter(aOther.myParent);
            return;
        }

        return;
    }

    else if (!aCollided && aOther.myIsTrigger)
    {
        if (state == eCollisionState::eExit)
        {
            myCurrentlyColliding[aOther.myParent] = eCollisionState::eNone;
            return;
        }

        if (state == eCollisionState::eEnter || myCurrentlyColliding[aOther.myParent] == eCollisionState::eStay)
        {
            myCurrentlyColliding[aOther.myParent] = eCollisionState::eExit;
            myParent->OnCollisionExit(aOther.myParent);
        }
    }
}

bool OBB3D::GetMTVTranslation(OBB3D& aOtherObb, Vector3& aMtv, float& aMinTranslation, Vector3& aMaxTv, float& aMaxTranslation)
{
    for (const Vector3& normal : myNormals)
    {
        float shape1Min, shape1Max, shape2Min, shape2Max;
        SATTest(normal, myCorners, shape1Min, shape1Max);
        SATTest(normal, aOtherObb.myCorners, shape2Min, shape2Max);
        if (!Overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
        {
            return false;
        }

        float translation = std::min(shape1Max, shape2Max) - std::max(shape1Min, shape2Min);
        if (translation < aMinTranslation)
        {
            aMinTranslation = translation;
            aMtv = normal;
        }

        aMaxTranslation;
        aMaxTv;

        //OLD
        /*if (translation > aMaxTranslation && translation < myMaxStepHeight)
        {
            if (aOtherObb.myTransform.GetMatrix().GetUp().GetNormalized().Dot(normal) > 0)
            {
                aMaxTranslation = translation;
                aMaxTv = normal;
            }
        }*/
    }

    {
        Vector3 up(0, 1, 0);
        float shape1Min, shape1Max, shape2Min, shape2Max;
        SATTest(up, myCorners, shape1Min, shape1Max);
        SATTest(up, aOtherObb.myCorners, shape2Min, shape2Max);

        float translation = std::min(shape1Max, shape2Max) - std::max(shape1Min, shape2Min);
        if (translation > aMinTranslation && translation < myMaxStepHeight)
        {
            aMaxTranslation = translation;
            aMaxTv = up;
        }   
    }
    

    return true;
}

void OBB3D::Calculate()
{
    myNormals.clear();
    myCorners.clear();

    Tga2D::Vector3f scale(
        myParent->GetTransform().GetScale().x,
        myParent->GetTransform().GetScale().y,
        myParent->GetTransform().GetScale().z
    );

    myTransform.SetScale(scale);

    Vector3 up = myTransform.GetMatrix().GetUp() * mySize.y;
    Vector3 right = myTransform.GetMatrix().GetRight() * mySize.x;
    Vector3 forward = myTransform.GetMatrix().GetForward() * mySize.z;

    myNormals.push_back(up * 0.5f);
    myNormals.push_back(up * -0.5f);
    myNormals.push_back(right * 0.5f);
    myNormals.push_back(right * -0.5f);
    myNormals.push_back(forward * 0.5f);
    myNormals.push_back(forward * -0.5f);


    /*Tga2D::Vector3f offset(
        myOffset.x / myParent->GetTransform().GetScale().x,
        myOffset.y / myParent->GetTransform().GetScale().y,
        myOffset.z / myParent->GetTransform().GetScale().z
    );*/

    Tga2D::Vector3f pos(Tga2D::Vector4f(myOffset, 1) * myTransform.GetMatrix());

    //auto pos = myTransform.GetPosition() + myOffset;

    myCorners.push_back(pos + (up * 0.5f) + (forward * 0.5f) + (right * -0.5f)); // Top forward left
    myCorners.push_back(pos + (up * 0.5f) + (forward * 0.5f) + (right * 0.5f)); // Top forward right
    myCorners.push_back(pos + (up * 0.5f) + (forward * -0.5f) + (right * -0.5f)); // Top back left
    myCorners.push_back(pos + (up * 0.5f) + (forward * -0.5f) + (right * 0.5f)); // Top back right

    myCorners.push_back(pos + (up * -0.5f) + (forward * 0.5f) + (right * -0.5f)); // Bottom forward left
    myCorners.push_back(pos + (up * -0.5f) + (forward * 0.5f) + (right * 0.5f)); // Bottom forward right
    myCorners.push_back(pos + (up * -0.5f) + (forward * -0.5f) + (right * -0.5f)); // Bottom back left
    myCorners.push_back(pos + (up * -0.5f) + (forward * -0.5f) + (right * 0.5f)); // Bottom back right

    for (auto& n : myNormals)
        n = n.GetNormalized();
}

void OBB3D::SetTransform(Tga2D::Transform& aTransform)
{
    if (myTransform == aTransform)
        return;

    myLastPos = aTransform.GetPosition();
    myTransform = aTransform;
    Calculate();
}

#ifdef _DEBUG
void OBB3D::Draw()
{
    if (!myDrawHitbox) return;

    auto& camera = Tga2D::Engine::GetInstance()->GetGraphicsEngine().GetCamera();
    for (auto& corner : myCorners)
    {
        auto pos1InCameraSpace = Tga2D::Matrix4x4f::GetFastInverse(camera.GetTransform().GetMatrix()) * corner;
        auto pos1 = camera.GetProjection() * pos1InCameraSpace;
        if (pos1.w != 0.0f)
        {
            pos1.x /= pos1.w;
            pos1.y /= pos1.w;
            pos1.z /= pos1.w;
        }

        // 0.5f, because the projection is from -1 -> 1, but Engine is from 0 -> 1
        Tga2D::Vector2f cornerPos1 = { pos1.x * 0.5f + 0.5f, pos1.y * -0.5f + 0.5f };
        //Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawLine({ renderPos.x, renderPos.y }, { renderPos.x + 0.2f, renderPos.y });

        for (auto& corner2 : myCorners)
        {
            auto pos2InCameraSpace = Tga2D::Matrix4x4f::GetFastInverse(camera.GetTransform().GetMatrix()) * corner2;
            auto pos2 = camera.GetProjection() * pos2InCameraSpace;
            if (pos2.w != 0.0f)
            {
                pos2.x /= pos2.w;
                pos2.y /= pos2.w;
                pos2.z /= pos2.w;
            }

            Tga2D::Vector2f cornerPos2 = { pos2.x * 0.5f + 0.5f, pos2.y * -0.5f + 0.5f };
            Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawLine(cornerPos1, cornerPos2, Tga2D::Color(0,1,0,1));
        }
    }

    Tga2D::Vector3f colliderPos(Tga2D::Vector4f(myOffset, 1) * myTransform.GetMatrix());
    for (auto& normal : myNormals)
    {
        auto posInCameraSpace = Tga2D::Vector4f(colliderPos + normal, 1) * Tga2D::Matrix4x4f::GetFastInverse(camera.GetTransform().GetMatrix());
        auto pos = posInCameraSpace * camera.GetProjection();
        
        if (pos.w != 0.0f)
        {
            pos.x /= pos.w;
            pos.y /= pos.w;
            pos.z /= pos.w;
        }

        auto arrowHeadPos = Tga2D::Vector4f(colliderPos + normal + normal.GetNormalized(), 1) * Tga2D::Matrix4x4f::GetFastInverse(camera.GetTransform().GetMatrix());
        arrowHeadPos = arrowHeadPos * camera.GetProjection();

        if (arrowHeadPos.w != 0.0f)
        {
            arrowHeadPos.x /= arrowHeadPos.w;
            arrowHeadPos.y /= arrowHeadPos.w;
            arrowHeadPos.z /= arrowHeadPos.w;
        }

        Tga2D::Vector2f arrowHeadPosTransformed = { arrowHeadPos.x * 0.5f + 0.5f, arrowHeadPos.y * -0.5f + 0.5f };
        Tga2D::Vector2f transformed = { pos.x * 0.5f + 0.5f, pos.y * -0.5f + 0.5f };
        Tga2D::Engine::GetInstance()->GetDebugDrawer().DrawArrow(transformed, arrowHeadPosTransformed, Tga2D::Color(1, 0, 0, 1), 0.35f);
    }

}
#endif // _DEBUG