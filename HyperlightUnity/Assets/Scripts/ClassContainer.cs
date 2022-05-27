using UnityEngine;
using System;
using System.Collections.Generic; // Import the System.Collections.Generic class to give us access to List<>

public enum TypeEnum
{
    Int,
    Float,
    String,
    Vector3,
    Bool,
    Color,
    Texture
}

//This class is used by "LevelDesigners" to add custom values to a class
public class ClassContainer : MonoBehaviour
{
    [System.Serializable]
    public class ClassObject
    {
        public TypeEnum myVariableType;
        public string myVariableName;

        //Accepted data types
        public int myInt;
        public float myFloat;
        public string myString;
        public Vector3 myVector3;
        public bool myBool;
        public Color myColor = Color.white;
        public Texture myTexture;
    }

    public string myScriptName; //Used to know what component in c++ this class should be sent to
    public List<ClassObject> myList = new List<ClassObject>();
}