using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[System.Serializable]
public enum BlendState
{
    Disabled,
    AlphaBlend,
    AdditiveBlend,
    Count
};

public class Emitter : MonoBehaviour
{
    // Start is called before the first frame update

    public Texture2D texture;
    public float startScale;
    public float endScale;
    public float minMass;
    public float maxMass;
    public float minTimeBetweenParticleSpawns;
    public float maxTimeBetweenParticleSpawns;
    public float minStartSpeed;
    public float maxStartSpeed;
    public float minAngle;
    public float maxAngle;
    public float minLifeTime;
    public float maxLifeTime;
    public float minSpawnOffsetDistance;
    public float maxSpawnOffsetDistance;
    public Vector3 acceleration;
    public Color startColor;
    public Color endColor;
    public BlendState blendState;
    public float emitTime;
    public float gravity;


    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
