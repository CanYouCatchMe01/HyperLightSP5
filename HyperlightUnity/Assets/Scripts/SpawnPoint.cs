using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnPoint : MonoBehaviour
{
    [SerializeField] public string spawnPointName;
    [SerializeField] public Color editorDrawColor = Color.cyan;
    private void OnDrawGizmos()
    {
        Gizmos.color = editorDrawColor;
        Gizmos.DrawWireSphere(transform.position, 0.5f);
    }

    
}
