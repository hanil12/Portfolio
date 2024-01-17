using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;

public class UI_Overlay_AngleSelect : UI_Base
{
	public override void Init()
	{
		TMP_Dropdown dropdown = transform.GetOrAddComponent<TMP_Dropdown>();
		TMP_Dropdown.OptionData noneData = new TMP_Dropdown.OptionData();
		noneData.text = "전체보기";
		dropdown.options.Add(noneData);

		foreach (var str in Data.SkeletonAnimationData.betweenJoint_Kor)
		{
			TMP_Dropdown.OptionData temp = new TMP_Dropdown.OptionData() { text = str };
			dropdown.options.Add(temp);
		}
	}
}
