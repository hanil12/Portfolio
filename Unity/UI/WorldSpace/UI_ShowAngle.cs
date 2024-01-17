using System.Collections;
using System.Collections.Generic;
using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.Diagnostics;
using UnityEngine.EventSystems;
using UnityEngine.UI;
using UnityEngine.UIElements;

public class UI_ShowAngle : UI_Base
{
    enum Texts
    {
        Angle
    }

    public enum TextPosition
    {
        None,
        Left,
        Right
    }

    public Transform TargetTransform { get { return _targetTransform; } }

    public float LineWidth { get; private set; } = 0.7f;

    private List<LineRenderer> _lines = new List<LineRenderer>();
    Material _red;

    public TextPosition TextPos_Overlay { get; set; } = TextPosition.None;
    private Transform _targetTransform = null;
    private TMP_Text _angleText;
    private UI_EventHandler _angleEventHandler;

    public bool Toggle { get; set; } = false;

    Vector3 _start = new Vector3 (0, 0, 0);
    Vector3 _mid = new Vector3(0,0, 0);
    Vector3 _end = new Vector3 (0, 0, 0);

	public override void Init()
	{
        Bind<TMP_Text>(typeof(Texts));
        _angleText = Get<TMP_Text>((int)Texts.Angle);
        SetUIEvent();

		CreateLines();
	}


    // Update is called once per frame
    void Update()
    {
        if(_targetTransform != null)
            transform.rotation = _targetTransform.rotation;
	}

    public void SetTarget(Transform target)
    {
        if (target != null)
            _targetTransform = target;
    }

    private void CreateLines()
    {
        _angleText = transform.GetComponentInChildren<TMP_Text>();

        _red = Resources.Load<Material>("Materials/Red");

        for(int i=0;i< 2; i++)
        {
			GameObject go = new GameObject();
			go.name = "Line" + (i + 1).ToString();
			go.transform.SetParent(transform);
			LineRenderer lineRenderer = go.AddComponent<LineRenderer>();
            lineRenderer.material = _red;
            lineRenderer.startWidth = LineWidth;
            lineRenderer.endWidth = LineWidth;

            Vector3[] arr = { Vector3.zero, Vector3.zero };
            lineRenderer.SetPositions(arr);
            lineRenderer.enabled = false;
            _lines.Add(lineRenderer);
		}
    }

    public void OnClickUI(PointerEventData eventData)
    {
        if(_lines.Count == 0 || eventData.button != PointerEventData.InputButton.Left) return;

        GameObject go = GameObject.Find("@Contents");
        if(go == null || go.transform.GetChild(0).gameObject.name == "SkeletonWebGL") return;

        for (int i = 0; i < go.transform.childCount; i++)
        {
            Util.FindChild<SkeletonLineRender>(go.transform.GetChild(i).gameObject, null, true).SetHighLightAngle_Init(transform.name);
        }

		Toggle = !Toggle;
        SetHighLight(Toggle);
        if (Toggle) CameraJoomIn();
        else CameraJoomOut();

		// ´Ù¸¥ ÂÊ ½ºÄÌ·¹Åæ HighLight
		for (int i = 0; i < go.transform.childCount; i++)
        {
            if (go.transform.GetChild(i).name == transform.parent.parent.name)
                continue;

            Transform Angle = Util.FindChild<Transform>(go.transform.GetChild(i).gameObject, transform.name, true);
            Angle.GetOrAddComponent<UI_ShowAngle>().SetHighLight(Toggle);
            Angle.GetOrAddComponent<UI_ShowAngle>().Toggle = Toggle;
            if(Toggle) Angle.GetOrAddComponent<UI_ShowAngle>().CameraJoomIn();
			else Angle.GetOrAddComponent<UI_ShowAngle>().CameraJoomOut();
		}
    }

	public void SetLinePosition(Vector3 start, Vector3 mid, Vector3 end)
    {
		_start = start;
		_mid = mid;
		_end = end;

		if (Toggle == false) return;

        if (_lines.Count == 0) return;

        Vector3[] arr1 = { _start, _mid };
        Vector3[] arr2 = { _mid, _end };
        _lines[0].SetPositions(arr1);
        _lines[1].SetPositions(arr2);
    }

    public void SetPosition(Vector3 pos)
    {
		switch (TextPos_Overlay)
		{
			case TextPosition.None:
				transform.localPosition = pos;
				break;
			case TextPosition.Left:
				transform.localPosition = pos + new Vector3(0,0,-1) * 7.0f;
				break;
			case TextPosition.Right:
                transform.localPosition = pos + new Vector3(0,0,1) * 7.0f;
				break;
		}
	}

	public void SetText(string value)
    {
        _angleText.text = value;
	}

    public void SetHighLight(bool isHighLight)
    {
		if (isHighLight)
        {
			foreach (var line in _lines)
			{
				line.enabled = true;
				line.material = _red;
			}

			Vector3[] arr1 = { _start, _mid };
			Vector3[] arr2 = { _mid, _end };
			_lines[0].SetPositions(arr1);
			_lines[1].SetPositions(arr2);

			_angleText.color = Color.white;
			_angleText.fontSize = 35;
			_angleText.fontStyle = FontStyles.Bold;

			_angleText.rectTransform.sizeDelta = new Vector2(120, 50);
        }
        else
        {
			foreach (var line in _lines)
			{
				line.enabled = false;
			}

			_angleText.color = Color.white;
			_angleText.fontSize = 12;
			_angleText.fontStyle = FontStyles.Normal;

			_angleText.rectTransform.sizeDelta = new Vector2(50, 30);
		}
    }

    public void CameraJoomIn()
    {
        if (Toggle == false)
            return;

        _targetTransform.GetOrAddComponent<MoveCamera>().SetOtherTargetEvent -= () => { _targetTransform.GetOrAddComponent<MoveCamera>().SetTargetUI(transform); };
        _targetTransform.GetOrAddComponent<MoveCamera>().SetOtherTargetEvent += () => { _targetTransform.GetOrAddComponent<MoveCamera>().SetTargetUI(transform); };

        _targetTransform.GetOrAddComponent<MoveCamera>().SaveOriginPos();
    }

    public void CameraJoomOut()
    {
        _targetTransform.GetOrAddComponent<MoveCamera>().SetTargetUI(null);
	}

    public void SetUIEvent()
    {
		_angleText.gameObject.BindEvent(OnClickUI);
	}

    public void ClearEvent()
    {
        _angleText.gameObject.ClearEvent();
	}
}
