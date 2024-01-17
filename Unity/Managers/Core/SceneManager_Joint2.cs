using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneManager_Joint2
{
	public BaseScene CurrentScene { get { return GameObject.FindObjectOfType<BaseScene>(); } }

	public void LoadScene(Defines.Scene type)
	{
		Manager.Instance.Clear();

		SceneManager.LoadScene(GetSceneName(type));
	}

	string GetSceneName(Defines.Scene type)
	{
		string name = System.Enum.GetName(typeof(Defines.Scene), type);
		return name;
	}

	public void Clear()
	{
		CurrentScene.Clear();
	}
}
