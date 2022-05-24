using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

[System.Serializable]
public class Teleporter : MonoBehaviour
{
    [SerializeField] public Object sceneToLoad;
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
    // Start is called before the first frame update
    
}
