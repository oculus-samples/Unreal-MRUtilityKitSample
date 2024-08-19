# Documentation

The latest documentation for Mixed Reality Utility Kit can be found here:

https://developer.oculus.com/documentation/unreal/unreal-mr-utility-kit-overview/

## Samples

### Scene Decoration

The Scene Decorator is a sample that showcases how to use Unreal Engines procedural content generation framework (PCG) to decorate a scene in Mixed Reality.
The relevant files for this sample can be found in the folder SampleSceneDecoration. The sample itself is located in SampleSceneDecoration/SceneDecoration. To understand the sample better it is advised to get familiar with the basics of [PCG in Unreal Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/procedural-content-generation-overview).

There are two PCG graphs to do the whole work of scene decoration. That is PCG_Raycast and PCG_SceneDecoration.

PCG_SceneDecoration is the main PCG graph. It queries the scene data with the help of a custom PCG node called B_SceneRayAnchor, and then executes raycasts against the queried scene data. These raycasts are used to project a point grid onto the scene geometry. The execution of the raycast and projection is done as a subgraph called PCG_Raycast.
After that is done the points are filtered down like in any other PCG graph to obtain the final point distribution. Finally static meshes get spawned in the place of the points.

#### Scene Ray Anchor

The basic idea is to create a point grid and then project that points grid onto the walls, floor and ceiling. This can be done in PCG with a Create Points Grid World node, a World Ray Hit Query Node, and Projection node. The World Ray Hit Query nodes determine how the points grid gets projected by the Projection node.

For that to work we need to obtain the correct arguments for the raycast and point grid. This is where the custom PCG node Scene Ray Anchor helps.

Scene Ray Anchor is a custom PCG node that gets as input a scene label and an offset and direction for a ray cast. It will query all anchors with the given label and create a PCG point that contains all the information needed for a raycast against the scene geometry for the given label. The attributes in the PCG point get used in a very specific way. The Position contains the Ray Origin. The Scale contains the Ray Direction. The Bounds Max contains the Grid Extents for the point grid and the Rotation contains the Rotation for the point grid. Take a look at PCG_SceneDecoration and PCG_Raycast to see how this works in detail.

#### Distance Map integration

In MRUK we have the possibility to generate a distance map. A distance map stores information about how far a certain point is away from walls or the scene objects. The distance map can be combined with the scene decoration to spawn for example only things close to walls. How this can be done can be seen in PCG_SceneDecoration.

#### Import into your own project

Copy PCG_SceneDecoration, PCG_Raycast, BP_SceneRayAnchor, BP_CreatePointsGrid into your project.

## How to Use

### Load the project

First, ensure you have Git LFS installed by running this command:
```sh
git lfs install
```

Then, clone this repo using the "Code" button above, or this command:
```sh
git clone https://github.com/oculus-samples/Unreal-MRUtilityKitSample
```

### Launch the project in the Unreal Editor using one of the following options.

#### Epic Games Launcher with MetaXR plugin

The easiest way to get started is to use the prebuilt Unreal Engine from the Epic Games Launcher, with MetaXR plugin.

1. Install the [Epic Games Launcher](https://www.epicgames.com/store/en-US/download)
2. In the launcher, install UE5 (recommended).
3. Download and install the MetaXR plugin from the [Unreal Engine 5 Integration download page](https://developer.oculus.com/downloads/package/unreal-engine-5-integration).
4. Launch the Unreal Editor
5. From "Recent Projects", click "Browse" and select `MRUtilityKitSample.uproject`

#### Meta fork of Epic’s Unreal Engine

The Meta fork of Epic’s Unreal Engine will give you the most up to date integration of Oculus features. However, you must build the editor from its source.

Follow the instructions on [Accessing Unreal Engine source code on GitHub](https://www.unrealengine.com/en-US/ue-on-github) to obtain:
- an Epic account
- a GitHub account
- authorization to access the Unreal Engine source repository
Disregard instructions on downloading Epic’s Unreal Engine source code as you will be building the Meta fork of Epic’s Unreal Engine source.

Make sure you have Visual Studio installed properly:
- Launch the Visual Studio Installer and click Modify for the Visual Studio version you want to use.
- Under the Workloads tab, click Game development with C++ if it isn’t checked and then click Modify.

1. Download the source code from the [Meta fork of Epic’s Unreal Engine on GitHub](https://github.com/Oculus-VR/UnrealEngine).
2. Open a command prompt in the root of the Unreal, then run this command:
```sh
.\GenerateProjectFiles.bat -Game MRUtilityKitSample -Engine <full path to Unreal-MRUtilityKitSample directory>\MRUtilityKitSample.uproject
```
3. Open the `MRUtilityKitSample.sln` file that has been generated in the `Unreal-MRUtilityKitSample` directory.
4. Set `MRUtilityKitSample` as the start-up project and `Development Editor` as the configuration.
5. Hit `F5` to build and debug the project (and the engine).
    - Depending on your machine, the build may take awhile to complete.

# Licenses
The Meta License applies to the SDK and supporting material. The MIT License applies to only certain, clearly marked documents. If an individual file does not indicate which license it is subject to, then the Meta License applies.
