using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class Manager : MonoBehaviour
{
	[DllImport("user32.dll")]
	private static extern bool SetWindowPos(IntPtr hwnd, int hWndInsertAfter, int x, int y, int cx, int cy, uint uFlags);

	[DllImport("user32.dll")]
	private static extern IntPtr GetActiveWindow();


	public bool _isDebug = true;

    private static Manager       _instance;

	public static Manager Instance { get { Init(); return _instance; } }

	public static InputManager			InputManager { get { return Instance._inputManager; } }
	public static DataManager			DataManager { get { return Instance._dataManager; } }
	public static UIManager				UIManager { get { return Instance._uiManager; } }
	public static ResourceManager		ResourceManager { get { return Instance._resourceManager; } }
	public static GameManager_Joint2	GameManager { get { return Instance._gameManager; } }
	public static SceneManager_Joint2	SceneManager { get { return Instance._scene; } }

	// Core
	private InputManager         _inputManager = new InputManager();
    private DataManager          _dataManager = new DataManager();
    private ResourceManager		 _resourceManager = new ResourceManager();
    private UIManager            _uiManager = new UIManager();
	private SceneManager_Joint2 _scene = new SceneManager_Joint2();

	// Contents
	private GameManager_Joint2 _gameManager = new GameManager_Joint2();

	//Assets/Resources/output2.csv
	private const string _pathFolder = @"Assets\Resources\";

    private void Awake()
    {
        {
			/* â ũ��� ��ġ ����
			// ���ϴ� â ��ġ�� �����մϴ�.
			int windowWidth = 990; // â�� ���� ũ��
			int windowHeight = 1080; // â�� ���� ũ��

            // â ��ġ�� ���ʿ� ���̷��� X ��ġ�� 0���� �����մϴ�.
            //int windowX = -15; //left
            int windowX = 940; // right
			int windowY = (Screen.currentResolution.height - windowHeight) / 2; // �߾� ���� ����

            IntPtr hwnd = GetActiveWindow();


		    int HWND_TOP = 0;
        	//SetWindowPos(hwnd, HWND_TOP, windowX, 0,windowWidth - 20, windowHeight, 0x0040); // left
        	//SetWindowPos(hwnd, HWND_TOP, windowX, 0,windowWidth - 3, windowHeight, 0x0040); // right
			*/
        }
    }

    void Start()
    {

    }

    static void Init()
    {
		if (_instance == null)
		{
			GameObject go = GameObject.Find("@Manager");
			if (go == null)
			{
				go = new GameObject { name = "@Manager" };
				go.AddComponent<Manager>();
			}

			DontDestroyOnLoad(go);
			_instance = go.GetComponent<Manager>();

			_instance._dataManager.Init();
		}
	}

    void Update()
    {
        _inputManager.OnUpdate();
    }

	public void Clear()
	{
		InputManager.Clear();
		SceneManager.Clear();
		UIManager.Clear();
	}
}
