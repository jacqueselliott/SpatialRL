using UnityEngine;
using Improbable.Worker;
using Improbable.General;
using Improbable.Unity.Core.Acls;
using Improbable.Math;
using Improbable.Player;
using Improbable;
using System;
using Improbable.Gameplay;

namespace Assets.EntityTemplates
{
    public class DroneEntityTemplate : MonoBehaviour
    {
        // Template definition for a drone entity
        public static SnapshotEntity GenerateDroneSnapshotEntityTemplate(Coordinates initialCoordinates)
        {
            // Set name of Unity prefab associated with this entity
            var drone = new SnapshotEntity { Prefab = "Drone" };

            // Define components attached to snapshot entity
            drone.Add(new WorldTransform.Data(new WorldTransformData(initialCoordinates, 0)));
            drone.Add(new Name.Data(new NameData("the_reason_the_player_can_see")));
            drone.Add(new DroneControls.Data(new DroneControlsData(new Vector3f(0, 0, 0))));
            drone.Add(new GoalSphere.Data(new GoalSphereData(false)));
            drone.Add(new WorldVelocity.Data(new WorldVelocityData(new Vector3f(0, 0, 0))));

            var acl = Acl.Build()
                .SetReadAccess(CommonPredicates.PhysicsOrVisual)
                .SetWriteAccess<WorldTransform>(CommonPredicates.PhysicsOnly)
                .SetWriteAccess<Name>(CommonPredicates.VisualOnly)
                .SetWriteAccess<DroneControls>(CommonPredicates.PhysicsOrVisual)
                .SetWriteAccess<GoalSphere>(CommonPredicates.PhysicsOrVisual)
                .SetWriteAccess<WorldVelocity>(CommonPredicates.PhysicsOnly);

            drone.SetAcl(acl);

            return drone;
        }
        //Template definition for C++ claimed entity
        public static SnapshotEntity GenerateRLDroneSnapshotEntityTemplate(Coordinates initialCoordinates)
        {
            // Set name of Unity prefab associated with this entity
            var drone = new SnapshotEntity { Prefab = "Drone" };

            // Define components attached to snapshot entity
            drone.Add(new WorldTransform.Data(new WorldTransformData(initialCoordinates, 0)));
            drone.Add(new Name.Data(new NameData("the_reason_the_player_can_see")));
            drone.Add(new DroneControls.Data(new DroneControlsData(new Vector3f(0, 0, 0))));
            drone.Add(new GoalSphere.Data(new GoalSphereData(false)));
            drone.Add(new WorldVelocity.Data(new WorldVelocityData(new Vector3f(0, 0, 0))));

            var acl = Acl.Build()
                .SetReadAccess(Acl.MakePredicate(CommonClaims.Physics, CommonClaims.Visual, Acl.MakeClaim("rl")))
                .SetWriteAccess<WorldTransform>(CommonPredicates.PhysicsOnly)
                .SetWriteAccess<Name>(CommonPredicates.VisualOnly)
                .SetWriteAccess<DroneControls>(Acl.MakePredicate(Acl.MakeClaim("rl")))
                .SetWriteAccess<GoalSphere>(CommonPredicates.PhysicsOrVisual)
                .SetWriteAccess<WorldVelocity>(CommonPredicates.PhysicsOnly);

            drone.SetAcl(acl);

            return drone;
        }
    }
}

