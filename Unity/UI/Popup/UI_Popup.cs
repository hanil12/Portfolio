using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI_Popup : UI_Base
{
    public override void Init()
    {
        Manager.UIManager.SetCanvas(gameObject, true);
    }

    public virtual void ClosePopupUI()
    {
        Manager.UIManager.ClosePopupUI(this);
    }
}
