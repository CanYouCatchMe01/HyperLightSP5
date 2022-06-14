using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using Newtonsoft.Json.Linq;
using UnityEditor;

class Converter
{
    public static int GetTagIndex(string aTag)
    {
        for (int i = 0; i < UnityEditorInternal.InternalEditorUtility.tags.Length; i++)
        {
            if (UnityEditorInternal.InternalEditorUtility.tags[i] == aTag)
            {
                return i;
            }
        }

        Debug.LogError("Tag not found: " + aTag);
        return 0; //return 0, so the TGE game will still run.
    }
    
    public static JObject ConvertToJSON(GameObject aGameObject)
    {
        JObject JGameObject = new JObject();

        //Every game object have the following properties
        JGameObject.Add("name", aGameObject.name);

        //Get the tag index and layer index
        JGameObject.Add("tag", GetTagIndex(aGameObject.tag));
        JGameObject.Add("layer", aGameObject.layer);

        JGameObject.Add("is_static", aGameObject.isStatic);
        JGameObject.Add("transform", ConvertToJSON(aGameObject.transform));

        //Components are added to an array
        JArray JComponents = new JArray();
        foreach (Component component in aGameObject.GetComponents<Component>())
        {
            JObject JComponent = ConvertToJSON(component);

            //Check if JObject containes anything, then add it to the array
            if (JComponent.Count > 0)
                JComponents.Add(JComponent);

        }
        JGameObject.Add("components", JComponents);

        //Recursively add children to the object
        JArray JChildren = new JArray();
        foreach (Transform child in aGameObject.transform)
        {
            if (!child.gameObject.activeSelf)
                continue;

            JChildren.Add(ConvertToJSON(child.gameObject));
        }
        JGameObject.Add("children", JChildren);

        return JGameObject;
    }

    public static JObject ConvertToJSON(Emitter aEmitter)
    {
        string texturePath = "";
        texturePath = AssetDatabase.GetAssetPath(aEmitter.texture);
        Export.AddDependency(texturePath);

        JObject result = new JObject(
            new JProperty("texture", Path.ChangeExtension(texturePath, ".dds")),
            new JProperty("startScale", aEmitter.startScale),
            new JProperty("endScale", aEmitter.endScale),
            new JProperty("minMass", aEmitter.minMass),
            new JProperty("maxMass", aEmitter.maxMass),
            new JProperty("minTimeBetweenParticleSpawns", aEmitter.minTimeBetweenParticleSpawns),
            new JProperty("maxTimeBetweenParticleSpawns", aEmitter.maxTimeBetweenParticleSpawns),
            new JProperty("minStartSpeed", aEmitter.minStartSpeed),
            new JProperty("maxStartSpeed", aEmitter.maxStartSpeed),
            new JProperty("minAngle", aEmitter.minAngle),
            new JProperty("maxAngle", aEmitter.maxAngle),
            new JProperty("minLifeTime", aEmitter.minLifeTime),
            new JProperty("maxLifeTime", aEmitter.maxLifeTime),
            new JProperty("minSpawnOffsetDistance", aEmitter.minSpawnOffsetDistance),
            new JProperty("maxSpawnOffsetDistance", aEmitter.maxSpawnOffsetDistance),
            new JProperty("acceleration", ConvertToJSON(aEmitter.acceleration)),
            new JProperty("color",
                new JArray(
                    ConvertToJSON(aEmitter.startColor),
                    ConvertToJSON(aEmitter.endColor)
                )
            ),
            new JProperty("blendState", aEmitter.blendState),
            new JProperty("emitTime", aEmitter.emitTime),
            new JProperty("gravity", aEmitter.gravity)
        );

        return result;
    }

    public static JObject ConvertToJSON(Vector3 aVector3)
    {
        JObject result = new JObject(
            new JProperty("x", aVector3.x),
            new JProperty("y", aVector3.y),
            new JProperty("z", aVector3.z)
        );

        return result;
    }

    public static JObject ConvertToJSON(Color aColor)
    {
        JObject result = new JObject(
            new JProperty("r", aColor.r),
            new JProperty("g", aColor.g),
            new JProperty("b", aColor.b),
            new JProperty("a", aColor.a)
        );

        return result;
    }

    public static JObject ConvertToJSON(Quaternion aQuaternion)
    {
        JObject result = new JObject(
            new JProperty("x", aQuaternion.x),
            new JProperty("y", aQuaternion.y),
            new JProperty("z", aQuaternion.z),
            new JProperty("w", aQuaternion.w)
        );

        return result;
    }

    public static JObject ConvertToJSON(Light aLight)
    {
        JObject result = new JObject();
        result.Add("type", (int)aLight.type);

        switch (aLight.type)
        {
            case LightType.Directional:
                {
                    result.Add("color", ConvertToJSON(aLight.color));
                    result.Add("intensity", aLight.intensity);
                    break;
                }
            case LightType.Point:
                {
                    result.Add("color", ConvertToJSON(aLight.color));
                    result.Add("intensity", aLight.intensity);
                    result.Add("range", aLight.range);
                    break;
                }
        }

        return result;
    }

    public static JObject ConvertToJSON(Camera aCamera)
    {
        JObject result = new JObject(
            new JProperty("fov", aCamera.fieldOfView)
        );

        return result;
    }

    public static JObject ConvertToJSON(Transform aTransform)
    {
        JObject result = new JObject();

        result.Add("position", ConvertToJSON(aTransform.position));
        result.Add("rotation", ConvertToJSON(aTransform.rotation));
        result.Add("scale", ConvertToJSON(aTransform.lossyScale));

        return result;
    }

    public static JObject ConvertToJSON(Component aComponent)
    {
        JObject result = new JObject();

        if (aComponent.GetType() == typeof(MeshFilter))
        {
            if (aComponent.GetComponent<Animation>() != null)
                return result;

            result.Add("type", "mesh");
            result.Add("data", ConvertToJSON((MeshFilter)aComponent));
        }
        else if (aComponent.GetType() == typeof(BoxCollider))
        {
            result.Add("type", "box_collider");
            result.Add("data", ConvertToJSON((BoxCollider)aComponent));
        }
        else if (aComponent.GetType() == typeof(Light))
        {
            result.Add("type", "light");
            result.Add("data", ConvertToJSON((Light)aComponent));
        }
        else if (aComponent.GetType() == typeof(ClassContainer))
        {
            result = ConvertToJSON((ClassContainer)aComponent);
        }
        else if (aComponent.GetType() == typeof(Camera))
        {
            result.Add("type", "camera");
            result.Add("data", ConvertToJSON((Camera)aComponent));
        }
        else if (aComponent.GetType() == typeof(Animation))
        {
            result.Add("type", "animated_mesh");
            result.Add("data", ConvertToJSON((Animation)aComponent));
        }
        else if (aComponent.GetType() == typeof(Teleporter))
        {
            result.Add("type", "teleporter");
            result.Add("data", ConvertToJSON((Teleporter)aComponent));
        }
        else if (aComponent.GetType() == typeof(SpawnPoint))
        {
            result.Add("type", "spawn_point");
            result.Add("data", ConvertToJSON((SpawnPoint)aComponent));
        }
        else if (aComponent.GetType() == typeof(CheckPoint))
        {
            result.Add("type", "check_point");
            result.Add("data", ConvertToJSON((CheckPoint)aComponent));
        }
        else if (aComponent.GetType() == typeof(Emitter))
        {
            result.Add("type", "emitter");
            result.Add("data", ConvertToJSON((Emitter)aComponent));
        }

        return result;
    }
    public static JObject ConvertToJSON(CheckPoint aCheckPoint)
    {
        JObject result = new JObject();

        if (!aCheckPoint.spawnPointScene)
            Debug.Log("Checkpoint on " + aCheckPoint.gameObject.name + " does not have a scene to load");

        if (aCheckPoint.spawnPointName == "")
            Debug.Log("Checkpoint on " + aCheckPoint.gameObject.name + " does not have a spawnpoint name");

        result.Add("scene", aCheckPoint.spawnPointScene.name);
        result.Add("name", aCheckPoint.spawnPointName);

        return result;
    }
    public static JObject ConvertToJSON(SpawnPoint aSpawnPoint)
    {
        JObject result = new JObject();

        if (aSpawnPoint.spawnPointName == "")
            Debug.Log("Spawnpoint on " + aSpawnPoint.gameObject.name + " does not have a spawnpoint name");

        result.Add("name", aSpawnPoint.spawnPointName);

        return result;
    }
    public static JObject ConvertToJSON(Teleporter aTeleporter)
    {
        JObject result = new JObject();

        if (!aTeleporter.sceneToLoad)
            Debug.Log("Teleporter on " + aTeleporter.gameObject.name + " does not have a scene to load");

        if (aTeleporter.spawnPointName == "")
            Debug.Log("Teleporter on " + aTeleporter.gameObject.name + " does not have a spawnpoint name");

        result.Add("scene", aTeleporter.sceneToLoad.name);
        result.Add("spawnpoint", aTeleporter.spawnPointName);

        return result;
    }

    public static JObject ConvertToJSON(Animation aAnimation)
    {
        JObject result = new JObject();

        Mesh mesh = aAnimation.gameObject.GetComponent<MeshFilter>().sharedMesh;
        MeshRenderer meshRenderer = aAnimation.gameObject.GetComponent<MeshRenderer>();

        if (!mesh || !meshRenderer)
            return result;

        string meshPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(mesh));


        string albedoPath = "";
        string normalPath = "";
        string reflectivePath = "";

        if (mesh && meshRenderer)
        {
            Material material = meshRenderer.sharedMaterial;
            var albedo = material.GetTexture("_MainTex");
            var normal = material.GetTexture("_BumpMap");
            var reflective = material.GetTexture("_MetallicGlossMap");

            albedoPath = AssetDatabase.GetAssetPath(albedo);
            normalPath = AssetDatabase.GetAssetPath(normal);
            reflectivePath = AssetDatabase.GetAssetPath(reflective);

            Export.AddDependency(albedoPath);
            Export.AddDependency(normalPath);
            Export.AddDependency(reflectivePath);
        }

        result.Add("albedo", Path.ChangeExtension(albedoPath, ".dds"));
        result.Add("normal", Path.ChangeExtension(normalPath, ".dds"));
        result.Add("reflective", Path.ChangeExtension(reflectivePath, ".dds"));

        JArray animations = new JArray();

        if (aAnimation)
        {
            foreach (AnimationState item in aAnimation)
            {
                JObject animation = new JObject();
                string path = AssetDatabase.GetAssetPath(item.clip);
                animation.Add("path", path);
                animation.Add("name", item.name);
                animations.Add(animation);
                Export.AddDependency(path);
            }
        }

        Export.AddDependency(meshPath);
        result.Add("model", meshPath);

        result.Add("animations", animations);

        return result;
    }

    public static JObject ConvertToJSON(MeshFilter aMeshFilter)
    {
        JObject result = new JObject();

        if (!aMeshFilter.sharedMesh)
            Debug.Log(aMeshFilter.gameObject.name + " is missing a mesh, could not export");
        
        var obj = PrefabUtility.GetCorrespondingObjectFromOriginalSource(aMeshFilter.sharedMesh);
        if (!obj)
            Debug.Log("Error in: " + aMeshFilter.gameObject.name);

        string modelPath = AssetDatabase.GetAssetPath(obj);

        if (modelPath == "Library/unity default resources")
        {
            Debug.LogError(aMeshFilter.gameObject.name + " has a mesh that is not created by the 'grafiker', add a mesh from the Art Folder");
            modelPath = "";
            return result;
        }

        result.Add("model", modelPath);
        Export.AddDependency(modelPath);

        string albedoPath = "";
        string normalPath = "";
        string reflectivePath = "";

        //Getting material properties
        MeshRenderer meshRenderer = aMeshFilter.gameObject.GetComponent<MeshRenderer>();
        if (meshRenderer && meshRenderer.sharedMaterial)
        {
            Material material = meshRenderer.sharedMaterial;
            var albedo = material.GetTexture("_MainTex");
            var normal = material.GetTexture("_BumpMap");
            var reflective = material.GetTexture("_MetallicGlossMap");

            //Getting the path of the textures
            albedoPath = AssetDatabase.GetAssetPath(albedo);
            normalPath = AssetDatabase.GetAssetPath(normal);
            reflectivePath = AssetDatabase.GetAssetPath(reflective);
            
            Export.AddDependency(albedoPath);
            Export.AddDependency(normalPath);
            Export.AddDependency(reflectivePath);

            //Adding their paths to the JSON result and changing extension
            
        }

        result.Add("albedo", Path.ChangeExtension(albedoPath, ".dds"));
        result.Add("normal", Path.ChangeExtension(normalPath, ".dds"));
        result.Add("reflective", Path.ChangeExtension(reflectivePath, ".dds"));

        return result;
    }

    public static JObject ConvertToJSON(Texture aTexture)
    {
        JObject result = new JObject();

        string texturePath = AssetDatabase.GetAssetPath(aTexture);

        result.Add("texture", Path.ChangeExtension(texturePath, ".dds"));
        Export.AddDependency(texturePath);

        return result;
    }

    public static JObject ConvertToJSON(BoxCollider aBoxCollider)
    {
        JObject result = new JObject();

        //Vector3 size;
        //size.x = aBoxCollider.size.x * aBoxCollider.transform.lossyScale.x;
        //size.y = aBoxCollider.size.y * aBoxCollider.transform.lossyScale.y;
        //size.z = aBoxCollider.size.z * aBoxCollider.transform.lossyScale.z;

        result.Add("is_trigger", aBoxCollider.isTrigger);
        result.Add("center", ConvertToJSON(aBoxCollider.center));
        result.Add("size", ConvertToJSON(aBoxCollider.size));

        return result;
    }

    public static JObject ConvertToJSON(SceneAsset aScene)
    {
        JObject result = new JObject();
        result.Add("name", aScene.name);

        return result;
    }

    public static JObject ConvertToJSON(ClassContainer aClassContainer)
    {
        JObject result = new JObject();
        result.Add("type", aClassContainer.myScriptName);
        JObject data = new JObject();
        
        foreach (var element in aClassContainer.myList)
        {
            switch (element.myVariableType)
            {
                case TypeEnum.Int:
                    {
                        data.Add(element.myVariableName, element.myInt);
                        break;
                    }
                case TypeEnum.Float:
                    {
                        data.Add(element.myVariableName, element.myFloat);
                        break;
                    }
                case TypeEnum.String:
                    {
                        data.Add(element.myVariableName, element.myString);
                        break;
                    }
                case TypeEnum.Vector3:
                    {
                        data.Add(element.myVariableName, ConvertToJSON(element.myVector3));
                        break;
                    }
                case TypeEnum.Bool:
                    {
                        data.Add(element.myVariableName, element.myBool);
                        break;
                    }
                case TypeEnum.Color:
                    {
                        data.Add(element.myVariableName, ConvertToJSON(element.myColor));
                        break;
                    }
                case TypeEnum.Texture:
                    {
                        if (!element.myTexture)
                            Debug.Log("Class container on " + aClassContainer.gameObject.name + " has a null texture");

                        data.Add(element.myVariableName, ConvertToJSON(element.myTexture));
                        break;
                    }
                case TypeEnum.Scene:
                    {
                        if (!element.myScene)
                            Debug.Log("Class container on " + aClassContainer.gameObject.name + " has a null scene");

                        data.Add(element.myVariableName, ConvertToJSON((SceneAsset)element.myScene));
                        break;
                    }
                case TypeEnum.GameObject:
                    {
                        if (!element.myGameObject)
                            Debug.Log("Class container on " + aClassContainer.gameObject.name +" has a null gameobject");

                        var obj = Object.Instantiate(element.myGameObject);
                        //var obj = PrefabUtility.InstantiatePrefab(element.myGameObject);
                        data.Add(element.myVariableName, ConvertToJSON((GameObject)obj));
                        Object.DestroyImmediate(obj);
                        break;
                    }
            }
        }

        result.Add("data", data);

        return result;
    }
}
