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
            EditorGUILayout.BeginHorizontal();

            //Enum type
            TypeEnum typeEnum = (TypeEnum)EditorGUILayout.EnumPopup(list[i].myVariableType);
            list[i].myVariableType = typeEnum;
            
            //Variable name
            list[i].myVariableName = EditorGUILayout.TextField(list[i].myVariableName);
            GUILayout.Label("=");

            var minWidth = GUILayout.MinWidth(64); //Used so the remove "x" stays in the window, when it is small
            switch (list[i].myVariableType)
            {
                case TypeEnum.Int:
                    {
                        list[i].myInt = EditorGUILayout.IntField("", list[i].myInt, minWidth);
                        break;
                    }
                case TypeEnum.Float:
                    {
                        list[i].myFloat = EditorGUILayout.FloatField("", list[i].myFloat, minWidth);
                        break;
                    }
                case TypeEnum.String:
                    {
                        list[i].myString = EditorGUILayout.TextField("", list[i].myString, minWidth);
                        break;
                    }
                case TypeEnum.Vector3:
                    {
                        list[i].myVector3 = EditorGUILayout.Vector3Field("", list[i].myVector3, minWidth);
                        break;
                    }
                case TypeEnum.Bool:
                    {
                        list[i].myBool = EditorGUILayout.Toggle("", list[i].myBool, minWidth);
                        break;
                    }
                case TypeEnum.Color:
                    {
                        list[i].myColor = EditorGUILayout.ColorField("", list[i].myColor, minWidth);
                        break;
                    }
                case TypeEnum.Texture:
                    {
                        list[i].myTexture = EditorGUILayout.ObjectField("", list[i].myTexture, typeof(Texture), true, minWidth) as Texture;
                        break;
                    }
                case TypeEnum.Scene:
                    {
                        list[i].myScene = EditorGUILayout.ObjectField("", list[i].myScene, typeof(SceneAsset), true, minWidth) as SceneAsset;
                        break;
                    }
                case TypeEnum.GameObject:
                    {
                        list[i].myGameObject = EditorGUILayout.ObjectField("", list[i].myGameObject, typeof(GameObject), true, minWidth) as GameObject;
                        break;
                    }
            }
            var deleteIcon = EditorGUIUtility.IconContent("d_P4_DeletedLocal@2x");
            if (GUILayout.Button(deleteIcon))
            {
                elementToRemove = i;
            }
            EditorGUILayout.EndHorizontal();
        }

        //Remove element
        if (elementToRemove != -1)
        {
            list.RemoveAt(elementToRemove);
        }

        EditorGUILayout.Space();
        if (GUILayout.Button("Add Variable"))
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