using UnityEngine;
using Improbable.Worker;
using Terrain = Improbable.Terrain.Terrain;
using TerrainData = Improbable.Terrain.TerrainData;
using Improbable.General;
using Improbable.Math;
using Improbable.Unity.Core.Acls;

namespace Assets.EntityTemplates
{
    public class TerrainEntityTemplate : MonoBehaviour
    {

        public static SnapshotEntity GenerateTerrainSnapshotEntityTemplate()
        {
            var terrain = new SnapshotEntity { Prefab = "Terrain" };
            terrain.Add(new Terrain.Data(new TerrainData()));
            terrain.Add(new WorldTransform.Data(new WorldTransformData(new Coordinates(0, 0, 0), 0)));

            var acl = Acl.Build()
                .SetReadAccess(CommonPredicates.PhysicsOrVisual)
                .SetWriteAccess<WorldTransform>(CommonPredicates.PhysicsOnly);
            terrain.SetAcl(acl);

            return terrain;
        }
    }
}
