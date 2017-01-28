using UnityEngine;
using Improbable.Worker;
using Improbable.General;
using Improbable.Math;
using Improbable.Unity.Core.Acls;
using Improbable.Gameplay;
using Improbable.Player;

namespace Assets.EntityTemplates
{
    public class GoalSphereEntityTemplate : MonoBehaviour
    {
        // Template definition for a Example entity
        public static SnapshotEntity GenerateGoalSphereSnapshotEntityTemplate()
        {
            // Set name of Unity prefab associated with this entity
            var goalSphereEntity = new SnapshotEntity { Prefab = "GoalSphere" };

            // Define components attached to snapshot entity
            goalSphereEntity.Add(new WorldTransform.Data(new WorldTransformData(RandomCoordinates(), 0)));
            goalSphereEntity.Add(new Name.Data(new NameData("RL")));

            goalSphereEntity.SetAcl(BuildACL());

            return goalSphereEntity;
        }

        public static Entity GenerateGoalSphereEntityTemplate()
        {
            var goalSphereEntity = new Entity();

            goalSphereEntity.Add(new WorldTransform.Data(new WorldTransformData(RandomCoordinates(), 0)));

            goalSphereEntity.SetAcl(BuildACL());

            return goalSphereEntity;
        }

        private static Acl BuildACL()
        {
            var acl = Acl.Build()
                .SetReadAccess(Acl.MakePredicate(CommonClaims.Physics, CommonClaims.Visual, Acl.MakeClaim("rl")))
                // Only FSim workers granted write access over WorldTransform component
                .SetWriteAccess<WorldTransform>(CommonPredicates.PhysicsOnly)
                .SetWriteAccess<Name>(Acl.MakePredicate(Acl.MakeClaim("rl")));
            return acl;
        }

        private static Coordinates RandomCoordinates()
        {
            float x = (Random.value * 50) - 25;
            float z = (Random.value * 40) - 20;
            return new Coordinates(x, 1, z);
        }
    }
}
