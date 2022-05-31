using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor.SceneManagement;

public class DrawGizmo : MonoBehaviour
{
    [SerializeField] public Color editorDrawColor = Color.cyan;

    private void OnDrawGizmos()
    {
        var boxCollider = GetComponent<BoxCollider>();
        if (boxCollider == null)
        {
            Debug.Log("gate trigger must be a collider");
            return;
        }

        Matrix4x4 mat = Matrix4x4.TRS(transform.position, transform.rotation, transform.lossyScale);
        Gizmos.matrix = mat;
        Gizmos.color = editorDrawColor;
        Gizmos.DrawWireCube(boxCollider.center, boxCollider.size);
    }
}
