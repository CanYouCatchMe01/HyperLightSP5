using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor.SceneManagement;

public class CheckPoint : MonoBehaviour
{
    [SerializeField] public Object spawnPointScene;
    [SerializeField] public string spawnPointName;

    [SerializeField] public Color editorDrawColor = Color.cyan;


    private void OnDrawGizmos()
    {
        var boxCollider = GetComponent<BoxCollider>();
        if (boxCollider == null)
        {
            Debug.Log("Spawnpoint must have a collider");
            return;
        }


        Matrix4x4 mat = Matrix4x4.TRS(transform.position, transform.rotation, transform.lossyScale);
        Gizmos.matrix = mat;
        Gizmos.color = editorDrawColor;
        Gizmos.DrawWireCube(boxCollider.center, boxCollider.size);
    }
}
