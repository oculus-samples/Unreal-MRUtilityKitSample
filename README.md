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

#### How to use it

You want to copy PCG_SceneDecoration, PCG_Raycast, BP_SceneRayAnchor, BP_CreatePointsGrid into your project. In addition you need to copy from the MRUtilityKitSample source folder MRUtilityKitPCG.cpp and MRUtilityPCG.h to your projects source folder. That means your project needs to be a C++ project.

# Licenses
The Meta License applies to the SDK and supporting material. The MIT License applies to only certain, clearly marked documents. If an individual file does not indicate which license it is subject to, then the Meta License applies.
