using Data;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Net.Mail;
using Unity.VisualScripting;
using UnityEngine;
using static TMPro.SpriteAssetUtilities.TexturePacker_JsonArray;

public class DataManager
{
	public string EventNumber { get; set; }

	public Action LoadDoneEvt;

	public void Init()
	{
	}

	public void LoadByPath_Resources(string filename, out Data.SkeletonAnimationData skeletonAnimationData)
	{
		string temp = Resources.Load<TextAsset>(filename).ToString();

		LoadByCSVString_XYZ(temp, out skeletonAnimationData);
	}

	public void LoadByPath_Directory(string path, out Data.SkeletonAnimationData skeletonAnimationData)
	{
		string temp = File.ReadAllText(path);
		
		LoadByCSVString_XYZ(temp, out skeletonAnimationData);
	}

	public void LoadByCSVString_XYZ(string data,out Data.SkeletonAnimationData skeletonAnimationData)
	{
		if (data.Length == 0)
		{
			skeletonAnimationData = null;
			return;
		}

		data = data.Replace("\r", "");
		data = data.Replace("\n", "+");

		Make_SkeletonAnimationData(data,out skeletonAnimationData);

		if(skeletonAnimationData == null)
		{
			Debug.LogError("Skeleton Data is Null");
		}
		int typeNum = (int)skeletonAnimationData.SkeletonData.Type;
		int[] arr = (int[])typeof(Defines.JointCount).GetEnumValues();
		Debug.Log($"Frame Count : {skeletonAnimationData.PointPerFrame.Count}, JointPerFrame : {arr[typeNum]}");

		if(LoadDoneEvt != null)
			LoadDoneEvt.Invoke();
	}

	private int GetJointCount(string[] csvSplit)
	{
		int count = 0;

		int index = 1;
		while (true)
		{
			string[] splitTwo = csvSplit[index].Split(',');
			if (splitTwo[1] == "0")
				count++;
			else break;

			index++;
		}

		return count;
	}

	public void Make_SkeletonAnimationData(string csv,out SkeletonAnimationData skeletonAnimationData)
	{
		Defines.SkeletonType skeletonType = Defines.SkeletonType.None;
		List<Dictionary<int, Vector3>> pointPerFrame;

		List<Vector3> vectorList = new List<Vector3>();

		string[] splitOne = csv.Split("+");
		int count = GetJointCount(splitOne);

		for (int i = 1; i < splitOne.Length; i += count)
		{
			for (int j = 0; j < count; j++)
			{
				if (i + j >= splitOne.Length) break;

				string line = splitOne[i + j];
				if (line == string.Empty) continue;

				line = line.Substring(line.IndexOf('(') + 1, line.IndexOf(')') - line.IndexOf('(') - 1);
				var val = line.Split(',');
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;

				if (float.TryParse(val[0], out x) == false || float.TryParse(val[1], out y) == false || float.TryParse(val[2], out z) == false)
					continue;

				x *= 150;
				y *= -150;
				z *= -150;

				vectorList.Add(new Vector3(x, y, z));
			}
		}

		switch (count)
		{
			case (int)Defines.JointCount.HybrikCount:
				{
					skeletonType = Defines.SkeletonType.Hybrik;
					break;
				}
			case (int)Defines.JointCount.IMUCount:
				{
					skeletonType = Defines.SkeletonType.IMU;
					break;
				}
			case (int)Defines.JointCount.MHFCount:
				{
					skeletonType = Defines.SkeletonType.MHF;
					break;
				}

			default: { Debug.Assert(false); break; }
		}

		pointPerFrame = MakePointPerFrame(vectorList, skeletonType);

		skeletonAnimationData = new SkeletonAnimationData(skeletonType, pointPerFrame, null);
	}

	public List<Dictionary<int, Vector3>> MakePointPerFrame(List<Vector3> vectorList, Defines.SkeletonType type)
	{
		List<Dictionary<int, Vector3>> pointPerFrame = new List<Dictionary<int, Vector3>>();
		int typeNum = (int)type;
		int[] arr = (int[])typeof(Defines.JointCount).GetEnumValues();

		int y = vectorList.Count / arr[typeNum];
		int x = arr[typeNum];

		for (int i = 0; i < y; i++)
		{
			Dictionary<int, Vector3> frame = new Dictionary<int, Vector3>();
			for (int j = 0; j < x; j++)
			{
				frame[j] = vectorList[i * x + j];
			}

			pointPerFrame.Add(frame);
		}

		return pointPerFrame;
	}

	public void LoadByCSVString_Angle(string data, SkeletonAnimationData skeletonAnimationData)
	{
		data = data.Replace("\r", "+");
		data = data.Replace("\n", "+");
		SetCSVAngle(data,skeletonAnimationData);
	}

	public void SetCSVAngle(string data, SkeletonAnimationData skeletonAnimationData)
	{
		skeletonAnimationData.AnglePerFrame.Clear();

		string[] strArr = data.Split("++");

		for (int i = 0; i < SkeletonAnimationData.betweenJoint.Length; i++) { SkeletonAnimationData.betweenJoint[i] = SkeletonAnimationData.betweenJoint[i].Trim(); }

		for (int i= 1; i < strArr.Length; i++)
		{
			string[] angles = strArr[i].Split(",");
			Dictionary<string, float> dic = new Dictionary<string, float>();
			for(int k = 1; k < angles.Length; k++)
			{
				float angle = 0.0f;
				if (float.TryParse(angles[k], out angle) == false) continue;

				dic[SkeletonAnimationData.betweenJoint[k - 1]] = angle;
			}

			if(dic.Count > 0) { skeletonAnimationData.AnglePerFrame.Add(dic); }
		}

		Debug.Log($"Angle_Frames : {skeletonAnimationData.AnglePerFrame.Count} , AnglesPerFrame : {skeletonAnimationData.AnglePerFrame[0].Count}");
	}
}
