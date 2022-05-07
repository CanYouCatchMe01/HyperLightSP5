using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEditor;

[CustomEditor(typeof(ClassContainer))]
public class CustomClassEditor : Editor
{
    ClassContainer myClassContainer;
    void OnEnable()
    {
        myClassContainer = (ClassContainer)target;
    }
    public override void OnInspectorGUI()
    {
        //EditorGUILayout.LabelField("Used in c++ to load the right component", EditorStyles.boldLabel);
        myClassContainer.myScriptName = EditorGUILayout.TextField("Script name", myClassContainer.myScriptName);

        var list = myClassContainer.myList;

        int elementToRemove = -1;
        for (int i = 0; i < list.Count; i++)
        {
            //Create a space
            EditorGUILayout.Space();

            TypeEnum typeEnum = (TypeEnum)EditorGUILayout.EnumPopup("Varible type", list[i].myVariableType);
            list[i].myVariableType = typeEnum;
            
            list[i].myVariableName = EditorGUILayout.TextField("Variable name", list[i].myVariableName);
            switch (list[i].myVariableType)
            {
                case TypeEnum.Int:
                    {
                        list[i].myInt = EditorGUILayout.IntField("My Int", list[i].myInt);
                        break;
                    }
                case TypeEnum.Float:
                    {
                        list[i].myFloat = EditorGUILayout.FloatField("My Float", list[i].myFloat);
                        break;
                    }
                case TypeEnum.String:
                    {
                        list[i].myString = EditorGUILayout.TextField("My String", list[i].myString);
                        break;
                    }
                case TypeEnum.Vector3:
                    {
                        list[i].myVector3 = EditorGUILayout.Vector3Field("My Vector3", list[i].myVector3);
                        break;
                    }
                case TypeEnum.Bool:
                    {
                        list[i].myBool = EditorGUILayout.Toggle("My Bool", list[i].myBool);
                        break;
                    }
                case TypeEnum.Color:
                    {
                        list[i].myColor = EditorGUILayout.ColorField("My Color", list[i].myColor);
                        break;
                    }
            }

            if (GUILayout.Button("Remove"))
            {
                elementToRemove = i;
            }
        }

        //Remove element
        if (elementToRemove != -1)
        {
            list.RemoveAt(elementToRemove);
        }

        EditorGUILayout.Space();
        if (GUILayout.Button("Add"))
        {
            list.Add(null);
        }

        //set dirty, which makes the editor unsaved
        if (GUI.changed)
        {
            EditorUtility.SetDirty(myClassContainer);
        }
    }
}