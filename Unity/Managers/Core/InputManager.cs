using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class InputManager
{
    public Action KeyAction = null;
    public Action<Defines.MouseEvent> MouseAction = null;

    bool _pressed = false;
    bool _rightPressed = false;

    public void OnUpdate()
    {
        if (EventSystem.current.IsPointerOverGameObject())
            return;

        if (Input.anyKey && KeyAction != null)
            KeyAction.Invoke();

        if (MouseAction != null)
        {
            if (Input.GetMouseButton(0))
            {
                MouseAction.Invoke(Defines.MouseEvent.Press);
                _pressed = true;
            }
            else if(_pressed == false)
            {
                MouseAction.Invoke(Defines.MouseEvent.Up);
            }
            else
            {
                if (_pressed)
                    MouseAction.Invoke(Defines.MouseEvent.Click);
                _pressed = false;
            }

            if (Input.GetMouseButton(1))
            {
                MouseAction.Invoke(Defines.MouseEvent.RightPress);
                _rightPressed = true;
            }
            else if(_rightPressed == false)
            {
                MouseAction.Invoke(Defines.MouseEvent.RightUp);
            }
            else
            {
                if (_rightPressed)
                    MouseAction.Invoke(Defines.MouseEvent.RightPress);
                _rightPressed = false;
            }

			float scroollWheel = Input.GetAxis("Mouse ScrollWheel");

			// Scroll
			if (scroollWheel > 0)
                MouseAction.Invoke(Defines.MouseEvent.WheelUp);

            if (scroollWheel < 0)
                MouseAction.Invoke(Defines.MouseEvent.WheelDown);
        }
    }

	public void Clear()
	{
		KeyAction = null;
		MouseAction = null;
	}
}
