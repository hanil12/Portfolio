using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.EventSystems;

public class UI_TMPTest : UI_Base
{
    enum GameObjects
    {
        Text
    }

	public override void Init()
	{
        Bind<GameObject>(typeof(GameObjects));
		GameObject go = Get<GameObject>((int)(GameObjects.Text));
		var text = go.GetOrAddComponent<TMP_Text>();
		go.GetOrAddComponent<UI_EventHandler>().OnClickHandler += (PointerEventData data) => { text.text = "Click!!"; };
	}
}
