using UnityEngine;
using System.Collections;
using Improbable.Unity.Visualizer;
using Improbable.Unity;
using Improbable.Player;
using Improbable.Math;

namespace GameLogic.Controls
{
    [EngineType(EnginePlatform.Client)]
    public class PlayerControls : MonoBehaviour
    {
        [Require]
        private DroneControls.Writer DroneControlsWriter;
        
        // Update is called once per frame
        void Update()
        {
            float inputHorizontal = Input.GetAxis("Horizontal");
            float inputVertical = Input.GetAxis("Vertical");
            DroneControlsWriter.Send(new DroneControls.Update().
                SetForce(new Vector3f(inputHorizontal, 0f, inputVertical)));
        }
    }
}
