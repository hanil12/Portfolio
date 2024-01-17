using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UI_VideoBtns : UI_Base
{
	enum Buttons
	{
		Play,
		Pause,
		Stop,
		X2_Btn,
		X4_Btn
	}

	private short _playStateFlag = 0;
	private const short PLAY_FLAG = 1 << 0;
	private const short PAUSE_FLAG = 1 << 1;
	private const short STOP_FLAG = 1 << 2;

	private short _speedStateFlag = 0;
	private const short X2_FLAG = 1 << 0;
	private const short X4_FLAG = 1 << 1;

	private bool _isPlaying = false;

	public override void Init()
	{
		Bind<Button>(typeof(Buttons));

		// PlayButtons
		var playEventHandler = GetButton((int)Buttons.Play).GetOrAddComponent<UI_EventHandler>();
		playEventHandler.OnClickHandler -= (PointerEventData data) => { Play(); };
		playEventHandler.OnClickHandler += (PointerEventData data) => { Play(); };
		GetButton((int)Buttons.Pause).GetOrAddComponent<UI_EventHandler>();
		var pauseEventHandler = GetButton((int)Buttons.Pause).GetOrAddComponent<UI_EventHandler>();
		pauseEventHandler.OnClickHandler -= (PointerEventData data) => { Pause(); };
		pauseEventHandler.OnClickHandler += (PointerEventData data) => { Pause(); };
		var stopEventHandler = GetButton((int)Buttons.Stop).GetOrAddComponent<UI_EventHandler>();
		stopEventHandler.OnClickHandler -= (PointerEventData data) => { Stop(); };
		stopEventHandler.OnClickHandler += (PointerEventData data) => { Stop(); };

		// SpeedButtons
		var x2SpeedEventHandler = GetButton((int)Buttons.X2_Btn).GetOrAddComponent<UI_EventHandler>();
		x2SpeedEventHandler.OnClickHandler -= (PointerEventData data) => { X2_Speed(); };
		x2SpeedEventHandler.OnClickHandler += (PointerEventData data) => { X2_Speed(); };

		var x4SpeedEventHandler = GetButton((int)Buttons.X4_Btn).GetOrAddComponent<UI_EventHandler>();
		x4SpeedEventHandler.OnClickHandler -= (PointerEventData data) => { X4_Speed(); };
		x4SpeedEventHandler.OnClickHandler += (PointerEventData data) => { X4_Speed(); };

		// SpaceBar Input
		Manager.InputManager.KeyAction -= PlayOrResume;
		Manager.InputManager.KeyAction += PlayOrResume;
	}

	bool CheckScene()
	{
		try
		{
			if (Manager.GameManager.IsSkeletonReady() == false) return false;
		}
		catch
		{
			return false;
		}

		return true;
	}

	public void Play()
	{
		if (CheckScene() == false) return;

		if ((_playStateFlag & PLAY_FLAG) > 0)
		{
			_playStateFlag &= ~PLAY_FLAG;
			_isPlaying = false;
		}
		else
		{
			_playStateFlag = 0;
			_playStateFlag |= PLAY_FLAG;
			_isPlaying = true;
		}

		Execute_Play();
	}

	public void Pause()
	{
		if (CheckScene() == false) return;

		if ((_playStateFlag & PAUSE_FLAG) > 0)
		{
			_playStateFlag &= ~PAUSE_FLAG;
		}
		else
		{
			_playStateFlag = 0;
			_playStateFlag |= PAUSE_FLAG;
		}

		_isPlaying = false;

		Execute_Play();
	}

	public void Stop()
	{
		if (CheckScene() == false) return;

		if ((_playStateFlag & STOP_FLAG) > 0)
		{
			_playStateFlag &= ~STOP_FLAG;
		}
		else
		{
			_playStateFlag = 0;
			_playStateFlag |= STOP_FLAG;
		}

		_isPlaying = false;

		Execute_Play();
	}

	public void X2_Speed()
	{
		if (CheckScene() == false) return;

		if ((_speedStateFlag & X2_FLAG) > 0)
		{
			_speedStateFlag &= ~X2_FLAG;
		}
		else
		{
			_speedStateFlag = 0;
			_speedStateFlag |= X2_FLAG;
		}

		Execute_Speed();
	}

	public void X4_Speed()
	{
		if (CheckScene() == false) return;

		if ((_speedStateFlag & X4_FLAG)  > 0)
		{
			_speedStateFlag &= ~X4_FLAG;
		}
		else
		{
			_speedStateFlag = 0;
			_speedStateFlag |= X4_FLAG;
		}

		Execute_Speed();
	}

	void Execute_Play()
	{
		var playBtn = GetButton((int)Buttons.Play);
		var pauseBtn = GetButton((int)Buttons.Pause);
		var stopBtn = GetButton((int)Buttons.Stop);

		playBtn.image.sprite = playBtn.spriteState.disabledSprite;
		pauseBtn.image.sprite = pauseBtn.spriteState.disabledSprite;
		stopBtn.image.sprite = stopBtn.spriteState.disabledSprite;

		switch(_playStateFlag)
		{
			case 0:
				{
					// 일시정지
					try
					{
						Manager.GameManager.Pause();
					}
					catch
					{
						break;
					}

					break;
				}

			case PLAY_FLAG:
				{
					// 재생
					try
					{
						Manager.GameManager.Play();
					}
					catch
					{
						break;
					}
					playBtn.image.sprite = playBtn.spriteState.selectedSprite;
					break;
				}

			case PAUSE_FLAG:
				{
					// 일시정지
					try
					{
						Manager.GameManager.Pause();
					}
					catch
					{
						break;
					}
					pauseBtn.image.sprite = pauseBtn.spriteState.selectedSprite;
					break;
				}
			case STOP_FLAG:
				{
					// 정지
					try
					{
						Manager.GameManager.Stop();
					}
					catch
					{
						break;
					}
					stopBtn.image.sprite = stopBtn.spriteState.selectedSprite;
					break;
				}
		}
	}

	void Execute_Speed()
	{
		var x2Btn = GetButton((int)Buttons.X2_Btn);
		var x4Btn = GetButton((int)Buttons.X4_Btn);

		x2Btn.image.sprite = x2Btn.spriteState.disabledSprite;
		x4Btn.image.sprite = x4Btn.spriteState.disabledSprite;

		switch (_speedStateFlag)
		{
			case 0:
				{
					// 정속
					try
					{
						Manager.GameManager.SetPlaySpeed(1.0f);
					}
					catch
					{
						break;
					}

					break;
				}

			case X2_FLAG:
				{
					// 2배속
					try
					{
						Manager.GameManager.SetPlaySpeed(2.0f);
					}
					catch
					{
						break;
					}
					x2Btn.image.sprite = x2Btn.spriteState.selectedSprite;
					break;
				}

			case X4_FLAG:
				{
					// 4배속
					try
					{
						Manager.GameManager.SetPlaySpeed(4.0f);
					}
					catch
					{
						break;
					}
					x4Btn.image.sprite = x4Btn.spriteState.selectedSprite;
					break;
				}
		}
	}

	public void PlayOrResume()
	{
		try
		{
			if (Manager.GameManager.IsSkeletonReady() == false) return;
		}
		catch
		{
			return;
		}

		if (!Input.GetKeyDown(KeyCode.Space)) return;

		if (_isPlaying)
			Pause();
		else
			Play();
	}
}
