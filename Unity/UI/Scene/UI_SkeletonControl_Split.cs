using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UI_SkeletonControl_Split : UI_Scene
{
	public enum GameObjects
	{
		SplitLine,
		Index,
		X_Axis,
		Y_Axis,
		Z_Axis,
		Slider,
		Panel1,
		Panel2,
		ModeDropDown,
		ProjectDropDown,
		Overlay_AngleSelect
	}

	public enum Buttons
	{
		AllBody,
		HigherBody,
		LowerBody,
		SceneBtn,
	}

	public int MaxFrame { get; set; } = 0;

	public event Action CompleteBindingEvent;
	public event Action<string, string> GetPathEvent;
	public event Action<string> IndexInputEvent;
	public event Action<Vector3> RotateInputEvent;
	public event Action AllBodyEvent;
	public event Action HigherBodyEvent;
	public event Action LowerBodyEvent;
	public event Action SceneBtnEvent;

	public Image SplitLine { get { return Get<GameObject>((int)GameObjects.SplitLine).GetOrAddComponent<Image>(); } }
	public Slider FrameSlider { get; private set; }
	public TMP_InputField IndexField { get; private set; }
	public TMP_InputField X_Axis { get; private set; }
	public TMP_InputField Y_Axis { get; private set; }
	public TMP_InputField Z_Axis { get; private set; }

	public Button AllBody_Btn { get; private set; }
	public Button HigherBody_Btn { get; private set; }
	public Button LowerBody_Btn { get; private set; }
	public Button SceneChange_Btn { get; private set; }

	public TMP_Dropdown Overlay_AngleSelect { get { return Get<GameObject>((int)GameObjects.Overlay_AngleSelect).GetOrAddComponent<TMP_Dropdown>(); } }
	public InputField InputField { get; private set; }

	private const string AbsPath = "C:\\Program Files (x86)\\SEO\\GaitDetectedAnalysis\\datas";
	private Vector3 _rototationVec = new Vector3();

	private List<string> _simpleJointPaths = new List<string>();
	private List<string> _expertJointPaths = new List<string>();

	public override void Init()
	{
		base.Init();

		Bind<GameObject>(typeof(GameObjects));
		Bind<Button>(typeof(Buttons));

		IndexField = Get<GameObject>((int)GameObjects.Index).GetOrAddComponent<TMP_InputField>();
		IndexField.onEndEdit.AddListener(SetFrameByIndex);

		X_Axis = Get<GameObject>((int)GameObjects.X_Axis).GetOrAddComponent<TMP_InputField>();
		X_Axis.onEndEdit.AddListener(XAxisSet);

		Y_Axis = Get<GameObject>((int)GameObjects.Y_Axis).GetOrAddComponent<TMP_InputField>();
		Y_Axis.onEndEdit.AddListener(YAxisSet);

		Z_Axis = Get<GameObject>((int)GameObjects.Z_Axis).GetOrAddComponent<TMP_InputField>();
		Z_Axis.onEndEdit.AddListener(ZAxisSet);

		FrameSlider = Get<GameObject>((int)GameObjects.Slider).GetComponent<Slider>();
		FrameSlider.onValueChanged.AddListener(SetIndexBySlider);

		AllBody_Btn = GetButton((int)Buttons.AllBody);
		AllBody_Btn.onClick.AddListener(() => { if (AllBodyEvent != null) AllBodyEvent.Invoke(); });

		HigherBody_Btn = GetButton((int)Buttons.HigherBody);
		HigherBody_Btn.onClick.AddListener(() => { if (HigherBodyEvent != null) HigherBodyEvent.Invoke(); });

		LowerBody_Btn = GetButton((int)Buttons.LowerBody);
		LowerBody_Btn.onClick.AddListener(() => { if (LowerBodyEvent != null) LowerBodyEvent.Invoke(); });

		SceneChange_Btn = GetButton((int)Buttons.SceneBtn);
		SceneChange_Btn.GetOrAddComponent<UI_EventHandler>().OnClickHandler -= (PointerEventData data) => { if (SceneBtnEvent == null) return; SceneBtnEvent.Invoke();  };
		SceneChange_Btn.GetOrAddComponent<UI_EventHandler>().OnClickHandler += (PointerEventData data) => { if (SceneBtnEvent == null) return; SceneBtnEvent.Invoke();  };

		var panel1 = Get<GameObject>((int)GameObjects.Panel1).GetOrAddComponent<UI_ChangePanel>();
		panel1.gameObject.GetOrAddComponent<UI_EventHandler>().OnEnterHandler -= (PointerEventData data) => { panel1.EnterMousePointer(data); };
		panel1.gameObject.GetOrAddComponent<UI_EventHandler>().OnEnterHandler += (PointerEventData data) => { panel1.EnterMousePointer(data); };

		var panel2 = Get<GameObject>((int)GameObjects.Panel2).GetOrAddComponent<UI_ChangePanel>();
		panel2.gameObject.GetOrAddComponent<UI_EventHandler>().OnEnterHandler -= (PointerEventData data) => { panel2.EnterMousePointer(data); };
		panel2.gameObject.GetOrAddComponent<UI_EventHandler>().OnEnterHandler += (PointerEventData data) => { panel2.EnterMousePointer(data); };

		Overlay_AngleSelect.gameObject.SetActive(false);

		SetDropDownItems();

		if (CompleteBindingEvent != null)
			CompleteBindingEvent.Invoke();
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
	}

	public void XAxisSet(string deltaX)
	{
		if (RotateInputEvent == null) return;

		float x = 0.0f;
		bool check = float.TryParse(deltaX, out x);

		if (!check) return;

		_rototationVec.x = x;

		RotateInputEvent(_rototationVec);
	}

	public void YAxisSet(string deltaY)
	{
		if (RotateInputEvent == null) return;

		float y = 0.0f;
		bool check = float.TryParse(deltaY, out y);

		if (!check) return;

		_rototationVec.y = y;

		RotateInputEvent(_rototationVec);
	}

	public void ZAxisSet(string deltaZ)
	{
		if (RotateInputEvent == null) return;

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
		if (IndexInputEvent == null) return;

		IndexInputEvent.Invoke(index);
	}

	void SetCSV(string csv)
	{
		// Manager.DataManager.LoadByPath_Resources(csv);
	}

	async void SetDropDownItems()
	{
		var projectDropdown = Get<GameObject>((int)GameObjects.ProjectDropDown).GetOrAddComponent<TMP_Dropdown>();
		// projectDropdown Set
		{
			List<string> projectPaths = new List<string>();

			Task projectSet = Task.Run(() => { ReadProjects(out projectPaths); });

			await projectSet;


			projectDropdown.options.Clear();
			projectDropdown.options.Add(new TMP_Dropdown.OptionData());
			foreach (var project in projectPaths)
			{
				TMP_Dropdown.OptionData option = new TMP_Dropdown.OptionData();
				option.text = project;
				projectDropdown.options.Add(option);
			}

			projectDropdown.onValueChanged.AddListener((int index) => { ModeDropDownSet(index); });
		}

		var modeDropdown = Get<GameObject>((int)GameObjects.ModeDropDown).GetOrAddComponent<TMP_Dropdown>();
		// modeDropdown Set
		{
			modeDropdown.onValueChanged.AddListener((int index) => 
			{
				if (string.IsNullOrEmpty(modeDropdown.options[index].text))
					return;

				switch(modeDropdown.options[index].text)
				{
					case "검증":
						{
							if (_expertJointPaths.Count < 2 || GetPathEvent == null) return;
							GetPathEvent.Invoke(_expertJointPaths[0], _expertJointPaths[1]);
							break;
						}
					case "일반":
						{
							if (_simpleJointPaths.Count < 2 || GetPathEvent == null) return;
							GetPathEvent.Invoke(_simpleJointPaths[0], _simpleJointPaths[1]);
							break;
						}
					default: { Debug.LogError("ModeDropDown Error"); break; }
				}
			});
		}
	}

	void ReadProjects(out List<string> param)
	{
		param = new List<string>();

		///////////////////////////////////////////
		//				절대 경로 : 사건번호들이 있는 경로
		///////////////////////////////////////////
		//C:\Program Files (x86)\SEO\GaitDetectedAnalysis\datas
		string[] projectPaths = System.IO.Directory.GetDirectories(AbsPath, "*", SearchOption.TopDirectoryOnly);

		foreach(string project in  projectPaths)
		{
			string projectName = Util.GetDirName(project);
			param.Add(projectName);
		}
	}

	void ModeDropDownSet(int projectIndex)
	{
		var modeDropdown = Get<GameObject>((int)GameObjects.ModeDropDown).GetOrAddComponent<TMP_Dropdown>();

		modeDropdown.options.Clear();
		_simpleJointPaths.Clear();
		_expertJointPaths.Clear();

		string project = Get<GameObject>((int)GameObjects.ProjectDropDown).GetOrAddComponent<TMP_Dropdown>().options[projectIndex].text;
		if (string.IsNullOrEmpty(project))
			return;

		string dataPath = AbsPath + "\\" + project + "\\";
		string[] dataPaths = System.IO.Directory.GetFiles(dataPath, "*joint_xyz*", SearchOption.AllDirectories);

		if(dataPath.Length == 0)
		{
			Debug.LogWarning("No xyz data");
			return;
		}

		foreach (var data in dataPaths)
		{
			if (string.IsNullOrEmpty(data) || data.IndexOf("angle") == -1) continue;

			if(data.IndexOf("expert") != - 1)
			{
				_expertJointPaths.Add(data);
			}
			else if(data.IndexOf("simple") != -1)
			{
				_simpleJointPaths.Add(data);
			}
		}

		modeDropdown.options.Add(new TMP_Dropdown.OptionData());
		modeDropdown.options.Add(new TMP_Dropdown.OptionData() { text = "검증" });
		modeDropdown.options.Add(new TMP_Dropdown.OptionData() { text = "일반" });
		modeDropdown.value = 0;
	}

	void SetIndexBySlider(float value)
	{
		if (MaxFrame == 0)
			return;
		IndexField.text = ((int)(value * MaxFrame)).ToString();
	}
}
