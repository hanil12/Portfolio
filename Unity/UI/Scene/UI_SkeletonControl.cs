using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UI_SkeletonControl : UI_Scene
{
	public enum GameObjects
	{
		Index,
		X_Axis,
		Y_Axis,
		Z_Axis,
		Slider
	}

	public enum Buttons
	{
		AllBody,
		HigherBody,
		LowerBody
	}

	public event Action CompleteBinding;
	public event Action<string> IndexInputEvent;
	public event Action<Vector3> RotateInputEvent;
	public event Action AllBodyEvent;
	public event Action HigherBodyEvent;
	public event Action LowerBodyEvent;

	public Slider FrameSlider { get; private set; }
	public InputField IndexField { get; private set; }
	public InputField X_Axis { get; private set; }
	public InputField Y_Axis { get; private set; }
	public InputField Z_Axis { get; private set; }

	public Button AllBody_Btn { get; private set; }
	public Button HigherBody_Btn { get; private set; }
	public Button LowerBody_Btn { get; private set; }

	public Dropdown Dropdown { get; private set; }
	public InputField InputField { get; private set; }

	Vector3 _rototationVec = new Vector3();

	public override void Init()
	{
		base.Init();

		Bind<GameObject>(typeof(GameObjects));
		Bind<Button>(typeof(Buttons));

		IndexField = Get<GameObject>((int)GameObjects.Index).GetOrAddComponent<InputField>();
		IndexField.onEndEdit.AddListener(SetFrameByIndex);

		X_Axis = Get<GameObject>((int)GameObjects.X_Axis).GetOrAddComponent<InputField>();
		X_Axis.onEndEdit.AddListener(XAxisSet);

		Y_Axis = Get<GameObject>((int)GameObjects.Y_Axis).GetOrAddComponent<InputField>();
		Y_Axis.onEndEdit.AddListener(YAxisSet);

		Z_Axis = Get<GameObject>((int)GameObjects.Z_Axis).GetOrAddComponent<InputField>();
		Z_Axis.onEndEdit.AddListener(ZAxisSet);

		FrameSlider = Get<GameObject>((int)GameObjects.Slider).GetComponent<Slider>();


		AllBody_Btn = GetButton((int)Buttons.AllBody);
		AllBody_Btn.onClick.AddListener(() => { if (AllBodyEvent != null) AllBodyEvent.Invoke();});

		HigherBody_Btn = GetButton((int)Buttons.HigherBody);
		HigherBody_Btn.onClick.AddListener(() => { if (HigherBodyEvent != null) HigherBodyEvent.Invoke(); });

		LowerBody_Btn = GetButton((int)Buttons.LowerBody);
		LowerBody_Btn.onClick.AddListener(() => { if (LowerBodyEvent != null) LowerBodyEvent.Invoke(); });

		if (CompleteBinding != null)
			CompleteBinding.Invoke();
	}


	public void SetWithOutInput()
	{
		IndexField.gameObject.SetActive(false);
		X_Axis.gameObject.SetActive(false);
		Y_Axis.gameObject.SetActive(false);
		Z_Axis.gameObject.SetActive(false);
		AllBody_Btn.gameObject.SetActive(false);
		HigherBody_Btn.gameObject.SetActive(false);
		LowerBody_Btn.gameObject.SetActive(false);
		// Dropdown.gameObject.SetActive(false);
	}

	public void XAxisSet(string deltaX)
	{
		float x = 0.0f;
		bool check = float.TryParse(deltaX, out x);

		if (!check) return;

		_rototationVec.x = x;

		RotateInputEvent(_rototationVec);
	}

	public void YAxisSet(string deltaY)
	{
		float y = 0.0f;
		bool check = float.TryParse(deltaY, out y);

		if (!check) return;

		_rototationVec.y = y;

		RotateInputEvent(_rototationVec);
	}

	public void ZAxisSet(string deltaZ)
	{
		float z = 0.0f;
		bool check = float.TryParse(deltaZ, out z);

		if (!check) return;

		_rototationVec.z = z;

		RotateInputEvent(_rototationVec);
	}

	public Vector3 GetRotation_Axis()
	{
		Vector3 result = Vector3.zero;

		bool xTry = float.TryParse(X_Axis.text, out result.x);
		bool yTry = float.TryParse(Y_Axis.text, out result.y);
		bool zTry = float.TryParse(Z_Axis.text, out result.z);

		if (xTry && yTry && zTry)
			return result;

		return Vector3.zero;
	}


	void SetFrameByIndex(string index)
	{
		IndexInputEvent.Invoke(index);
	}

	void SetCSV(string csv)
	{
		// Manager.DataManager.LoadByPath_Resources(csv);
	}
}
