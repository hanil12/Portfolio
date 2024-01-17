using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using UnityEngine;
using UnityEngine.Diagnostics;
using UnityEngine.UI;

public class UIManager
{
	int _order = 10;

	Stack<UI_Popup> _popupStack = new Stack<UI_Popup>();
	UI_Scene _sceneUI = null;

	public GameObject Root
	{
		get
		{
			GameObject root = GameObject.Find("@UI_Root");
			if (root == null)
				root = new GameObject { name = "@UI_Root" };
			return root;
		}
	}

	public void SetCanvas(GameObject go, bool sort = true)
	{
		Canvas canvas = Util.GetOrAddComponent<Canvas>(go);
		canvas.renderMode = RenderMode.ScreenSpaceOverlay;
		canvas.overrideSorting = true;

		if(sort)
		{
			canvas.sortingOrder = _order;
			++_order;
		}
		else
		{
			canvas.sortingOrder = 0;
		}
	}

	public T MakeWorldSpaceUI<T>(Transform parent = null, string name = null, Camera camera = null) where T : UI_Base
	{
		if (string.IsNullOrEmpty(name))
			name = typeof(T).Name;

		// Assets/Resources/Prefabs/UI/WorldSpace/UI_Angle.prefab
		GameObject go = Manager.ResourceManager.Instantiate($"UI/WorldSpace/{name}");
		if(parent != null)
		{
			go.transform.SetParent(parent);
		}

		Canvas canvas = go.GetOrAddComponent<Canvas>();
		canvas.renderMode = RenderMode.WorldSpace;
		if (camera == null)
			canvas.worldCamera = Camera.main;
		else
			canvas.worldCamera = camera;

		return Util.GetOrAddComponent<T>(go);
	}

	public T MakeSubItem<T>(Transform parent = null, string name = null) where T : UI_Base
	{
		if (string.IsNullOrEmpty(name))
			name = typeof(T).Name;

		GameObject go = Manager.ResourceManager.Instantiate($"UI/SubItem/{name}");
		if(parent != null)
		{
			go.transform.SetParent(parent);
		}

		return Util.GetOrAddComponent<T>(go);
	}

	public T ShowSceneUI<T>(string name = null) where T : UI_Scene
	{
		if(string.IsNullOrEmpty(name))
			name = typeof(T).Name;

		GameObject go = Manager.ResourceManager.Instantiate($"UI/Scene/{name}");
		T sceneUI = Util.GetOrAddComponent<T>(go);
		_sceneUI = sceneUI;

		go.transform.SetParent(Root.transform);

		return sceneUI;
	}

	public T ShowPopupUI<T>(string name = null) where T : UI_Popup
	{
		if(string.IsNullOrEmpty(name))
			name = typeof(T).Name;

		GameObject go = Manager.ResourceManager.Instantiate($"UI/Popup/{name}");
		T popup = Util.GetOrAddComponent<T>(go);
		_popupStack.Push(popup);

		go.transform.SetParent(Root.transform);

		return popup;
	}

	public void ClosePopupUI(UI_Popup popup)
	{
		if (_popupStack.Count == 0)
			return;

		if(_popupStack.Peek() != popup)
		{
			Debug.LogError("Close Popup Failed!!");
			return;
		}

		ClosePopupUI();
	}

	public void ClosePopupUI()
	{
		if (_popupStack.Count == 0)
			return;

		UI_Popup popup = _popupStack.Pop();
		Manager.ResourceManager.Destroy(popup.gameObject);
		popup = null;

		--_order;
	}

	public void CloseSceneUI()
	{
		if (_sceneUI == null)
			return;

		Manager.ResourceManager.Destroy(_sceneUI.gameObject);
		_sceneUI = null;
	}

	public void CloseAllPopupUI()
	{
		while(true)
		{
			if (_popupStack.Count == 0)
				return;

			ClosePopupUI();
		}
	}

	public void Clear()
	{
		CloseAllPopupUI();
		CloseSceneUI();
	}
}
