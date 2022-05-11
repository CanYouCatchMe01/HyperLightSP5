using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;
using Newtonsoft.Json.Linq;
using UnityEditor;

class Converter
{
    public static JObject ConvertToJSON(GameObject aGameObject)
    {
        JObject JGameObject = new JObject();

        //Every game object have the following properties
        JGameObject.Add("name", aGameObject.name);
        JGameObject.Add("tag", aGameObject.tag);
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
        JObject result = new JObject(
            new JProperty("type", aLight.type)
        );

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

        return result;
    }

    public static JObject ConvertToJSON(MeshFilter aMeshFilter)
    {
        JObject result = new JObject();

        string modelPath = AssetDatabase.GetAssetPath(PrefabUtility.GetCorrespondingObjectFromOriginalSource(aMeshFilter.sharedMesh));

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
            }
        }

        result.Add("data", data);

        return result;
    }
}