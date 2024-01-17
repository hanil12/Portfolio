using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_SkeletonSlider : UI_Scene
{
	public Slider FrameSlider { get; private set; }

    public enum GameObjects
    {
        Slider
    }

	public override void Init()
	{
		base.Init();

		Bind<GameObject>(typeof(GameObjects));

		FrameSlider = Get<GameObject>((int)GameObjects.Slider).GetOrAddComponent<Slider>();
	}
}
