﻿using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class UI_EventHandler : MonoBehaviour, IPointerClickHandler, IDragHandler, IPointerEnterHandler
{
    public Action<PointerEventData> OnClickHandler = null;
    public Action<PointerEventData> OnDragHandler = null;
	public Action<PointerEventData> OnEnterHandler = null;

	public void OnPointerClick(PointerEventData eventData)
	{
		if (OnClickHandler != null)
			OnClickHandler.Invoke(eventData);
	}

	public void OnDrag(PointerEventData eventData)
    {
		if (OnDragHandler != null)
            OnDragHandler.Invoke(eventData);
	}

	public void OnPointerEnter(PointerEventData eventData)
	{
		if (OnEnterHandler != null)
			OnEnterHandler.Invoke(eventData);
	}
}
