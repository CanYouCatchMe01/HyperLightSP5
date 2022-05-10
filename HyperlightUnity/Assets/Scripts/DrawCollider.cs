using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DrawCollider : MonoBehaviour
{
    // Start is called before the first frame update
    public static bool myDrawColliders;

    private void OnDrawGizmos()
    {
        if (!myDrawColliders)
            return;

        Gizmos.color = Color.red;
        Vector3 colliderCenter = GetComponent<BoxCollider>().center;
        Vector3 colliderSize = GetComponent<BoxCollider>().size;

        Matrix4x4 mat = Matrix4x4.TRS(transform.position, transform.rotation, transform.lossyScale);
        //Vector3 pos = mat * new Vector4(colliderCenter.x, colliderCenter.y, colliderCenter.z, 1);
        Gizmos.matrix = mat;
        Gizmos.DrawWireCube(colliderCenter, colliderSize);
        //Gizmos.DrawCube(colliderCenter, scale);
    }
}
