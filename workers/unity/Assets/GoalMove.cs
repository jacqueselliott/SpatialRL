using UnityEngine;
using System.Collections;
using Improbable.Unity.Visualizer;
using Improbable.General;
using Assets.Gamelogic.Behaviours;
using Improbable.Player;
using Improbable.Math;


public class GoalMove : MonoBehaviour {

    [Require]
    private WorldTransform.Writer WorldTransformWriter;

    // Use this for initialization
    void OnEnable()
    {
    }

    public void Move(Coordinates coordinate)
    {
        transform.rotation = Quaternion.Euler(0f, transform.rotation.eulerAngles.y, 0f);
        transform.position = coordinate.ToVector3();

        WorldTransformWriter.Send(new WorldTransform.Update().SetPosition(coordinate)
                                                                        .SetRotation((uint)transform.rotation.eulerAngles.y));


    }

    
}

public static class Vector3Extensions
{
    public static Coordinates ToCoordinates(this Vector3 vector3)
    {
        return new Coordinates(vector3.x, vector3.y, vector3.z);
    }
    public static Vector3f ToVector3f(this Vector3 vector3)
    {
        return new Vector3f(vector3.x, vector3.y, vector3.z);
    }
}
