using UnityEngine;
using System.Collections;
using Improbable.Unity.Visualizer;
using Improbable.General;
using Improbable.Math;
using Assets.Gamelogic.Behaviours;
using Improbable.Player;

namespace GameLogic.Controls
{
    public class DroneMovement : MonoBehaviour
    {

        [Require]
        private WorldTransform.Writer WorldTransformWriter;

        [Require]
        private DroneControls.Reader DroneControlsReader;

        [Require]
        private WorldVelocity.Writer WorldVelocityWriter;

        public float moveSpeed = 5000;
        private Rigidbody droneRigidbody;
        // Use this for initialization
        void OnEnable()
        {
            transform.position = WorldTransformWriter.Data.position.ToVector3();
            droneRigidbody = GetComponent<Rigidbody>();
        }

        // Update is called once per frame
        void Update()
        {
            var delta = Time.deltaTime;
            Vector3f forceFromControls = DroneControlsReader.Data.force;
            Vector3 force = new Vector3(forceFromControls.X, forceFromControls.Y - 0.75f, forceFromControls.Z) * moveSpeed * delta;
            droneRigidbody.AddForce(force);
            transform.rotation = Quaternion.Euler(0f, transform.rotation.eulerAngles.y, 0f);
            WorldTransformWriter.Send(new WorldTransform.Update().SetPosition(transform.position.ToCoordinates())
                                                                     .SetRotation((uint)transform.rotation.eulerAngles.y));
            WorldVelocityWriter.Send(new WorldVelocity.Update().SetVelocity(droneRigidbody.velocity.ToVector3f()));
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
}
