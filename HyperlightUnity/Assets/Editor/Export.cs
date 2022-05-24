using System.Collections;
using System.Collections.Generic;
using System;
using System.IO;
using UnityEngine;
using UnityEditor;
using UnityEditor.SceneManagement;
using Newtonsoft.Json.Linq;

public class Export
{
    private static bool CompareFiles(string file1, string file2)
    {
        const int BYTES_TO_READ = 1024 * 10;

        using (FileStream fs1 = File.Open(file1, FileMode.Open))
        using (FileStream fs2 = File.Open(file2, FileMode.Open))
        {
            byte[] one = new byte[BYTES_TO_READ];
            byte[] two = new byte[BYTES_TO_READ];
            while (true)
            {
                int len1 = fs1.Read(one, 0, BYTES_TO_READ);
                int len2 = fs2.Read(two, 0, BYTES_TO_READ);
                int index = 0;
                while (index < len1 && index < len2)
                {
                    if (one[index] != two[index]) return false;
                    index++;
                }
                if (len1 == 0 || len2 == 0) break;
            }
        }

        return true;
    }

    //holds unique names for each file it should export
    private static HashSet<string> myRequiredPaths = new HashSet<string>();
    public static void AddDependency(string aPath)
    {
        myRequiredPaths.Add(aPath);
    }
    public static void ExportDependencies()
    {
        foreach (string path in myRequiredPaths)
        {
            if (path == "")
                continue;

            ExportFile(path, "../TGE/Bin/" + path);
        }

        myRequiredPaths.Clear();
    }

    public static void ExportFile(string aSource, string aDestination)
    {
        string dirname = Path.GetDirectoryName(aDestination);
        if (!Directory.Exists(dirname))
        {
            Directory.CreateDirectory(dirname);
        }

        if (File.Exists(aDestination) && CompareFiles(aSource, aDestination))
        {
            return;
        }

        if (Path.GetExtension(aSource) == ".png")
        {
            string ddsDest = Application.dataPath.Replace("Assets", "") + Path.ChangeExtension(aDestination, ".dds");
            string exe = Application.dataPath.Replace("Assets", "convert.exe");
            string sourcePath = Application.dataPath.Replace("Assets", "") + aSource;
            string args = "\"" + sourcePath + "\"" + " " + "\"" + ddsDest + "\"";
            System.Diagnostics.Process.Start(exe, args); // Call imagemagick convert
            //return; // Dont copy png;
        }

        File.Copy(aSource, aDestination, true);
    }



    [MenuItem("Tools/Export active scene")]
    private static void ExportActiveScene()
    {
        myRequiredPaths.Clear();

        if (!EditorSceneManager.SaveCurrentModifiedScenesIfUserWantsTo()) // Check if active scene is saved
            return;

        var scene = UnityEngine.SceneManagement.SceneManager.GetActiveScene();
        ExportScene(scene);

        ExportDependencies();
        ExportTagsAndLayersEnumFile();
    }

    [MenuItem("Tools/Export scenes in build settings")]
    private static void ExportAllScenes()
    {
        myRequiredPaths.Clear();

        if (!EditorSceneManager.SaveCurrentModifiedScenesIfUserWantsTo()) // Check if active scene is saved
            return;

        string initiallyOpened = UnityEngine.SceneManagement.SceneManager.GetActiveScene().path;
        List<EditorBuildSettingsScene> Scenes = new List<EditorBuildSettingsScene>(EditorBuildSettings.scenes);
        foreach (var scene in Scenes)
        {
            var currentScene = EditorSceneManager.OpenScene(scene.path);
            ExportScene(currentScene);
        }
        ExportDependencies();

        EditorSceneManager.OpenScene(initiallyOpened); // Load the scene you first had opened
    }



    [MenuItem("Tools/Export and play active scene")]
    private static void Play()
    {
        ExportActiveScene();

        System.Diagnostics.ProcessStartInfo info = new System.Diagnostics.ProcessStartInfo();
        string binFolder = Application.dataPath.Replace("Assets", "..") + "/TGE/Bin/";
        string bin = binFolder + "Launcher_Debug.exe";
        info.FileName = bin;
        info.WorkingDirectory = binFolder;
        info.CreateNoWindow = false;

        System.Diagnostics.Process.Start(info);
    }

    [MenuItem("Tools/Toggle visible colliders")]
    private static void ToggleColliders()
    {
        DrawCollider.myDrawColliders = !DrawCollider.myDrawColliders;
    }
    private static void ExportScene(UnityEngine.SceneManagement.Scene aScene)
    {
        GameObject[] rootGameobjects = aScene.GetRootGameObjects();

        JObject jsonData = new JObject();
        jsonData.Add("name", aScene.name);
        JArray gameObjects = new JArray();

        //Looping through all root game objects add recursively adding all their children
        foreach (GameObject rootGameObject in rootGameobjects)
        {
            if (!rootGameObject.activeSelf)
                continue;

            gameObjects.Add(Converter.ConvertToJSON(rootGameObject));
        }

        jsonData["gameObjects"] = gameObjects;

        if (!Directory.Exists("../TGE/Bin/Assets/Scenes/"))
            Directory.CreateDirectory("../TGE/Bin/Assets/Scenes/");

        StreamWriter streamWriter = new StreamWriter("../TGE/Bin/Assets/Scenes/" + aScene.name + ".json");
        streamWriter.WriteLine(jsonData.ToString());
        streamWriter.Close();

        Debug.Log("Exported scene: " + aScene.name);
    }

    //This function exports a .h file containing the tags and layers enum, to be used in the engine
    private static void ExportTagsAndLayersEnumFile()
    {
        //Get all tags in a string list
        List<string> tags = new List<string>();
        for (int i = 0; i < UnityEditorInternal.InternalEditorUtility.tags.Length; i++)
        {
            tags.Add(UnityEditorInternal.InternalEditorUtility.tags[i]);
        }

        //Get all layers in a string list
        List<string> layers = new List<string>();
        for (int i = 0; i < UnityEditorInternal.InternalEditorUtility.layers.Length; i++)
        {
            layers.Add(UnityEditorInternal.InternalEditorUtility.layers[i]);
        }

        StreamWriter streamWriter = new StreamWriter("../TGE/Bin/Assets/TagsAndLayers.h");
        streamWriter.WriteLine("#pragma once");
        streamWriter.WriteLine("//This file is auto-generated by the TGE exporter. Do not edit it manually");
        streamWriter.WriteLine("");

        //Create a enum class for tags and add the tags list to it
        streamWriter.WriteLine("enum class eTag");
        streamWriter.WriteLine("{");
        for (int i = 0; i < tags.Count; i++)
        {
            string tag = tags[i];
            //remove space from tag
            tag = tag.Replace(" ", "");

            streamWriter.WriteLine("\t" + tag + ",");
        }
        streamWriter.WriteLine("};");

        //Create a enum class for layers and add the layers list to it
        streamWriter.WriteLine("enum class eLayer");
        streamWriter.WriteLine("{");
        for (int i = 0; i < layers.Count; i++)
        {
            string layer = layers[i];
            //remove space from layer
            layer = layer.Replace(" ", "");

            streamWriter.WriteLine("\t" + layer + ",");
        }
        streamWriter.WriteLine("};");

        streamWriter.Close();
    }
}
