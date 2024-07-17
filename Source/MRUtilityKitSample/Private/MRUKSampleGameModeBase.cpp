/*
Copyright (c) Meta Platforms, Inc. and affiliates.
All rights reserved.

This source code is licensed under the license found in the
LICENSE file in the root directory of this source tree.
*/

#include "MRUKSampleGameModeBase.h"

#include "IHeadMountedDisplay.h"
#include "IXRTrackingSystem.h"

/**
 * Checks if the current game mode is running in editor without VR preview.
 *
 * @return true if the game mode is running in editor without VR preview, false otherwise
 */
bool AMRUKSampleGameModeBase::IsEditorNoVRPreview()
{
	if (!GEngine || !GEngine->XRSystem.IsValid())
	{
		return false;
	}
	const auto HMDDevice = GEngine->XRSystem->GetHMDDevice();
	if (!HMDDevice || !HMDDevice->IsHMDConnected() || !HMDDevice->IsHMDEnabled())
	{
		return false;
	}
	const auto StereoRenderingDevice = GEngine->XRSystem->GetStereoRenderingDevice();
	if (!StereoRenderingDevice || !StereoRenderingDevice->IsStereoEnabled())
	{
		return false;
	}
	return true;
}
