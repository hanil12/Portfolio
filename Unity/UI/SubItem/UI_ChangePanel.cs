using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UI_ChangePanel : UI_Base
{
	UI_ChangePanel otherPanel = null;

	public override void Init()
	{
	}

	public void EnterMousePointer(PointerEventData data)
	{
		GameObject camera1 = null;
		GameObject camera2 = null;
		UI_EventHandler handler = gameObject.GetOrAddComponent<UI_EventHandler>();
		handler.enabled = false;
		gameObject.GetOrAddComponent<Image>().raycastTarget = false;

		if(name == "Panel1")
		{
			otherPanel = transform.parent.Find("Panel2").GetComponent<UI_ChangePanel>();
			camera1 = GameObject.Find("Camera1");
			camera2 = GameObject.Find("Camera2");
			if (camera1 != null && camera2 != null)
			{
				//camera1.GetOrAddComponent<MoveCamera>().enabled = true;
				//camera2.GetOrAddComponent<MoveCamera>().enabled = false;
			}
		}
		else if(name == "Panel2")
		{
			otherPanel = transform.parent.Find("Panel1").GetComponent<UI_ChangePanel>();
			camera1 = GameObject.Find("Camera1");
			camera2 = GameObject.Find("Camera2");
			if (camera1 != null && camera2 != null)
			{
				//camera2.GetOrAddComponent<MoveCamera>().enabled = true;
				//camera1.GetOrAddComponent<MoveCamera>().enabled = false;
			}
		}

		otherPanel.GetOrAddComponent<Image>().raycastTarget = true;
		otherPanel.gameObject.GetOrAddComponent<UI_EventHandler>().enabled = true;
	}
}
