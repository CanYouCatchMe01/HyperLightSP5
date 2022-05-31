using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

public class DrawCollider : MonoBehaviour
{
    // Start is called before the first frame update
    public static bool myDrawColliders;
    public bool myAlwaysDraw = false;
    public Color myColor = Color.red;

    [ExecuteInEditMode]
    private void Start()
    {
        
    }

    private void OnDrawGizmos()
    {
        if (!myDrawColliders && !myAlwaysDraw)
            return;

        var boxCollider = GetComponent<BoxCollider>();
        if (boxCollider == null)
            return;

        Camera camera = Camera.current;
        if (camera == null) return;

        var planes = GeometryUtility.CalculateFrustumPlanes(camera);

        //Vector3 camForward = camera.transform.forward;

        //Vector3 colliderToCamera = camera.transform.position - transform.position;

        //float product = Vector3.Dot(camForward.normalized, colliderToCamera.normalized);
        //if (product > 0.5f)
        //{
        //    Debug.Log("did not draw collider");
        //    return;
        //}

        if (!GeometryUtility.TestPlanesAABB(planes, boxCollider.bounds))
            return;

        //Gizmos.color = Color.red;

        Matrix4x4 mat = Matrix4x4.TRS(transform.position, transform.rotation, transform.lossyScale);
        //Vector3 pos = mat * new Vector4(colliderCenter.x, colliderCenter.y, colliderCenter.z, 1);

        

        Handles.matrix = mat;
        Handles.color = myColor;
        Handles.DrawWireCube(boxCollider.center, boxCollider.size);
    }
}
