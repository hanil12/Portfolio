using Data;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static Defines;

public class GameManager_Joint2
{
	public Defines.Skeleton Skeleton1 { get { return skeleton1; } }
	public Defines.Skeleton Skeleton2 { get { return skeleton2; } }
	public ref SkeletonAnimationData SkeletonData1 { get { return ref _skeletonAnimationData1; } }
	public ref SkeletonAnimationData SkeletonData2 { get { return ref _skeletonAnimationData2; } }

	private Defines.Skeleton skeleton1;
	private Defines.Skeleton skeleton2;
	private SkeletonAnimationData _skeletonAnimationData1 = null;
	private SkeletonAnimationData _skeletonAnimationData2 = null;

	public void CreateOrCheckSkeleton()
	{
		if(skeleton1 == null)
		{
			skeleton1 = new Defines.Skeleton();
			skeleton1.CreateSkeleton("Skeleton1", Vector3.zero);
		}

		if(skeleton2 == null)
		{
			skeleton2 = new Defines.Skeleton();
			skeleton2.CreateSkeleton("Skeleton2", Vector3.zero);
		}
	}

	public void Delete_Skeleton()
	{
		Manager.ResourceManager.Destroy(skeleton1.skeleton);
		Manager.ResourceManager.Destroy(skeleton2.skeleton);

		_skeletonAnimationData1 = null;
		_skeletonAnimationData2 = null;
	}

	public void Play()
	{
		if (!IsSkeletonReady()) return;

		skeleton1.controller.Play();
		skeleton2.controller.Play();
	}

	public void Pause()
	{
		if (!IsSkeletonReady()) return;

		skeleton1.controller.Pause();
		skeleton2.controller.Pause();
	}

	public void Stop()
	{
		if (!IsSkeletonReady()) return;

		skeleton1.controller.Stop();
		skeleton2.controller.Stop();
	}

	public void SetPlaySpeed(float speed)
	{
		if (!IsSkeletonReady()) return;

		skeleton1.controller.SetSpeed(speed);
		skeleton2.controller.SetSpeed(speed);
	}

	public bool IsSkeletonReady()
	{
		if (skeleton1 == null || skeleton2 == null) return false;
		if (_skeletonAnimationData1 == null || _skeletonAnimationData2 == null) return false;

		return true;
	}
}
