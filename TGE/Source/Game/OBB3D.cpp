#include "stdafx.h"
#include "OBB3D.h"

OBB3D::OBB3D(Vector3 aSize, Vector3 anOffset, bool aIsStatic, bool aIsTrigger, GameObject* aParent) : myParent(aParent), myOffset(anOffset), myIsTrigger(aIsTrigger)
{
    myIsStatic = aIsStatic;
    mySize = aSize;

    Calculate();
}

bool OBB3D::Collides(OBB3D& anOther)
{
    if (myParent == anOther.myParent || myIsStatic && anOther.myIsStatic) // Skip if colliders are on the same parent
        return false;

    Vector3 mtv;
    Vector3 maxTv(1.0f,1.0f,1.0f);
    float maximumOverlap = -FLT_MAX;
    float minimumOverlap = FLT_MAX;
    if (!GetMTVTranslation(anOther, mtv, minimumOverlap, maxTv, maximumOverlap))
        return false;

    if (!anOther.GetMTVTranslation(*this, mtv, minimumOverlap, maxTv, maximumOverlap))
        return false;

    Tga2D::Vector3f correction = mtv.GetNormalized() * minimumOverlap;
    Tga2D::Vector3f vecToObstacle = anOther.myTransform.GetPosition() - myTransform.GetPosition();

    if (maximumOverlap > 0)
    {
        correction = maxTv.GetNormalized() * maximumOverlap;
        Vector3 n = correction.GetNormalized();

        if (n != Vector3(1,0,0) || n != Vector3(-1,0,0) || n != Vector3(0,0,1) || n != Vector3(0,0,-1))
        {
            correction.x = 0;
            correction.z = 0;
        }
        
    }

    

    if (vecToObstacle.Dot(correction) < 0)
        correction *= -1.0f;

    SetCollisionEvent(true, anOther);

    if (myIsStatic)
    {
        anOther.myParent->GetTransform().SetPosition(anOther.myParent->GetTransform().GetPosition() + correction);
        return true;
    }

    myParent->GetTransform().SetPosition(myParent->GetTransform().GetPosition() - correction);

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
    if (aCollided && aOther.myIsTrigger)
    {
        if (myCollisionState == eCollisionState::eNone)
        {
            myCollisionState = eCollisionState::eEnter;
            myParent->OnCollisionEnter(aOther.myParent);
            return;
        }

        if (myCollisionState == eCollisionState::eEnter)
        {
            myCollisionState = eCollisionState::eStay;
            myParent->OnCollisionStay(aOther.myParent);
            return;
        }

        if (myCollisionState == eCollisionState::eStay)
        {
            myParent->OnCollisionStay(aOther.myParent);
            return;
        }

        if (myCollisionState == eCollisionState::eExit)
        {
            myCollisionState = eCollisionState::eEnter;
            myParent->OnCollisionEnter(aOther.myParent);
            return;
        }

        return;
    }

    if (aOther.myIsTrigger)
    {
        if (myCollisionState == eCollisionState::eExit)
        {
            myCollisionState = eCollisionState::eNone;
            return;
        }

        if (myCollisionState != eCollisionState::eNone)
        {
            myCollisionState = eCollisionState::eExit;
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
            SetCollisionEvent(false, aOtherObb);
            return false;
        }

        float translation = std::min(shape1Max, shape2Max) - std::max(shape1Min, shape2Min);
        if (translation < aMinTranslation)
        {
            aMinTranslation = translation;
            aMtv = normal;
        }

        if (translation > aMaxTranslation && translation < myMaxStepHeight)
        {
            if (aOtherObb.myTransform.GetMatrix().GetUp().Dot(normal) > 0)
            {
                aMaxTranslation = translation;
                aMaxTv = normal;
            }
        }
    }
    return true;
}

void OBB3D::Calculate()
{
    myNormals.clear();
    myCorners.clear();

    Vector3 up = myTransform.GetMatrix().GetUp();
    Vector3 right = myTransform.GetMatrix().GetRight();
    Vector3 forward = myTransform.GetMatrix().GetForward();

    myNormals.push_back(up * 0.5f);
    myNormals.push_back(up * -0.5f);
    myNormals.push_back(right * 0.5f);
    myNormals.push_back(right * -0.5f);
    myNormals.push_back(forward * 0.5f);
    myNormals.push_back(forward * -0.5f);

    auto pos = myTransform.GetPosition() + myOffset;

    myCorners.push_back(pos + (up * 0.5f) + (forward * 0.5f) + (right * -0.5f)); // Top forward left
    myCorners.push_back(pos + (up * 0.5f) + (forward * 0.5f) + (right * 0.5f)); // Top forward right
    myCorners.push_back(pos + (up * 0.5f) + (forward * -0.5f) + (right * -0.5f)); // Top back left
    myCorners.push_back(pos + (up * 0.5f) + (forward * -0.5f) + (right * 0.5f)); // Top back right

    myCorners.push_back(pos + (up * -0.5f) + (forward * 0.5f) + (right * -0.5f)); // Bottom forward left
    myCorners.push_back(pos + (up * -0.5f) + (forward * 0.5f) + (right * 0.5f)); // Bottom forward right
    myCorners.push_back(pos + (up * -0.5f) + (forward * -0.5f) + (right * -0.5f)); // Bottom back left
    myCorners.push_back(pos + (up * -0.5f) + (forward * -0.5f) + (right * 0.5f)); // Bottom back right
}

void OBB3D::SetTransform(Tga2D::Transform& aTransform)
{
    myTransform = aTransform;
    myTransform.SetScale(mySize);
    Calculate();
}
