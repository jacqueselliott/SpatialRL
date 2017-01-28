using UnityEngine;
using System.Collections;
using Improbable.Unity.Visualizer;
using Improbable.Gameplay;
using Improbable.Unity.Core;
using Improbable.Worker;
using Assets.EntityTemplates;
using Improbable.Math;

namespace GameLogic
{
    public class CollisionDetect : MonoBehaviour
    {
        [Require]
        private GoalSphere.Writer GoalSphereWriter;

        void OnEnable()
        {
            GoalSphereWriter.Send(new GoalSphere.Update().SetCollided(false));
        }

        private void OnTriggerEnter(Collider other)
        {
            Debug.Log("collision");
            if (other != null && other.gameObject.tag == "GoalSphere")
            {
                GoalSphereWriter.Send(new GoalSphere.Update().SetCollided(true));
                TeleportGoalSphere(other.gameObject);

                //DeleteGoalSphere(other.gameObject);

                //CreateGoalSphere();
            }
        }

        private void TeleportGoalSphere(GameObject goalObject)
        {
            GoalMove goalMove = goalObject.GetComponent<GoalMove>();
            goalMove.Move(RandomCoordinates());
        }

        private static Coordinates RandomCoordinates()
        {
            float x = (Random.value * 50) - 25;
            float z = (Random.value * 40) - 20;
            return new Coordinates(x, 1, z);
        }

        private void DeleteGoalSphere(GameObject goalObject)
        {
            SpatialOS.Commands.DeleteEntity(GoalSphereWriter, goalObject.EntityId(), result =>
            {
                if (result.StatusCode != StatusCode.Success)
                {
                    Debug.Log("failed to delete entity with error: " + result.ErrorMessage);
                    return;
                }
                Debug.Log("deleted entity: " + result.Response.Value);
            });
        }

        private void CreateGoalSphere()
        {
            var goalSphereEntityTemplate = GoalSphereEntityTemplate.GenerateGoalSphereEntityTemplate();
            SpatialOS.Commands.CreateEntity(GoalSphereWriter, "GoalSphere", goalSphereEntityTemplate, result =>
            {
                if (result.StatusCode != StatusCode.Success)
                {
                    Debug.LogError("PlayerSpawnManager failed to create entity: " + result.ErrorMessage);
                    return;
                }
                var createdEntityId = result.Response.Value;
                Debug.Log("PlayerSpawnManager created player entity with entity ID: " + createdEntityId);
            });
        }

    }
}