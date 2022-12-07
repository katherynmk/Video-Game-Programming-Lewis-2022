import bpy
from bpy.props import StringProperty
from bpy_extras.io_utils import ExportHelper
from io import BytesIO
from mathutils import Matrix
from mathutils import Quaternion
from mathutils import Vector
import json
import math
import os
import struct
import zlib

# Collection of children of a given scene node.
SCENE_NODE_CHILDREN_KEY = "Children"
# Type of a given scene node.
SCENE_NODE_TYPE_KEY = "Type"
# Name of the type for the base scene node class.
SCENE_NODE_TYPE_NAME = "SceneNode"
# Position of the node relative to its parent.
SCENE_NODE_POSITION_OFFSET_KEY = "PositionOffset"
# Rotation of the node relative to its parent.
SCENE_NODE_ROTATION_KEY = "Rotation"
# Scale of the node relative to its parent.
SCENE_NODE_SCALE_KEY = "Scale"
# Collection of data for meshes.
MESHES_KEY = "Meshes"
# Name of a bone in a mesh's collection of bone data.
MESH_BONE_NAME_KEY = "Name"
# Collection of bone influence data for a submesh.
MESH_SUBMESH_BONES_KEY = "Bones"
# Name of the bone.
MESH_SUBMESH_BONE_NAME_KEY = "Name"
# Temporary collection of bone vertex influences for before normalization is
# applied.
MESH_SUBMESH_BONE_VERTEX_INFLUENCES_TEMP_KEY = "VertexInfluencesTemp"
# Part of the specifications of a submesh containing the morph targets.
MESH_SUBMESH_MORPH_TARGETS_KEY = "MorphTargets"
# Collection of data for materials.
MATERIALS_KEY = "Materials"
# Texture to use for the base color of a material.
MATERIAL_BASE_COLOR_TEXTURE_KEY = "BaseColorTexture"
# Texture to use for the normal map of a material.
MATERIAL_NORMAL_TEXTURE_KEY = "NormalTexture"
# Texture to use for combined metallic, roughness, and occlusion.
MATERIAL_METALLIC_ROUGHNESS_OCCLUSION_TEXTURE_KEY = "MetallicRoughnessOcclusionTexture"
# Texture to use for emission of a material.
MATERIAL_EMISSION_TEXTURE_KEY = "EmissionTexture"
# How to interpolate beteween texture values.
MATERIAL_TEXTURE_FILTER_MODE_KEY = "FilterMode"
# How to extend a texture beyond the texcoord range [0, 1] for the U axis.
MATERIAL_TEXTURE_WRAP_MODE_U_KEY = "WrapModeU"
# How to extend a texture beyond the texcoord range [0, 1] for the V axis.
MATERIAL_TEXTURE_WRAP_MODE_V_KEY = "WrapModeV"
# True if the material should be drawn with an outline.
MATERIAL_OUTLINE_DRAW_KEY = "OutlineDraw"
# True if the material should be affected by the wind.
MATERIAL_WIND_APPLY_KEY = "WindApply"
# Maximum number of vertices in a single vertex buffer.  (1 less than 2^16)
VERTEX_BUFFER_VERTEX_COUNT_MAX = 65535
# Binary data for a vertex or index buffer.
BUFFER_DATA_KEY = "Data"
# Meaning of a vertex attribute.  (Position, Normal, etc.)
VERTEX_ATTRIBUTE_SEMANTIC_KEY = "Semantic"
# Index to differentiate between vertex attributes with the same semantic.
# (TexCoord0, TexCoord1, etc)
VERTEX_ATTRIBUTE_SEMANTIC_INDEX_KEY = "SemanticIndex"
# Name given to a vertex attribute.
VERTEX_ATTRIBUTE_NAME_KEY = "Name"
# Index of the vertex buffer within the relevant submesh containing the data for
# this vertex attribute.
VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY = "VertexBufferIndex"
# Offset in bytes from the beginning of the vertex buffer to the start of this
# attribute's data.
VERTEX_ATTRIBUTE_OFFSET_KEY = "Offset"
# Number of bytes between the beginning of data for one vertex and the beginning
# of data for the next for this attribute.
VERTEX_ATTRIBUTE_STRIDE_KEY = "Stride"
# Number type of the components of this attribute.  (uint16, float32, uint8,
# etc.)
VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY = "ComponentType"
# Number of components of the data for one vertex for this attribute.  (3 for a
# Point3F, 2 for a Point2I, etc.)
VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY = "ComponentCount"
# True if the components should be normalized to [0, 1] or [-1, 1].  For
# example, this would be true for vertex colors if the components were [0, 255]
# but the meaning was [0, 1].
VERTEX_ATTRIBUTE_NORMALIZED_KEY = "Normalized"
# Name of the vertex attribute to use for wind information.
VERTEX_ATTRIBUTE_WIND_NAME = "Wind"
# Number type to indicate a 32-bit float.
COMPONENT_TYPE_FLOAT32 = "float32"
# Number type to indicate a uint8 float.
COMPONENT_TYPE_UINT8 = "uint8"
# Name of the object to be affected by the animation channel.
ANIMATION_CHANNEL_TARGET_OBJECT_KEY = "TargetObject"
# Type of the object to be affected by the animation channel.
ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_KEY = "TargetObjectType"
# Name of an ancestor of the object to be affected by the animation channel.
ANIMATION_CHANNEL_TARGET_OBJECT_ANCESTOR_KEY = "TargetObjectAncestor"
# Name of the subobject to be affected by the animation channel.
ANIMATION_CHANNEL_TARGET_SUBOBJECT_KEY = "TargetSubobject"
# Type of the subobject to be affected by the animation channel.
ANIMATION_CHANNEL_TARGET_SUBOBJECT_TYPE_KEY = "TargetSubobjectType"
# "TargetObjectType" value for a SceneNode.
ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_SCENE_NODE_NAME = "SceneNode"
# "TargetObjectType" value for a morph target.
ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_MORPH_TARGET_NAME = "MorphTarget"
# Property of the object to be affected by the animation channel.
ANIMATION_CHANNEL_TARGET_PROPERTY_KEY = "TargetProperty"
# "TargetProperty" for a weight
ANIMATION_CHANNEL_TARGET_PROPERTY_WEIGHT_NAME = "Weight"
# Format of the channel data.
ANIMATION_CHANNEL_TYPE_KEY = "Type"
# Channel data intended for a SplineLinear<float>
ANIMATION_CHANNEL_TYPE_FLOAT_SPLINE_LINEAR_NAME = "FloatSplineLinear"
# Channel data intended for a SplineLinear<Point3F>
ANIMATION_CHANNEL_TYPE_POINT3F_SPLINE_LINEAR_NAME = "Point3FSplineLinear"
# Channel data intended for a SplineLinear<Quaternion>
ANIMATION_CHANNEL_TYPE_QUATERNION_SPLINE_LINEAR_NAME = "QuaternionSplineLinear"
# Array of points to provide to a SlineLinear.
ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY = "Points"
# Time for a particular point in a SplineLinear.
SPLINE_LINEAR_POINT_TIME_KEY = "Time"
# Value for a particular point in a SplineLinear.
SPLINE_LINEAR_POINT_VALUE_KEY = "Value"
# Name of the vertex group for specifying how much a vertex should lean due to
# the wind.
VERTEX_GROUP_WIND_LEAN_NAME = "DuckWindLean"
# Name of the vertex group for specifying how much a vertex should shake due to
# the wind.
VERTEX_GROUP_WIND_SHAKE_NAME = "DuckWindShake"
# Name of the vertex group for additional, probably high-frequency, shaking due
# to the wind.
VERTEX_GROUP_WIND_SHAKE_DETAIL_NAME = "DuckWindShakeDetail"
# Name of the vertex group for specifying how to modify the timing of how a
# vertex should shake due to the wind.
VERTEX_GROUP_WIND_SHAKE_PHASE_NAME = "DuckWindShakePhase"
# Name of the object bake type that indicates an object should be exported as a
# mesh with animation baked into an animated series of morph targets.
DUCK_OBJECT_BAKE_TYPE_MORPH_TARGETS_NAME = "MorphTargets"

bl_info = {
   "name": "Duck Exporter",
   "description": "Export scenes to the Duck format.",
   "version": (1, 0),
   "blender": (2, 80, 0),
   "location": "File > Export > Duck",
   "warning": "",
   "category": "Import-Export"
}

#===============================================================================

# Helper function for converting a tree of data to BSON.
class BSONWriter:
   output = None

   # Write the tree of data to the given file.
   def Save(self, _output, root):
      if not isinstance(root, dict):
         return False
      self.output = _output
      return self.ValueWrite(root)
      self.output = None

   #----------------------------------------------------------------------------

   # Write the given value to the file.
   def ValueWrite(self, value):
      if value == None:
         # Null
         # Nothing to write
         return True
      elif isinstance(value, bool):
         # Boolean
         if value:
            self.output.write(b"\x01")
         else:
            self.output.write(b"\x00")
      elif isinstance(value, float):
         # double
         self.output.write(struct.pack("<d", value))
      elif isinstance(value, int):
         if (value >= -2147483648) and (value <= 2147483647):
            # int32
            self.output.write(struct.pack("<i", value))
         else:
            # int64
            self.output.write(struct.pack("<q", value))
      elif isinstance(value, str):
         # String
         # Write the size of the string, including the null terminator.
         self.output.write(struct.pack("<i", len(value) + 1))
         # Write the string.
         if len(value) > 0:
            self.output.write(value.encode(encoding="utf-8"))
         # Add the null terminator.
         self.output.write(b"\x00")
      elif isinstance(value, bytes):
         # Binary data
         # Write the size of the buffer.
         self.output.write(struct.pack("<i", len(value)))
         # For now, list all binary data as being generic.
         self.output.write(b"\x00")
         # Write the binary data.
         self.output.write(value)
      elif isinstance(value, dict):
         # Object
         # Reserve a speace at the beginning for the size.
         beginPosition = self.output.tell()
         self.output.write(struct.pack("<i", 0))
         # Write the children.
         for childKey, childValue in sorted(value.items()):
            # Write the child type
            if not self.TypeWrite(childValue):
               return False
            # Write the child key, including the null terminator.
            self.output.write(childKey.encode(encoding="utf-8"))
            self.output.write(b"\x00")
            # Write the child value.
            if not self.ValueWrite(childValue):
               return False
         # Write the 1-byte zero at the end.
         self.output.write(b"\x00")
         # Go back to the beginning, and write the size of this collection.
         resumePosition = self.output.tell()
         self.output.seek(beginPosition)
         self.output.write(struct.pack("<i", resumePosition - beginPosition))
         # Return to the end of the file.
         self.output.seek(resumePosition)
      elif isinstance(value, list) or isinstance(value, tuple):
         # Array
         # Reserve a speace at the beginning for the size.
         beginPosition = self.output.tell()
         self.output.write(struct.pack("<i", 0))
         # Write the children
         childIndex = 0
         for childValue in value:
            # Write the child type
            if not self.TypeWrite(childValue):
               return False
            # Write the child key, including the null terminator.
            self.output.write(("%d" % childIndex).encode(encoding="utf-8"))
            self.output.write(b"\x00")
            childIndex += 1
            # Write the child value.
            if not self.ValueWrite(childValue):
               return False
         # Write the 1-byte zero at the end.
         self.output.write(b"\x00")
         # Go back to the beginning, and write the size of this collection.
         resumePosition = self.output.tell()
         self.output.seek(beginPosition)
         self.output.write(struct.pack("<i", resumePosition - beginPosition))
         # Return to the end of the file.
         self.output.seek(resumePosition)
      else:
         return False
      return True

   #----------------------------------------------------------------------------

   # Write the type for the given value.
   def TypeWrite(self, value):
      if value == None:
         # Null
         self.output.write(b"\x0A")
      elif isinstance(value, bool):
         # Boolean
         self.output.write(b"\x08")
      elif isinstance(value, float):
         # double
         self.output.write(b"\x01")
      elif isinstance(value, int):
         if(value >= -2147483648) and (value <= 2147483647):
            # int32
            self.output.write(b"\x10")
         else:
            # int64
            self.output.write(b"\x12")
      elif isinstance(value, str):
         # String
         self.output.write(b"\x02")
      elif isinstance(value, bytes):
         # Binary data
         self.output.write(b"\x05")
      elif isinstance(value, dict):
         # Object
         self.output.write(b"\x03")
      elif isinstance(value, list) or isinstance(value, tuple):
         # Array
         self.output.write(b"\x04")
      else:
         return False
      return True

#===============================================================================

class DuckExporter(bpy.types.Operator, ExportHelper):
   bl_idname = "export_scene.duck"
   bl_label = "Export Duck"
   filename_ext = ".dsc"
   filter_glob: StringProperty(default="*.dsc", options={"HIDDEN"})

   def execute(self, context):
      # Switch to object mode since certain data (like UVs) don't seem to be
      # accessible in edit mode.
      if(bpy.context.object):
         bpy.ops.object.mode_set(mode="OBJECT")

      # Go to the first frame, even if this is not exporting animation data, so
      # the result of exporting won't depend as much on the current position in
      # the timeline when the scene was exported.  For example, this can affect
      # how baked morph target animations are exported.
      restoreFrameNumber = bpy.context.scene.frame_current
      bpy.context.scene.frame_set(max(0, bpy.context.scene.duck_animation_frame_begin))

      try:
         # Gather the data to be exported.
         fileData = {}
         nodesGathered = []
         poseBonesGathered = []
         shapeKeysGathered = []
         submeshExportAsMorphTargetsSamplers = []
         sceneSpecs = fileData["Scene"] = {}
         sceneRootNodeSpecs = sceneSpecs["Root"] = {}
         #sceneRootNodeSpecs[SCENE_NODE_TYPE_KEY] = "SceneNode"
         sceneRootNodeChildrenSpecs = sceneRootNodeSpecs[SCENE_NODE_CHILDREN_KEY] = []
         for node in bpy.context.scene.objects.values():
            if node.parent != None:
               continue
            self.GatherNode(node, fileData, sceneRootNodeChildrenSpecs, nodesGathered, poseBonesGathered, shapeKeysGathered, submeshExportAsMorphTargetsSamplers)

         # Export an animation if needed.
         if bpy.context.scene.duck_export_animation:
            self.GatherAnimation(nodesGathered, poseBonesGathered, shapeKeysGathered, submeshExportAsMorphTargetsSamplers, fileData)

         #print(json.dumps(fileData, sort_keys = True, indent = 3, default = DebugJSONSerializer))

         # Convert the data to BSON, and put it in a buffer in RAM.
         bsonData = BytesIO()
         bsonWriter = BSONWriter()
         bsonWriter.Save(bsonData, fileData)
         bsonDataSize = len(bsonData.getvalue())
         if bsonDataSize > 4294967296:
            bpy.context.scene.frame_set(restoreFrameNumber)
            return {"CANCELLED"}

         # Create a compressed version of the BSON data.
         compressedBSONData = BytesIO()
         compressor = zlib.compressobj(wbits=-15)
         compressedBSONData.write(compressor.compress(bsonData.getvalue()))
         compressedBSONData.write(compressor.flush())

         # Write the compressed data to a file.
         outputFile = open(self.filepath, "wb")
         if outputFile:
            # Add the magic number.
            outputFile.write(struct.pack("<BBBB", 102, 99, 102, 0)) # fcf\0
            # Add the container version number.
            outputFile.write(struct.pack("<I", 1))
            # Add the content version number.
            outputFile.write(struct.pack("<I", 0))
            # Add the decompressed size in bytes.
            outputFile.write(struct.pack("<Q", bsonDataSize))
            # Add the compressed data.
            outputFile.write(compressedBSONData.getvalue())
            outputFile.close()
      except:
         bpy.context.scene.frame_set(restoreFrameNumber)
         raise

      bpy.context.scene.frame_set(restoreFrameNumber)
      return {"FINISHED"}

   #----------------------------------------------------------------------------

   # Gather data for a node and its decendants.
   def GatherNode(self, node, fileData, parentChildrenSpecs, nodesGathered, poseBonesGathered, shapeKeysGathered, submeshExportAsMorphTargetsSamplers):
      # See if this node and its descendants should be skipped.
      if not node.duck_export:
         return

      # Start the specs for the node.
      nodeSpecs = {"Name": node.name}
      parentChildrenSpecs.append(nodeSpecs)
      bakeAsMesh = node.duck_object_bake_type == DUCK_OBJECT_BAKE_TYPE_MORPH_TARGETS_NAME
      if (node.type == "MESH") or (bakeAsMesh and ((node.type == "SURFACE") or (node.type == "META") or (node.type == "FONT"))):
         self.GatherNodeMesh(node, fileData, nodeSpecs, shapeKeysGathered, submeshExportAsMorphTargetsSamplers)
      elif node.type == "LIGHT":
         self.GatherNodeLight(node, nodeSpecs)
      elif (node.type == "EMPTY") and (node.empty_display_type == "SPHERE"):
         self.GatherNodeHelperSphere(node, nodeSpecs)
      elif node.type == "ARMATURE":
         self.GatherNodeArmature(node, nodeSpecs, poseBonesGathered)
      elif node.type == "CAMERA":
         nodeSpecs[SCENE_NODE_TYPE_KEY] = "Camera"

      # Add this to the collection of nodes included in the export.
      nodesGathered.append(node)

      # Get the transform of the node relative to its parent.
      if node.parent:
         parentTransform = node.parent.matrix_world
      else:
         parentTransform = Matrix()
      nodeTransformLocal = parentTransform.inverted_safe() @ node.matrix_world

      nodePositionLocal = nodeTransformLocal.to_translation()
      nodeRotationLocal = nodeTransformLocal.to_quaternion()
      nodeScaleLocal = nodeTransformLocal.to_scale()

      nodeSpecs[SCENE_NODE_POSITION_OFFSET_KEY] = Point3FStringBuild((nodePositionLocal.x, nodePositionLocal.y, nodePositionLocal.z))
      nodeSpecs[SCENE_NODE_ROTATION_KEY] = QuaternionStringBuild((nodeRotationLocal.w, nodeRotationLocal.x, nodeRotationLocal.y, nodeRotationLocal.z))
      if (nodeScaleLocal.x != 1.0) or (nodeScaleLocal.y != 1.0) or (nodeScaleLocal.z != 1.0):
         nodeSpecs[SCENE_NODE_SCALE_KEY] = Point3FStringBuild((nodeScaleLocal.x, nodeScaleLocal.y, nodeScaleLocal.z))

      # Save whether this node should initially be visible.
      if not node.duck_visible_specific:
         nodeSpecs["VisibleSpecific"] = False

      # Save whether this node's geometry should be included by default for
      # collision tests.
      if not node.duck_collidable_specific:
         nodeSpecs["CollidableSpecific"] = False

      # Save the billboard mode, if any.
      billboardMode = node.duck_billboard_mode
      if billboardMode != "Disabled":
         nodeSpecs["BillboardMode"] = billboardMode

      # Save the depth sort priority if needed.
      depthSortPriority = node.duck_depth_sort_priority
      if depthSortPriority != 0.0:
         nodeSpecs["DepthSortPriority"] = depthSortPriority

      # Save the bounding volume scale if needed.
      duckBoundingVolumeScale = node.duck_bounding_volume_scale
      if duckBoundingVolumeScale != 1.0:
         nodeSpecs["BoundingVolumeScale"] = duckBoundingVolumeScale

      # If this node is an LOD group, set the relevant parameters.
      if node.duck_lod_group:
         nodeSpecs["LODGroup"] = True
         duckLODGroupDrawDistanceMax = node.duck_lod_group_draw_distance_max
         if duckLODGroupDrawDistanceMax > 0.0:
            nodeSpecs["LODGroupDrawDistanceMax"] = duckLODGroupDrawDistanceMax

      # If this node is part of an LOD group, set the parameters for being a
      # member of the group.
      if node.parent and node.parent.duck_lod_group:
         duckLODLevelDrawDistanceMin = node.duck_lod_level_draw_distance_min
         if duckLODLevelDrawDistanceMin > 0.0:
            nodeSpecs["LODLevelDrawDistanceMin"] = duckLODLevelDrawDistanceMin

      # If no type has yet been specified, use the default.
      if not SCENE_NODE_TYPE_KEY in nodeSpecs:
         nodeSpecs[SCENE_NODE_TYPE_KEY] = SCENE_NODE_TYPE_NAME

      # Recurse to the descendants if needed.
      if len(node.children) > 0:
         if not SCENE_NODE_CHILDREN_KEY in nodeSpecs:
            nodeSpecs[SCENE_NODE_CHILDREN_KEY] = []
         nodeChildrenSpecs = nodeSpecs[SCENE_NODE_CHILDREN_KEY]
         for childNode in node.children:
            self.GatherNode(childNode, fileData, nodeChildrenSpecs, nodesGathered, poseBonesGathered, shapeKeysGathered, submeshExportAsMorphTargetsSamplers)

   #----------------------------------------------------------------------------

   # Gather mesh data for the given node.
   def GatherNodeMesh(self, node, fileData, nodeSpecs, shapeKeysGathered, submeshExportAsMorphTargetsSamplers):
      exportAsMorphTargetAnimation = node.duck_object_bake_type == DUCK_OBJECT_BAKE_TYPE_MORPH_TARGETS_NAME
      originalNode = node
      shouldClearMesh = False
      meshName = node.data.name
      nodeSpecs[SCENE_NODE_TYPE_KEY] = "Mesh"
      nodeSpecs["Mesh"] = meshName
      sourceMeshData = node.data

      # If necessary, convert the object to a mesh with all modifiers applied.
      if exportAsMorphTargetAnimation:
         depsgraph = bpy.context.evaluated_depsgraph_get()
         depsgraph.update()
         node = node.evaluated_get(depsgraph)
         sourceMeshData = node.to_mesh(preserve_all_data_layers=True, depsgraph=depsgraph)
         shouldClearMesh = True

      # Determine the number of materials used by different parts of this mesh.
      materialIndices = []
      for face in sourceMeshData.polygons:
         if not face.material_index in materialIndices:
            materialIndices.append(face.material_index)
      if len(materialIndices) < 1:
         if shouldClearMesh:
            node.to_mesh_clear()
         return
      if len(sourceMeshData.materials) < 1:
         if shouldClearMesh:
            node.to_mesh_clear()
         return

      # If the main collection of mesh data for the file does not exist yet,
      # create it now.
      if not MESHES_KEY in fileData:
         fileData[MESHES_KEY] = {}
      meshesSpecs = fileData[MESHES_KEY]

      # If the mesh data for this node has not yet been added to the main
      # collection of mesh data, add it now.
      if not meshName in meshesSpecs:
         meshesSpecs[meshName] = {}
      meshSpecs = meshesSpecs[meshName]
      submeshesSpecs = meshSpecs["Submeshes"] = []

      # For each material used, one or more submeshes will be needed.  Gather
      # the materials now.
      includeTangents = False
      includeSmoothNormals = False
      if not MATERIALS_KEY in fileData:
         fileData[MATERIALS_KEY] = {}
      materialsSpecs = fileData[MATERIALS_KEY]
      for materialIndex in materialIndices:
         material = sourceMeshData.materials[materialIndex]
         self.GatherMaterial(material, fileData)
         # At the same time, determine whether additional details, like tangents
         # or smooth normals, are needed by the relevant materials.
         if material.name in materialsSpecs:
            materialSpecs = materialsSpecs[material.name]
            if MATERIAL_NORMAL_TEXTURE_KEY in materialSpecs:
               includeTangents = True
            if MATERIAL_OUTLINE_DRAW_KEY in materialSpecs:
               includeSmoothNormals = True
            if MATERIAL_WIND_APPLY_KEY in materialSpecs:
               includeSmoothNormals = True

      # Calculate information needed for exporting.
      sourceMeshData.calc_loop_triangles()
      if includeTangents:
         try:
            sourceMeshData.calc_tangents()
         except:
            print("WARNING: calc_tangents failed for mesh " + meshName)
            includeTangents = False
      if not includeTangents:
         sourceMeshData.calc_normals_split()

      # See if this mesh has an armature modifier.
      armatureModifier = None
      for modifier in node.modifiers:
         if modifier.type == "ARMATURE":
            armatureModifier = modifier
            break

      # Begin tracking the bounding volume.
      boundingVolumeHelper = BoundingVoumeHelper()
      boundingVolumeHelper.Set(node)

      # Create the submeshes.
      vertexTree = None
      indices = None
      for materialIndex in materialIndices:
         material = sourceMeshData.materials[materialIndex]
         if not material:
            continue
         for triangle in sourceMeshData.loop_triangles:
            if triangle.material_index != materialIndex:
               continue

            # Start a new submesh if needed.
            if (not vertexTree) or (len(vertexTree.vertices) + 3 > VERTEX_BUFFER_VERTEX_COUNT_MAX):
               # Finish the old submesh if needed.
               if vertexTree:
                  self.SubmeshFinish(originalNode, node, sourceMeshData, meshSpecs, submeshesSpecs, \
                     vertexTree, indices, material, boundingVolumeHelper, armatureModifier, includeTangents, includeSmoothNormals, \
                     exportAsMorphTargetAnimation, submeshExportAsMorphTargetsSamplers)
               vertexTree = VertexTree()
               indices = []

            # Process the individual vertices of the triangle.
            for triangleVertexIndex in range(3):
               originalVertexIndex = triangle.vertices[triangleVertexIndex]
               loopIndex = triangle.loops[triangleVertexIndex]
               # Build an object for the current vertex.
               vertex = Vertex()
               vertex.originalIndex = originalVertexIndex
               vertex.originalLoopIndex = loopIndex
               vertex.position = sourceMeshData.vertices[originalVertexIndex].co.copy()
               vertex.normal = sourceMeshData.loops[loopIndex].normal.copy()
               if includeTangents:
                  vertex.tangent = sourceMeshData.loops[loopIndex].tangent.copy()
               else:
                  vertex.tangent = Vector((0.0, 0.0, 0.0))
               if includeSmoothNormals:
                  vertex.normalSmooth = sourceMeshData.vertices[originalVertexIndex].normal.copy()
               else:
                  vertex.normalSmooth = Vector((0.0, 0.0, 0.0))
               # Add information for all available UVMaps.  Flip the 
               # UV coordinates vertically for Frog's texture coordinate
               # conventions.
               for uvLayer in sourceMeshData.uv_layers:
                  originalUV = uvLayer.data[loopIndex].uv
                  vertex.texCoords[uvLayer.name] = Vector((originalUV.x, 1.0 - originalUV.y))
               # Add information for all available vertex color layers.
               for colorLayer in sourceMeshData.vertex_colors:
                  color = colorLayer.data[loopIndex].color
                  vertex.colors[colorLayer.name] = [color[0], color[1], color[2], color[3]]
               # Add the vertex and index to the collections.
               indices.append(vertexTree.VertexIndexGet(vertex))
         self.SubmeshFinish(originalNode, node, sourceMeshData, meshSpecs, submeshesSpecs, vertexTree, \
            indices, material, boundingVolumeHelper, armatureModifier, includeTangents, includeSmoothNormals, \
            exportAsMorphTargetAnimation, submeshExportAsMorphTargetsSamplers)
         vertexTree = None
         indices = None

      # If any of the submeshes have bone influence data, add data that would be
      # needed for skinning and shared by all submeshes.
      skinningNeeded = False
      for submeshSpecs in submeshesSpecs:
         if MESH_SUBMESH_BONES_KEY in submeshSpecs:
            skinningNeeded = True
            break
      if skinningNeeded and armatureModifier:
         # Remember the root of the skeleton.
         meshSpecs["Skeleton"] = armatureModifier.object.name

         # Gather the inverse bind matrices relative to this mesh.
         # Get a matrix for going from armature space to mesh space.
         armatureToMeshMatrix = node.matrix_world.inverted_safe() @ armatureModifier.object.matrix_world
         bonesSpecs = meshSpecs["Bones"] = []
         for submeshSpecs in submeshesSpecs:
            if MESH_SUBMESH_BONES_KEY in submeshSpecs:
               submeshBonesInfuencesSpecs = submeshSpecs[MESH_SUBMESH_BONES_KEY]
               for submeshBoneInfuencesSpecs in submeshBonesInfuencesSpecs:
                  # Make sure this bone hasn't already been processed.
                  boneName = submeshBoneInfuencesSpecs[MESH_SUBMESH_BONE_NAME_KEY]
                  boneAlreadyProcessed = False
                  for _boneSpecs in bonesSpecs:
                     if _boneSpecs[MESH_BONE_NAME_KEY] == boneName:
                        boneAlreadyProcessed = True
                        break
                  if boneAlreadyProcessed:
                     continue

                  # Get the bone.
                  bone = armatureModifier.object.data.bones[boneName]
                  if not bone:
                     continue

                  # Start the specs for this bone.
                  boneSpecs = {}
                  bonesSpecs.append(boneSpecs)
                  boneSpecs[MESH_BONE_NAME_KEY] = boneName

                  # Compute the inverse bind matrix from this.
                  inverseBindMatrix = (armatureToMeshMatrix @ bone.matrix_local).inverted_safe()
                  inverseBindMatrixSpecs = boneSpecs["InverseBindMatrix"] = []
                  inverseBindMatrixSpecs.append(Point3FStringBuild((inverseBindMatrix[0][0], inverseBindMatrix[1][0], inverseBindMatrix[2][0])))
                  inverseBindMatrixSpecs.append(Point3FStringBuild((inverseBindMatrix[0][1], inverseBindMatrix[1][1], inverseBindMatrix[2][1])))
                  inverseBindMatrixSpecs.append(Point3FStringBuild((inverseBindMatrix[0][2], inverseBindMatrix[1][2], inverseBindMatrix[2][2])))
                  inverseBindMatrixSpecs.append(Point3FStringBuild((inverseBindMatrix[0][3], inverseBindMatrix[1][3], inverseBindMatrix[2][3])))

      # Add any shape keys to the collection so we can gather their animation
      # data.
      if (not exportAsMorphTargetAnimation) and sourceMeshData.shape_keys and len(sourceMeshData.shape_keys.key_blocks):
         for shapeKeyIndex, shapeKey in enumerate(sourceMeshData.shape_keys.key_blocks):
            # Skip the reference key.
            if shapeKey == sourceMeshData.shape_keys.reference_key:
               continue
            # Remember the shape key and the node to which it belongs.
            shapeKeysGathered.append((node, shapeKey))

      # Save the bounding volume.
      boundingVolumeHelper.OutputBoundingVolumeSpecs(meshSpecs)

   #----------------------------------------------------------------------------

   # Helper function for completing a submesh.
   def SubmeshFinish(self, originalNode, node, sourceMeshData, meshSpecs, submeshesSpecs, vertexTree, \
      indices, material, boundingVolumeHelper, armatureModifier, includeTangents, includeSmoothNormals, \
      exportAsMorphTargetAnimation, submeshExportAsMorphTargetsSamplers):

      if len(indices) <= 0:
         return

      # Start the specs for the new submesh.
      submeshSpecs = { "Material": material.name }
      submeshesSpecs.append(submeshSpecs)
      vertexAttributesSpecs = submeshSpecs["VertexAttributes"] = []

      # Build the index buffer.
      indexBuffersSpecs = submeshSpecs["IndexBuffers"] = []
      indexBufferSpecs = {}
      indexBuffersSpecs.append(indexBufferSpecs)
      bufferDataBuilder = BytesIO()
      for index in indices:
         bufferDataBuilder.write(struct.pack("<H", index))
      indexBufferSpecs[BUFFER_DATA_KEY] = bufferDataBuilder.getvalue()
      indexBufferSpecs["PrimitiveType"] = "Triangle"
      indexBufferSpecs["ComponentType"] = "uint16"
      indexBufferSpecs["IndexCount"] = len(indices)

      # Build the vertex buffers.
      vertexBuffersSpecs = submeshSpecs["VertexBuffers"] = []

      # Build a vertex buffer for position and normal data.
      bufferDataBuilder = BytesIO()
      for vertex in vertexTree.vertices:
         bufferDataBuilder.write(struct.pack("<fff", vertex.position.x, vertex.position.y, vertex.position.z))
         bufferDataBuilder.write(struct.pack("<fff", vertex.normal.x, vertex.normal.y, vertex.normal.z))
      vertexBufferSpecs = {}
      vertexBuffersSpecs.append(vertexBufferSpecs)
      vertexBufferSpecs[BUFFER_DATA_KEY] = bufferDataBuilder.getvalue()

      # Add vertex attributes for position and normal.
      vertexAttributesSpecs.append({
         VERTEX_ATTRIBUTE_SEMANTIC_KEY: "Position",
         VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY: 0,
         VERTEX_ATTRIBUTE_OFFSET_KEY: 0,
         VERTEX_ATTRIBUTE_STRIDE_KEY: 24,
         VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY: COMPONENT_TYPE_FLOAT32,
         VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY: 3
      })
      vertexAttributesSpecs.append({
         VERTEX_ATTRIBUTE_SEMANTIC_KEY: "Normal",
         VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY: 0,
         VERTEX_ATTRIBUTE_OFFSET_KEY: 12,
         VERTEX_ATTRIBUTE_STRIDE_KEY: 24,
         VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY: COMPONENT_TYPE_FLOAT32,
         VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY: 3
      })

      # Add vertex buffer and attribute for tangents.
      vertexBufferIndex = 1
      if includeTangents:
         bufferDataBuilder = BytesIO()
         for vertex in vertexTree.vertices:
            bufferDataBuilder.write(struct.pack("<fff", vertex.tangent.x, vertex.tangent.y, vertex.tangent.z))
         vertexBufferSpecs = {}
         vertexBuffersSpecs.append(vertexBufferSpecs)
         vertexBufferSpecs[BUFFER_DATA_KEY] = bufferDataBuilder.getvalue()
         vertexAttributesSpecs.append({
            VERTEX_ATTRIBUTE_SEMANTIC_KEY: "Tangent",
            VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY: vertexBufferIndex,
            VERTEX_ATTRIBUTE_OFFSET_KEY: 0,
            VERTEX_ATTRIBUTE_STRIDE_KEY: 12,
            VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY: COMPONENT_TYPE_FLOAT32,
            VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY: 3
         })
         vertexBufferIndex += 1

      # Add vertex buffer and attribute for smooth normals.
      if includeSmoothNormals:
         bufferDataBuilder = BytesIO()
         for vertex in vertexTree.vertices:
            bufferDataBuilder.write(struct.pack("<fff", vertex.normalSmooth.x, vertex.normalSmooth.y, vertex.normalSmooth.z))
         vertexBufferSpecs = {}
         vertexBuffersSpecs.append(vertexBufferSpecs)
         vertexBufferSpecs[BUFFER_DATA_KEY] = bufferDataBuilder.getvalue()
         vertexAttributesSpecs.append({
            VERTEX_ATTRIBUTE_NAME_KEY: "Smooth",
            VERTEX_ATTRIBUTE_SEMANTIC_KEY: "Normal",
            VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY: vertexBufferIndex,
            VERTEX_ATTRIBUTE_OFFSET_KEY: 0,
            VERTEX_ATTRIBUTE_STRIDE_KEY: 12,
            VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY: COMPONENT_TYPE_FLOAT32,
            VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY: 3
         })
         vertexBufferIndex += 1

      # Add vertex buffers and attributes for texture coordinates.
      texCoordSemanticIndex = 0
      for uvMapName, uvMap in sorted(sourceMeshData.uv_layers.items()):
         # Add the vertex attribute
         vertexAttributesSpecs.append({
            VERTEX_ATTRIBUTE_NAME_KEY: uvMapName,
            VERTEX_ATTRIBUTE_SEMANTIC_KEY: "TexCoord",
            VERTEX_ATTRIBUTE_SEMANTIC_INDEX_KEY: texCoordSemanticIndex,
            VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY: vertexBufferIndex,
            VERTEX_ATTRIBUTE_OFFSET_KEY: 0,
            VERTEX_ATTRIBUTE_STRIDE_KEY: 8,
            VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY: COMPONENT_TYPE_FLOAT32,
            VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY: 2
         })

         # Add the vertex buffer.
         bufferDataBuilder = BytesIO()
         for vertex in vertexTree.vertices:
            texCoord = vertex.texCoords[uvMapName]
            bufferDataBuilder.write(struct.pack("<ff", texCoord.x, texCoord.y))
         vertexBufferSpecs = {}
         vertexBuffersSpecs.append(vertexBufferSpecs)
         vertexBufferSpecs[BUFFER_DATA_KEY] = bufferDataBuilder.getvalue()

         texCoordSemanticIndex += 1
         vertexBufferIndex += 1

      # Add vertex buffers and attributes for vertex colors.
      colorSemanticIndex = 0
      for colorLayerName, colorLayer in sorted(sourceMeshData.vertex_colors.items()):
         # Add the vertex attribute
         vertexAttributesSpecs.append({
            VERTEX_ATTRIBUTE_NAME_KEY: colorLayerName,
            VERTEX_ATTRIBUTE_SEMANTIC_KEY: "Color",
            VERTEX_ATTRIBUTE_SEMANTIC_INDEX_KEY: colorSemanticIndex,
            VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY: vertexBufferIndex,
            VERTEX_ATTRIBUTE_OFFSET_KEY: 0,
            VERTEX_ATTRIBUTE_STRIDE_KEY: 4,
            VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY: COMPONENT_TYPE_UINT8,
            VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY: 4,
            VERTEX_ATTRIBUTE_NORMALIZED_KEY: True
         })

         # Add the vertex buffer.
         bufferDataBuilder = BytesIO()
         for vertex in vertexTree.vertices:
            color = vertex.colors[colorLayerName]
            bufferDataBuilder.write(struct.pack("<BBBB",
               int(max(0.0, min(255.0, color[0] * 255.0))),
               int(max(0.0, min(255.0, color[1] * 255.0))),
               int(max(0.0, min(255.0, color[2] * 255.0))),
               int(max(0.0, min(255.0, color[3] * 255.0)))))
         vertexBufferSpecs = {}
         vertexBuffersSpecs.append(vertexBufferSpecs)
         vertexBufferSpecs[BUFFER_DATA_KEY] = bufferDataBuilder.getvalue()

         texCoordSemanticIndex += 1
         vertexBufferIndex += 1

      # If an armature modifier is involved, add skin information.
      if (not exportAsMorphTargetAnimation) and armatureModifier:
         submeshBonesSpecs = None
         armature = armatureModifier.object
         if armature:
            for poseBone in armature.pose.bones:
               # Get the vertex group with the weights for this bone.
               if not poseBone.name in node.vertex_groups:
                  continue
               vertexGroup = node.vertex_groups[poseBone.name]
               if not vertexGroup:
                  continue

               # Build an temporary array of objects that specify the influence
               # of this bone on individual vertices.
               bone = poseBone.bone
               boneVertexInfluencesTemp = []
               vertexInfluenceCount = 0
               for vertex in vertexTree.vertices:
                  originalVertex = sourceMeshData.vertices[vertex.originalIndex]
                  for groupElement in originalVertex.groups:
                     if groupElement.group != vertexGroup.index:
                        continue
                     boneWeight = groupElement.weight
                     if boneWeight != 0.0:
                        vertexInfluenceCount += 1
                        boneVertexInfluencesTemp.append([vertex.index, boneWeight])
                     break

               # If the bone doesn't affect any vertices in this submesh, skip
               # to the next bone.
               if vertexInfluenceCount == 0:
                  continue

               # If needed, create the main collection of bone influences for
               # this submesh.
               if not submeshBonesSpecs:
                  submeshBonesSpecs = submeshSpecs[MESH_SUBMESH_BONES_KEY] = []

               # Create an entry in the specs for the influence of this bone.
               submeshBoneSpecs = {}
               submeshBonesSpecs.append(submeshBoneSpecs)
               submeshBoneSpecs[MESH_SUBMESH_BONE_NAME_KEY] = poseBone.name
               submeshBoneSpecs[MESH_SUBMESH_BONE_VERTEX_INFLUENCES_TEMP_KEY] = boneVertexInfluencesTemp

            vertexCount = len(vertexTree.vertices)
            if submeshBonesSpecs and vertexCount:
               # The runtime assumes that the weights which influence each
               # vertex are normalized, but Blender doesn't necessarily do this,
               # so normalize the temporary collection of influences.
               thisVertexInfluences = []
               for vertexIndex in range(vertexCount):
                  # Get the sum of the weights for this vertex across all bones.
                  vertexWeightTotal = 0.0
                  thisVertexInfluences.clear()
                  for submeshBoneSpecs in submeshBonesSpecs:
                     boneVertexInfluencesTemp = submeshBoneSpecs[MESH_SUBMESH_BONE_VERTEX_INFLUENCES_TEMP_KEY]
                     for boneVertexInfluenceTemp in boneVertexInfluencesTemp:
                        if boneVertexInfluenceTemp[0] == vertexIndex:
                           vertexWeightTotal += boneVertexInfluenceTemp[1]
                           thisVertexInfluences.append(boneVertexInfluenceTemp)
                           break

                  # Normalize the influences which were found for this vertex.
                  if vertexWeightTotal:
                     for boneVertexInfluenceTemp in thisVertexInfluences:
                        boneVertexInfluenceTemp[1] /= vertexWeightTotal

               # Replace the temporary collection of vertex weights with a
               # binary format now that they've been normalized.
               for submeshBoneSpecs in submeshBonesSpecs:
                  boneVertexInfluencesBuilder = BytesIO()
                  boneVertexInfluencesTemp = submeshBoneSpecs[MESH_SUBMESH_BONE_VERTEX_INFLUENCES_TEMP_KEY]
                  del submeshBoneSpecs[MESH_SUBMESH_BONE_VERTEX_INFLUENCES_TEMP_KEY]
                  for boneVertexInfluenceTemp in boneVertexInfluencesTemp:
                     boneVertexInfluencesBuilder.write(struct.pack("<Lf", boneVertexInfluenceTemp[0], boneVertexInfluenceTemp[1]))
                  submeshBoneSpecs["VertexInfluences"] = boneVertexInfluencesBuilder.getvalue()

      # Export morph targets, if available.
      if (not exportAsMorphTargetAnimation) and sourceMeshData.shape_keys and len(sourceMeshData.shape_keys.key_blocks):
         # Back up settings that will be changed.
         restoreArmatureModifier = armatureModifier and armatureModifier.show_viewport
         restoreShowOnlyShapeKey = node.show_only_shape_key
         retoreActiveShapeKeyIndex = node.active_shape_key_index

         # If an armature is involved, temporarily disable it.
         if armatureModifier:
            armatureModifier.show_viewport = False

         # Set up the node to show one shape key at a time, and at full influence.
         node.show_only_shape_key = True

         # Process the shape keys.
         expectedVertexCount = len(sourceMeshData.vertices)
         for shapeKeyIndex, shapeKey in enumerate(sourceMeshData.shape_keys.key_blocks):
            # Skip the reference key.
            if shapeKey == sourceMeshData.shape_keys.reference_key:
               continue

            # Prepare a temporary mesh with the current shape key at full
            # influence.
            node.active_shape_key_index = shapeKeyIndex
            self.SubmeshFinishMorphTargetHelper(originalNode, vertexTree, boundingVolumeHelper, includeTangents, includeSmoothNormals, submeshSpecs, expectedVertexCount, shapeKey.name)

         # Restore settings that were changed temporarily.
         if armatureModifier and restoreArmatureModifier:
            armatureModifier.show_viewport = True
         node.show_only_shape_key = restoreShowOnlyShapeKey
         node.active_shape_key_index = retoreActiveShapeKeyIndex

      # If exporting a baked series of morph targets, prepare a sampler for this
      # submesh now.
      if exportAsMorphTargetAnimation:
         expectedVertexCount = len(sourceMeshData.vertices)
         submeshExportAsMorphTargetsSamplers.append(AnimationSamplerSubmeshExportAsMorphTargets(self, originalNode, vertexTree, boundingVolumeHelper, includeTangents, includeSmoothNormals, meshSpecs, submeshSpecs, expectedVertexCount))

      # Export wind deformation information if provided.
      vertexGroupWindLeanIndex = -1
      vertexGroupWindShakeIndex = -1
      vertexGroupWindShakeDetailIndex = -1
      vertexGroupWindShakePhaseIndex = -1
      if VERTEX_GROUP_WIND_LEAN_NAME in node.vertex_groups:
         vertexGroupWindLeanIndex = node.vertex_groups[VERTEX_GROUP_WIND_LEAN_NAME].index
      if VERTEX_GROUP_WIND_SHAKE_NAME in node.vertex_groups:
         vertexGroupWindShakeIndex = node.vertex_groups[VERTEX_GROUP_WIND_SHAKE_NAME].index
      if VERTEX_GROUP_WIND_SHAKE_DETAIL_NAME in node.vertex_groups:
         vertexGroupWindShakeDetailIndex = node.vertex_groups[VERTEX_GROUP_WIND_SHAKE_DETAIL_NAME].index
      if VERTEX_GROUP_WIND_SHAKE_PHASE_NAME in node.vertex_groups:
         vertexGroupWindShakePhaseIndex = node.vertex_groups[VERTEX_GROUP_WIND_SHAKE_PHASE_NAME].index
      includeWindAttribute = (vertexGroupWindLeanIndex != -1) or (vertexGroupWindShakeIndex != -1) or (vertexGroupWindShakeDetailIndex != -1) or (vertexGroupWindShakePhaseIndex != -1)
      if VERTEX_ATTRIBUTE_WIND_NAME in sourceMeshData.vertex_colors:
         includeWindAttribute = False
      if includeWindAttribute:
         # Add the vertex attribute
         vertexAttributesSpecs.append({
            VERTEX_ATTRIBUTE_NAME_KEY: VERTEX_ATTRIBUTE_WIND_NAME,
            VERTEX_ATTRIBUTE_SEMANTIC_KEY: "Color",
            VERTEX_ATTRIBUTE_VERTEX_BUFFER_INDEX_KEY: vertexBufferIndex,
            VERTEX_ATTRIBUTE_OFFSET_KEY: 0,
            VERTEX_ATTRIBUTE_STRIDE_KEY: 4,
            VERTEX_ATTRIBUTE_COMPONENT_TYPE_KEY: COMPONENT_TYPE_UINT8,
            VERTEX_ATTRIBUTE_COMPONENT_COUNT_KEY: 4,
            VERTEX_ATTRIBUTE_NORMALIZED_KEY: True
         })

         # Add the vertex buffer.
         bufferDataBuilder = BytesIO()
         for vertex in vertexTree.vertices:
            # For each vertex, try go get a value for each property from the
            # appropriate vertex group.
            leanValue = 0.0
            shakeValue = 0.0
            shakeDetailValue = 0.0
            shakePhaseValue = 0.0
            originalVertex = sourceMeshData.vertices[vertex.originalIndex]
            for groupElement in originalVertex.groups:
               if groupElement.group == vertexGroupWindLeanIndex:
                  leanValue = groupElement.weight
               elif groupElement.group == vertexGroupWindShakeIndex:
                  shakeValue = groupElement.weight
               elif groupElement.group == vertexGroupWindShakeDetailIndex:
                  shakeDetailValue = groupElement.weight
               elif groupElement.group == vertexGroupWindShakePhaseIndex:
                  shakePhaseValue = groupElement.weight
            bufferDataBuilder.write(struct.pack("<BBBB",
               int(max(0.0, min(255.0, leanValue * 255.0))),
               int(max(0.0, min(255.0, shakeValue * 255.0))),
               int(max(0.0, min(255.0, shakeDetailValue * 255.0))),
               int(max(0.0, min(255.0, shakePhaseValue * 255.0)))))
         vertexBufferSpecs = {}
         vertexBuffersSpecs.append(vertexBufferSpecs)
         vertexBufferSpecs[BUFFER_DATA_KEY] = bufferDataBuilder.getvalue()

         vertexBufferIndex += 1

#----------------------------------------------------------------------------

   # Helper function for gathering information about a shape key for a submesh.
   # Return True if an output morph target was created.
   def SubmeshFinishMorphTargetHelper(self, node, vertexTree, boundingVolumeHelper, includeTangents, includeSmoothNormals, submeshSpecs, expectedVertexCount, outputMorphTargetName):
      depsgraph = bpy.context.evaluated_depsgraph_get()
      depsgraph.update()
      tempNode = node.evaluated_get(depsgraph)
      tempMesh = tempNode.to_mesh(preserve_all_data_layers=True, depsgraph=depsgraph)
      if len(tempMesh.vertices) != expectedVertexCount:
         raise Exception("ERROR -- Unable to export a morph target for \"%s\", because the number of vertices changed." % (node.name))
      tempIncludeTangents = includeTangents
      tempMesh.calc_loop_triangles()
      if tempIncludeTangents:
         try:
            tempMesh.calc_tangents()
         except:
            tempIncludeTangents = False
      if not tempIncludeTangents:
         tempMesh.calc_normals_split()

      # Update the bounding volume based on this morph target.
      boundingVolumeHelper.Union(tempNode)

      # Build binary buffers for the differences in the relevant vertex attributes.
      positionChangeBuilder = BytesIO()
      normalChangeBuilder = BytesIO()
      normalSmoothChangeBuilder = BytesIO()
      tangentChangeBuilder = BytesIO()
      for vertexIndex, vertex in enumerate(vertexTree.vertices):
         positionChange = tempMesh.vertices[vertex.originalIndex].co - vertex.position
         if positionChange.x or positionChange.y or positionChange.z:
            positionChangeBuilder.write(struct.pack("<Lfff", vertexIndex, positionChange.x, positionChange.y, positionChange.z))
         normalChange = tempMesh.loops[vertex.originalLoopIndex].normal - vertex.normal
         if normalChange.x or normalChange.y or normalChange.z:
            normalChangeBuilder.write(struct.pack("<Lfff", vertexIndex, normalChange.x, normalChange.y, normalChange.z))
         if includeSmoothNormals:
            normalSmoothChange = tempMesh.vertices[vertex.originalIndex].normal - vertex.normalSmooth
            if normalSmoothChange.x or normalSmoothChange.y or normalSmoothChange.z:
               normalSmoothChangeBuilder.write(struct.pack("<Lfff", vertexIndex, normalSmoothChange.x, normalSmoothChange.y, normalSmoothChange.z))
         if tempIncludeTangents:
            tangentChange = tempMesh.loops[vertex.originalLoopIndex].tangent - vertex.tangent
            if tangentChange.x or tangentChange.y or tangentChange.z:
               tangentChangeBuilder.write(struct.pack("<Lfff", vertexIndex, tangentChange.x, tangentChange.y, tangentChange.z))

      # See if there were any actual changes for this morph target.
      positionChangeBytes = positionChangeBuilder.getvalue()
      normalChangeBytes = normalChangeBuilder.getvalue()
      normalSmoothChangeBytes = normalSmoothChangeBuilder.getvalue()
      tangentChangeBytes = tangentChangeBuilder.getvalue()
      if (len(positionChangeBytes) == 0) and (len(normalChangeBytes) == 0) and (len(normalSmoothChangeBytes) == 0) and (len(tangentChangeBytes) == 0):
         tempNode.to_mesh_clear()
         return False

      # If needed, create the main collection of morph targets for this
      # submesh.
      submeshMorphTargetsSpecs = None
      if not MESH_SUBMESH_MORPH_TARGETS_KEY in submeshSpecs:
         submeshSpecs[MESH_SUBMESH_MORPH_TARGETS_KEY] = []
      submeshMorphTargetsSpecs = submeshSpecs[MESH_SUBMESH_MORPH_TARGETS_KEY]

      # Create an entry in the specs for this morph target.
      submeshMorphTargetSpecs = {}
      submeshMorphTargetsSpecs.append(submeshMorphTargetSpecs)
      submeshMorphTargetSpecs["Name"] = outputMorphTargetName
      if len(positionChangeBytes):
         submeshMorphTargetSpecs["Position"] = positionChangeBytes
      if len(normalChangeBytes):
         submeshMorphTargetSpecs["Normal"] = normalChangeBytes
      if len(normalSmoothChangeBytes):
         submeshMorphTargetSpecs["NormalSmooth"] = normalSmoothChangeBytes
      if len(tangentChangeBytes):
         submeshMorphTargetSpecs["Tangent"] = tangentChangeBytes

      tempNode.to_mesh_clear()
      return True

#----------------------------------------------------------------------------

   # Gather data from the given material.
   def GatherMaterial(self, material, fileData):
      # If the main collection of material data for the file does not exist yet,
      # create it now.
      if not MATERIALS_KEY in fileData:
         fileData[MATERIALS_KEY] = {}
      materialsSpecs = fileData[MATERIALS_KEY]

      # See if the given material has already been processed.
      if material.name in materialsSpecs:
         return
      materialSpecs = materialsSpecs[material.name] = {}

      # Find the active material output node.
      if not material.use_nodes:
         return
      for node in material.node_tree.nodes:
         if (node.type == "OUTPUT_MATERIAL") and node.is_active_output:
            activeOutputNode = node
            break
      if not activeOutputNode:
         return

      # Get the shader node connected to the output node, and make sure it is of
      # the expected type (Principled BSDF).
      if(len(activeOutputNode.inputs['Surface'].links) < 1):
         return
      shaderNode = activeOutputNode.inputs['Surface'].links[0].from_node
      if (not shaderNode) or (shaderNode.type != "BSDF_PRINCIPLED"):
         return

      # Specify the material type.
      materialSpecs["Type"] = "Standard"

      # Prepare to gather properties.
      pbrSpecs = materialSpecs["PBRMetallicRoughness"] = {}

      # If the material is two-sided, note this.
      if material.duck_two_sided:
         materialSpecs["TwoSided"] = True

      # If the normals of back faces should not be flipped, note this.
      if not material.duck_back_face_normals_flip:
         materialSpecs["BackFaceNormalsFlip"] = False

      # If the material should include an outline, note this.
      if material.duck_outline_draw:
         materialSpecs[MATERIAL_OUTLINE_DRAW_KEY] = True

      # See if there is a node for miscellaneous Duck settings.
      duckSettingsNode = None
      if "DuckSettings" in material.node_tree.nodes:
         duckSettingsNode = material.node_tree.nodes["DuckSettings"]

      # Try to find an input of baked lighting.
      lightmapInput = None
      if duckSettingsNode and ("Lightmap" in duckSettingsNode.inputs):
         lightmapInput = duckSettingsNode.inputs["Lightmap"]

      # Try to find an input of baked ambient occlusion.
      occlusionInput = None
      if duckSettingsNode and ("Occlusion" in duckSettingsNode.inputs):
         occlusionInput = duckSettingsNode.inputs["Occlusion"]

      # See if there is a texture for the base color.
      baseColorInput = shaderNode.inputs["Base Color"]
      if len(baseColorInput.links) == 1:
         baseColorTextureNode = baseColorInput.links[0].from_node
         self.GatherTexture(baseColorTextureNode, pbrSpecs, MATERIAL_BASE_COLOR_TEXTURE_KEY)

      # If no texture was found, use a solid color.
      alphaInput = shaderNode.inputs["Alpha"]
      if not MATERIAL_BASE_COLOR_TEXTURE_KEY in pbrSpecs:
         baseColorDefaultValue = baseColorInput.default_value
         baseColor = (baseColorDefaultValue[0], baseColorDefaultValue[1], \
            baseColorDefaultValue[2], alphaInput.default_value)
         pbrSpecs["BaseColor"] = ColorRGBA8StringBuild(baseColor)

      # Get the metallic property.
      metallicInput = shaderNode.inputs["Metallic"]

      # Get the roughness property.
      roughnessInput = shaderNode.inputs["Roughness"]

      # Get the emission property.
      emissionInput = shaderNode.inputs["Emission"]

      # See if there is a lightmap.
      if lightmapInput and (len(lightmapInput.links) == 1) and lightmapInput.links[0] and \
         lightmapInput.links[0].from_node:
         lightmapNode = lightmapInput.links[0].from_node
         if (lightmapNode.type == "TEX_IMAGE"):
            self.GatherTexture(lightmapNode, materialSpecs, "LightmapTexture")

      # See if there is a normal map.
      normalMapInput = shaderNode.inputs["Normal"]
      if normalMapInput and (len(normalMapInput.links) == 1) and normalMapInput.links[0] and \
         normalMapInput.links[0].from_node:
         normalMapNode = normalMapInput.links[0].from_node
         if (normalMapNode.type == "NORMAL_MAP") and (len(normalMapNode.inputs["Color"].links) == 1) and \
            normalMapNode.inputs["Color"].links[0] and normalMapNode.inputs["Color"].links[0].from_node:
            normalMapColorNode = normalMapNode.inputs["Color"].links[0].from_node
            if normalMapColorNode.type == "TEX_IMAGE":
               self.GatherTexture(normalMapColorNode, materialSpecs, MATERIAL_NORMAL_TEXTURE_KEY)

      # See if there is a texture input for occlusion.
      occlusionImageNode = None
      if occlusionInput and (len(occlusionInput.links) == 1) and occlusionInput.links[0] and \
         occlusionInput.links[0].from_node:
         occlusionInputFromNode = occlusionInput.links[0].from_node
         if occlusionInputFromNode and occlusionInputFromNode.type == "SEPRGB" and \
            occlusionInputFromNode.inputs['Image']:
            separateRGBImageInput = occlusionInputFromNode.inputs['Image']
            if (len(separateRGBImageInput.links) == 1) and separateRGBImageInput.links[0] and \
               separateRGBImageInput.links[0].from_node and (separateRGBImageInput.links[0].from_node.type == "TEX_IMAGE"):
               occlusionImageNode = separateRGBImageInput.links[0].from_node

      # See if there is a texture input for metallic.
      metallicImageNode = None
      if metallicInput and (len(metallicInput.links) == 1) and metallicInput.links[0] and \
         metallicInput.links[0].from_node:
         metallicInputFromNode = metallicInput.links[0].from_node
         if metallicInputFromNode and metallicInputFromNode.type == "SEPRGB" and \
            metallicInputFromNode.inputs['Image']:
            separateRGBImageInput = metallicInputFromNode.inputs['Image']
            if (len(separateRGBImageInput.links) == 1) and separateRGBImageInput.links[0] and \
               separateRGBImageInput.links[0].from_node and (separateRGBImageInput.links[0].from_node.type == "TEX_IMAGE"):
               metallicImageNode = separateRGBImageInput.links[0].from_node

      # See if there is a texture input for roughness.
      roughnessImageNode = None
      if roughnessInput and (len(roughnessInput.links) == 1) and roughnessInput.links[0] and \
         roughnessInput.links[0].from_node:
         roughnessInputFromNode = roughnessInput.links[0].from_node
         if roughnessInputFromNode and roughnessInputFromNode.type == "SEPRGB" and \
            roughnessInputFromNode.inputs['Image']:
            separateRGBImageInput = roughnessInputFromNode.inputs['Image']
            if (len(separateRGBImageInput.links) == 1) and separateRGBImageInput.links[0] and \
               separateRGBImageInput.links[0].from_node and (separateRGBImageInput.links[0].from_node.type == "TEX_IMAGE"):
               roughnessImageNode = separateRGBImageInput.links[0].from_node

      # Handle the possiblity of a MetallicRoughnessOcclusion map.  Assume these
      # properties are coming from the red, green, and blue channels,
      # respectively, from the same texture through a "Separate RGB" node.
      if metallicImageNode and roughnessImageNode and occlusionImageNode and \
         (metallicImageNode == roughnessImageNode) and (roughnessImageNode == occlusionImageNode):
         self.GatherTexture(metallicImageNode, pbrSpecs, MATERIAL_METALLIC_ROUGHNESS_OCCLUSION_TEXTURE_KEY)

      # If metallic and roughness aren't given by a texture, export a constant.
      if not MATERIAL_METALLIC_ROUGHNESS_OCCLUSION_TEXTURE_KEY in pbrSpecs:
         pbrSpecs["Metallic"] = metallicInput.default_value
         pbrSpecs["Roughness"] = roughnessInput.default_value

      # See if there is an emission texture.
      if emissionInput and (len(emissionInput.links) == 1) and emissionInput.links[0] and \
         emissionInput.links[0].from_node:
         emissionNode = emissionInput.links[0].from_node
         if emissionNode.type == "TEX_IMAGE":
            self.GatherTexture(emissionNode, materialSpecs, MATERIAL_EMISSION_TEXTURE_KEY)

      # If emission is not given by a texture and isn't fully black, export a
      # constant.
      if not MATERIAL_EMISSION_TEXTURE_KEY in materialSpecs:
         if (emissionInput.default_value[0] != 0.0) or (emissionInput.default_value[1] != 0.0) or \
            (emissionInput.default_value[2] != 0.0):
            materialSpecs["EmissionColor"] = ColorRGBA8StringBuild(emissionInput.default_value)

      # Get the dielectric specular property.  Scale from the Blender range to a
      # directly usable fresnel0 value.
      dielectricSpecularInput = shaderNode.inputs["Specular"]
      pbrSpecs["DielectricSpecular"] = dielectricSpecularInput.default_value * 0.08

      # Get the blend method.  Assume the opacity would be coming from the alpha
      # channel of the base color.
      if (material.duck_alpha_mode == "Clip") or (material.duck_shadow_cast_mode == "AlphaClip"):
         materialSpecs["AlphaCutoff"] = material.alpha_threshold
      if material.duck_alpha_mode != "Opaque":
         materialSpecs["AlphaMode"] = material.duck_alpha_mode

      # Get the shadow cast mode.
      if material.duck_shadow_cast_mode != "Default":
         materialSpecs["ShadowCastMode"] = material.duck_shadow_cast_mode

      # Get the extent of additive blending.
      if material.duck_additive_blending != 0.0:
         materialSpecs["AdditiveBlending"] = material.duck_additive_blending

      # Get whether dynamic lighting should be performed.
      if material.duck_unlit:
         materialSpecs["Unlit"] = True

      # Get whether the object should appear to scale down with distance.
      if material.duck_distance_scale_down:
         materialSpecs["DistanceScaleDown"] = True
         materialSpecs["DistanceScaleDownNear"] = material.duck_distance_scale_down_near
         materialSpecs["DistanceScaleDownFar"] = material.duck_distance_scale_down_far

      # Get whether the normals should be treated as pointing upward.
      if material.duck_normals_up_local:
         materialSpecs["NormalsUpLocal"] = True

      # Include wind data if needed.
      if material.duck_wind_apply:
         materialSpecs[MATERIAL_WIND_APPLY_KEY] = True
         materialSpecs["WindLeanCoefficient"] = material.duck_wind_lean_coefficient
         materialSpecs["WindShakeCoefficient"] = material.duck_wind_shake_coefficient
         materialSpecs["WindShakeDetailCoefficient"] = material.duck_wind_shake_detail_coefficient

   #----------------------------------------------------------------------------

   # Gather data about a texture.
   def GatherTexture(self, textureNode, parentSpecs, textureKey):
      if textureNode and (textureNode.type == "TEX_IMAGE") and \
            textureNode.image and (len(textureNode.inputs["Vector"].links) == 1):
            # Get the image for the texture.
            textureImage = textureNode.image
            # Get the UV Map.
            uvMapNode = textureNode.inputs["Vector"].links[0].from_node
            if textureImage.filepath and uvMapNode and (uvMapNode.type == "UVMAP") and \
               uvMapNode.uv_map:
               # Get the filename, excluding both the parent and the extension.
               textureInstanceSpecs = parentSpecs[textureKey] = {
                  "Filename": os.path.splitext(os.path.split(textureImage.filepath)[1])[0],
                  "UVMap": uvMapNode.uv_map
               }
               # Get the filter/interpolation mode.
               if textureNode.interpolation == "Closest":
                  textureInstanceSpecs[MATERIAL_TEXTURE_FILTER_MODE_KEY] = "Point"
               elif textureNode.interpolation == "Cubic":
                  textureInstanceSpecs[MATERIAL_TEXTURE_FILTER_MODE_KEY] = "Trilinear"
               else:
                  textureInstanceSpecs[MATERIAL_TEXTURE_FILTER_MODE_KEY] = "Bilinear"
               # Get the texture wrap mode.
               if textureNode.extension == "EXTEND":
                  textureInstanceSpecs[MATERIAL_TEXTURE_WRAP_MODE_U_KEY] = textureInstanceSpecs[MATERIAL_TEXTURE_WRAP_MODE_V_KEY] = "Clamp"
               else:
                  textureInstanceSpecs[MATERIAL_TEXTURE_WRAP_MODE_U_KEY] = textureInstanceSpecs[MATERIAL_TEXTURE_WRAP_MODE_V_KEY] = "Repeat"

   #----------------------------------------------------------------------------

   # Gather light data for the given node.
   def GatherNodeLight(self, node, nodeSpecs):
      if node.data.type == "SUN":
         lightType = "Directional"
      elif node.data.type == "POINT":
         lightType = "Point"
      elif node.data.type == "SPOT":
         lightType = "Spot"
         nodeSpecs["LightConeAngleOuter"] = math.degrees(node.data.spot_size)
         nodeSpecs["LightConeAngleBlending"] = node.data.spot_blend
      else:
         return

      if (node.data.type == "POINT") or (node.data.type == "SPOT"):
         nodeSpecs["LightRange"] = node.data.cutoff_distance

      nodeSpecs[SCENE_NODE_TYPE_KEY] = "Light"
      nodeSpecs["LightType"] = lightType
      nodeSpecs["LightColor"] = ColorRGBA8StringBuild(node.data.color)
      nodeSpecs["LightIntensity"] = node.data.energy
      nodeSpecs["LightBakeType"] = node.data.duck_bake_type

   #----------------------------------------------------------------------------

   # Gather helper sphere data for the given node.
   def GatherNodeHelperSphere(self, node, nodeSpecs):
      nodeSpecs[SCENE_NODE_TYPE_KEY] = "HelperSphere"
      nodeSpecs["HelperSphereRadius"] = node.empty_display_size

   #----------------------------------------------------------------------------

   # Gather a skeleton from the given armature node.
   def GatherNodeArmature(self, node, nodeSpecs, poseBonesGathered):
      # Gather the bones of the armature, and make them children of this node.
      if len(node.data.bones) > 0:
         if not SCENE_NODE_CHILDREN_KEY in nodeSpecs:
            nodeSpecs[SCENE_NODE_CHILDREN_KEY] = []
         nodeChildrenSpecs = nodeSpecs[SCENE_NODE_CHILDREN_KEY]

         # For convenience, we'll use the pose.bones collection.  This includes
         # all nodes in the armature, so look out for ones with no assigned
         # parent to start the hierarchy.
         for descendantPoseBone in node.pose.bones:
            if descendantPoseBone.parent == None:
               self.GatherBone(descendantPoseBone, nodeChildrenSpecs, node, poseBonesGathered)

   #----------------------------------------------------------------------------

   def GatherBone(self, poseBone, parentChildrenSpecs, armatureNode, poseBonesGathered):
      poseBonesGathered.append((poseBone, armatureNode))

      # Start the specs for the bone node.
      bone = poseBone.bone
      nodeSpecs = {"Name": poseBone.name}
      nodeSpecs[SCENE_NODE_TYPE_KEY] = SCENE_NODE_TYPE_NAME
      parentChildrenSpecs.append(nodeSpecs)

      # Get the transform of the bone relative to its parent.  Blender provides
      # the transform relative to the armature.  For now, use the rest pose by
      # default.
      if bone.parent:
         parentTransform = bone.parent.matrix_local
      else:
         parentTransform = Matrix()
      nodeTransformLocal = parentTransform.inverted_safe() @ bone.matrix_local

      nodePositionLocal = nodeTransformLocal.to_translation()
      nodeRotationLocal = nodeTransformLocal.to_quaternion()
      nodeScaleLocal = nodeTransformLocal.to_scale()

      nodeSpecs[SCENE_NODE_POSITION_OFFSET_KEY] = Point3FStringBuild((nodePositionLocal.x, nodePositionLocal.y, nodePositionLocal.z))
      nodeSpecs[SCENE_NODE_ROTATION_KEY] = QuaternionStringBuild((nodeRotationLocal.w, nodeRotationLocal.x, nodeRotationLocal.y, nodeRotationLocal.z))
      if (nodeScaleLocal.x != 1.0) or (nodeScaleLocal.y != 1.0) or (nodeScaleLocal.z != 1.0):
         nodeSpecs[SCENE_NODE_SCALE_KEY] = Point3FStringBuild((nodeScaleLocal.x, nodeScaleLocal.y, nodeScaleLocal.z))

      # Recurse to this bone's children.
      if len(poseBone.children) > 0:
         poseBoneChildrenSpecs = nodeSpecs[SCENE_NODE_CHILDREN_KEY] = []
         for childPoseBone in poseBone.children:
            self.GatherBone(childPoseBone, poseBoneChildrenSpecs, armatureNode, poseBonesGathered)

   #----------------------------------------------------------------------------

   # Gather animation data from the relevant objects.
   def GatherAnimation(self, nodesGathered, poseBonesGathered, shapeKeysGathered, submeshExportAsMorphTargetsSamplers, fileData):
      # Create the entry for animation data for the exported file.
      animationSpecs = fileData["Animation"] = {}
      channelsSpecs = animationSpecs["Channels"] = []
      animationSpecs["FrameRate"] = bpy.context.scene.render.fps

      # Set up objects to sample the supported properties from the relevant
      # scene nodes.
      nodeAnimationSamplers = []
      for node in nodesGathered:
         nodeAnimationSamplers.append(AnimationSamplerSceneNode(node))

      # Set up objects to sample the transforms of the relevant pose bones.
      poseBoneAnimationSamplers = []
      for poseBone, armatureNode in poseBonesGathered:
         poseBoneAnimationSamplers.append(AnimationSamplerSceneNodeBone(poseBone, armatureNode))

      # Set up objects to sample the weights from the relevant shape keys.
      shapeKeyAnimationSamplers = []
      for node, shapeKey in shapeKeysGathered:
         shapeKeyAnimationSamplers.append(AnimationSamplerShapeKey(node, shapeKey))

      # Loop through the range of frames sampling supported properties from the
      # included nodes.
      restoreFrameNumber = bpy.context.scene.frame_current
      frameBegin = animationSpecs["FrameBegin"] = max(0, bpy.context.scene.duck_animation_frame_begin)
      frameEnd = animationSpecs["FrameEnd"] = max(0, bpy.context.scene.duck_animation_frame_end)
      for frameNumber in range(frameBegin, frameEnd + 1):
         # Set the frame.
         bpy.context.scene.frame_set(frameNumber)

         # Sample values from the nodes.
         for sampler in nodeAnimationSamplers:
            sampler.Sample()

         # Sample values from the pose bones.
         for sampler in poseBoneAnimationSamplers:
            sampler.Sample()

         # Sample values from the shape keys.
         for sampler in shapeKeyAnimationSamplers:
            sampler.Sample()

         # Sample submeshes to be baked and exported as morph target animations.
         for sampler in submeshExportAsMorphTargetsSamplers:
            sampler.Sample()

      # Restore the frame number.
      bpy.context.scene.frame_set(restoreFrameNumber)

      # Convert the gathered data into specifications.
      for sampler in nodeAnimationSamplers:
         sampler.Export(channelsSpecs)
      for sampler in poseBoneAnimationSamplers:
         sampler.Export(channelsSpecs)
      for sampler in shapeKeyAnimationSamplers:
         sampler.Export(channelsSpecs)
      for sampler in submeshExportAsMorphTargetsSamplers:
         sampler.Export(channelsSpecs, frameBegin, frameEnd)

#===============================================================================

# Helper class for building submeshes.
class Vertex:
   position = None
   normal = None
   normalSmooth = None
   tangent = None
   originalIndex = -1
   originalLoopIndex = -1
   index = -1

   def __init__(self):
      self.texCoords = {}
      self.colors = {}

#===============================================================================

# Helper class for deduplicating vertices.
class VertexTreeNode:
   def __init__(self, position):
      self.position = position
      self.children = [None] * 8
      self.vertices = []

#===============================================================================

# Helper class for deduplicating vertices.
class VertexTree:
   def __init__(self):
      self.root = None
      self.vertices = []

   def VertexIndexGet(self, vertex):
      return self.VertexIndexGetHelper(None, 0, vertex)

   def VertexIndexGetHelper(self, parentTreeNode, treeNodeIndex, vertex):
      treeNode = None
      if parentTreeNode:
         treeNode = parentTreeNode.children[treeNodeIndex]
      else:
         treeNode = self.root
      if not treeNode:
         # If an empty node has been reached, add a new one.
         treeNode = VertexTreeNode(vertex.position)
         if not parentTreeNode:
            self.root = treeNode
         else:
            parentTreeNode.children[treeNodeIndex] = treeNode
      elif treeNode.position == vertex.position:
         # If the position matches the current node, see if it matches any
         # vertices in this collection.
         for treeNodeVertex in treeNode.vertices:
            if (treeNodeVertex.normal == vertex.normal) and (treeNodeVertex.tangent == vertex.tangent) and (treeNodeVertex.normalSmooth == vertex.normalSmooth):
               differenceFound = False
               for uvLayerName, texCoord in vertex.texCoords.items():
                  if texCoord != treeNodeVertex.texCoords[uvLayerName]:
                     differenceFound = True
                     break
               for colorLayerName, color in vertex.colors.items():
                  existingColor = treeNodeVertex.colors[colorLayerName]
                  if (color[0] != existingColor[0]) or (color[1] != existingColor[1]) or (color[2] != existingColor[2]) or (color[3] != existingColor[3]):
                     differenceFound = True
                     break
               if not differenceFound:
                  return treeNodeVertex.index
      else:
         # If the position doesn't match, recurse the relevant octant among the
         # children of this node.
         childIndex = 0
         if vertex.position.x > treeNode.position.x:
            childIndex += 1
         if vertex.position.y > treeNode.position.y:
            childIndex += 2
         if vertex.position.z > treeNode.position.z:
            childIndex += 4
         return self.VertexIndexGetHelper(treeNode, childIndex, vertex)
      # This is a new vertex for this position.
      vertex.index = len(self.vertices)
      treeNode.vertices.append(vertex)
      self.vertices.append(vertex)
      return vertex.index

#===============================================================================

# Object for storing a single sampled value for a given time.
class AnimationSample:
   time = 0
   value = None

   def __init__(self, time, value):
      self.time = time
      self.value = value

#===============================================================================

# Object for sampling supported properties.
class AnimationSampler:
   # Assuming 'samples' is a list of AnimationSamples, remove unnecessary samples.
   def SamplesRedundantRemove(self, samples):
      # Search the samples for cases where 3 consective sample values are
      # colinear.  Remove the middle samples in those cases.
      sampleFirstIndex = 0
      while (sampleFirstIndex + 2) < len(samples):
         sampleFirst = samples[sampleFirstIndex]
         sampleSecond = samples[sampleFirstIndex + 1]
         sampleThird = samples[sampleFirstIndex + 2]

         t = (sampleSecond.time - sampleFirst.time) / (sampleThird.time - sampleFirst.time)
         sampleSecondValuePredicted = (sampleFirst.value * (1.0 - t)) + (sampleThird.value * t)

         if sampleSecondValuePredicted == sampleSecond.value:
            del samples[sampleFirstIndex + 1]
         else:
            sampleFirstIndex += 1

#===============================================================================

# Object for sampling supported properties from a scene node.
class AnimationSamplerSceneNode(AnimationSampler):
   node = None
   positionSamples = None
   rotationSamples = None
   scaleSamples = None
   positionChannelSpecs = None
   rotationChannelSpecs = None
   scaleChannelSpecs = None

   def __init__(self, node):
      self.node = node
      self.positionSamples = []
      self.rotationSamples = []
      self.scaleSamples = []
      self.positionChannelSpecs = {}
      self.rotationChannelSpecs = {}
      self.scaleChannelSpecs = {}

   # Sample the supported node properties for the current frame.
   def Sample(self):
      # Get the transform of the node relative to its parent.
      if self.node.parent:
         parentTransform = self.node.parent.matrix_world
      else:
         parentTransform = Matrix()
      nodeTransformLocal = parentTransform.inverted_safe() @ self.node.matrix_world

      nodePositionLocal = nodeTransformLocal.to_translation()
      nodeRotationLocal = nodeTransformLocal.to_quaternion()
      nodeScaleLocal = nodeTransformLocal.to_scale()

      # Add sampled values for these properties.
      frameNumber = bpy.context.scene.frame_current
      self.positionSamples.append(AnimationSample(frameNumber, nodePositionLocal))
      self.rotationSamples.append(AnimationSample(frameNumber, nodeRotationLocal))
      self.scaleSamples.append(AnimationSample(frameNumber, nodeScaleLocal))

   # Export the gathered animation channels to the given collection of specs.
   def Export(self, channelsSpecs):
      # Export the position data.
      channelsSpecs.append(self.positionChannelSpecs)
      self.positionChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_KEY] = ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_SCENE_NODE_NAME
      self.positionChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_KEY] = self.node.name
      self.positionChannelSpecs[ANIMATION_CHANNEL_TARGET_PROPERTY_KEY] = "PositionOffset"
      self.positionChannelSpecs[ANIMATION_CHANNEL_TYPE_KEY] = ANIMATION_CHANNEL_TYPE_POINT3F_SPLINE_LINEAR_NAME
      positionPointsSpecs = self.positionChannelSpecs[ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY] = []
      self.SamplesRedundantRemove(self.positionSamples)
      for sample in self.positionSamples:
         pointSpecs = {}
         positionPointsSpecs.append(pointSpecs)
         pointSpecs[SPLINE_LINEAR_POINT_TIME_KEY] = sample.time
         sampleValue = sample.value
         pointSpecs[SPLINE_LINEAR_POINT_VALUE_KEY] = Point3FStringBuild((sampleValue.x, sampleValue.y, sampleValue.z))

      # Export the rotation data.
      channelsSpecs.append(self.rotationChannelSpecs)
      self.rotationChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_KEY] = ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_SCENE_NODE_NAME
      self.rotationChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_KEY] = self.node.name
      self.rotationChannelSpecs[ANIMATION_CHANNEL_TARGET_PROPERTY_KEY] = "Rotation"
      self.rotationChannelSpecs[ANIMATION_CHANNEL_TYPE_KEY] = ANIMATION_CHANNEL_TYPE_QUATERNION_SPLINE_LINEAR_NAME
      rotationPointsSpecs = self.rotationChannelSpecs[ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY] = []
      self.SamplesRedundantRemove(self.rotationSamples)
      for sample in self.rotationSamples:
         pointSpecs = {}
         rotationPointsSpecs.append(pointSpecs)
         pointSpecs[SPLINE_LINEAR_POINT_TIME_KEY] = sample.time
         sampleValue = sample.value
         pointSpecs[SPLINE_LINEAR_POINT_VALUE_KEY] = QuaternionStringBuild((sampleValue.w, sampleValue.x, sampleValue.y, sampleValue.z))

      # Export the scale data.
      channelsSpecs.append(self.scaleChannelSpecs)
      self.scaleChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_KEY] = ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_SCENE_NODE_NAME
      self.scaleChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_KEY] = self.node.name
      self.scaleChannelSpecs[ANIMATION_CHANNEL_TARGET_PROPERTY_KEY] = "Scale"
      self.scaleChannelSpecs[ANIMATION_CHANNEL_TYPE_KEY] = ANIMATION_CHANNEL_TYPE_POINT3F_SPLINE_LINEAR_NAME
      scalePointsSpecs = self.scaleChannelSpecs[ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY] = []
      self.SamplesRedundantRemove(self.scaleSamples)
      for sample in self.scaleSamples:
         pointSpecs = {}
         scalePointsSpecs.append(pointSpecs)
         pointSpecs[SPLINE_LINEAR_POINT_TIME_KEY] = sample.time
         sampleValue = sample.value
         pointSpecs[SPLINE_LINEAR_POINT_VALUE_KEY] = Point3FStringBuild((sampleValue.x, sampleValue.y, sampleValue.z))

#===============================================================================

# Object for sampling supported properties from a bone for a scene node.
class AnimationSamplerSceneNodeBone(AnimationSamplerSceneNode):
   armatureNode = None

   def __init__(self, node, armatureNode):
      super().__init__(node)
      self.armatureNode = armatureNode

   # Sample the supported pose bone properties for the current frame.
   def Sample(self):
      # Get the transform of the node relative to its parent.  Blender provides
      # transforms relative to the armature.
      if self.node.parent:
         parentTransform = self.node.parent.matrix
      else:
         parentTransform = Matrix()
      nodeTransformLocal = parentTransform.inverted_safe() @ self.node.matrix

      nodePositionLocal = nodeTransformLocal.to_translation()
      nodeRotationLocal = nodeTransformLocal.to_quaternion()
      nodeScaleLocal = nodeTransformLocal.to_scale()

      # Add sampled values for these properties.
      frameNumber = bpy.context.scene.frame_current
      self.positionSamples.append(AnimationSample(frameNumber, nodePositionLocal))
      self.rotationSamples.append(AnimationSample(frameNumber, nodeRotationLocal))
      self.scaleSamples.append(AnimationSample(frameNumber, nodeScaleLocal))

   # Export the gathered animation channels to the given collection of specs.
   def Export(self, channelsSpecs):
      # Save the name of the armature to help reduce ambiguities when there are
      # multiple armatures in the scene.
      if self.armatureNode and self.armatureNode.name:
         self.positionChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_ANCESTOR_KEY] = self.armatureNode.name
         self.rotationChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_ANCESTOR_KEY] = self.armatureNode.name
         self.scaleChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_ANCESTOR_KEY] = self.armatureNode.name

      super().Export(channelsSpecs)

#===============================================================================

# Object for sampling supported properties from a shape key.
class AnimationSamplerShapeKey(AnimationSampler):
   node = None
   shapeKey = None
   weightSamples = None

   def __init__(self, node, shapeKey):
      self.node = node
      self.shapeKey = shapeKey
      self.weightSamples = []

   # Sample the supported shape key properties for the current frame.
   def Sample(self):
      # Get the weight.
      weight = self.shapeKey.value

      # Add sampled value for this property.
      frameNumber = bpy.context.scene.frame_current
      self.weightSamples.append(AnimationSample(frameNumber, weight))

   # Export the gathered animation channels to the given collection of specs.
   def Export(self, channelsSpecs):
      # Export the weight data.
      weightChannelSpecs = {}
      channelsSpecs.append(weightChannelSpecs)
      weightChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_KEY] = ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_SCENE_NODE_NAME
      weightChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_KEY] = self.node.name
      weightChannelSpecs[ANIMATION_CHANNEL_TARGET_SUBOBJECT_TYPE_KEY] = ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_MORPH_TARGET_NAME
      weightChannelSpecs[ANIMATION_CHANNEL_TARGET_SUBOBJECT_KEY] = self.shapeKey.name
      weightChannelSpecs[ANIMATION_CHANNEL_TARGET_PROPERTY_KEY] = ANIMATION_CHANNEL_TARGET_PROPERTY_WEIGHT_NAME
      weightChannelSpecs[ANIMATION_CHANNEL_TYPE_KEY] = ANIMATION_CHANNEL_TYPE_FLOAT_SPLINE_LINEAR_NAME
      weightPointsSpecs = weightChannelSpecs[ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY] = []
      self.SamplesRedundantRemove(self.weightSamples)
      for sample in self.weightSamples:
         pointSpecs = {}
         weightPointsSpecs.append(pointSpecs)
         pointSpecs[SPLINE_LINEAR_POINT_TIME_KEY] = sample.time
         pointSpecs[SPLINE_LINEAR_POINT_VALUE_KEY] = sample.value

#===============================================================================

# Object for sampling a submesh for export as an animated series of baked morph
# targets.
class AnimationSamplerSubmeshExportAsMorphTargets(AnimationSampler):
   exporter = None
   node = None
   vertexTree = None
   boundingVolumeHelper = None
   includeTangents = False
   includeSmoothNormals = False
   meshSpecs = None
   submeshSpecs = None
   expectedVertexCount = 0
   generatedMorphTargets = None

   def __init__(self, exporter, node, vertexTree, boundingVolumeHelper, includeTangents, includeSmoothNormals, meshSpecs, submeshSpecs, expectedVertexCount):
      self.exporter = exporter
      self.node = node
      self.vertexTree = vertexTree
      self.boundingVolumeHelper = boundingVolumeHelper
      self.includeTangents = includeTangents
      self.includeSmoothNormals = includeSmoothNormals
      self.meshSpecs = meshSpecs
      self.submeshSpecs = submeshSpecs
      self.expectedVertexCount = expectedVertexCount
      self.generatedMorphTargetFrameNumbers = []

   # Sample the submesh for the current frame.
   def Sample(self):
      frameNumber = bpy.context.scene.frame_current
      morphTargetName = "Frame%04d" % ((frameNumber))
      if self.exporter.SubmeshFinishMorphTargetHelper(self.node, self.vertexTree, self.boundingVolumeHelper, self.includeTangents, self.includeSmoothNormals, self.submeshSpecs, self.expectedVertexCount, morphTargetName):
         self.generatedMorphTargetFrameNumbers.append(frameNumber)

   # Export the gathered animation channels to the given collection of specs.
   def Export(self, channelsSpecs, frameBegin, frameEnd):
      # Remove duplicate consecutive frames.

      # Generate channel weight animations to show the correct morph target at
      # the correct time.
      for frameNumber in self.generatedMorphTargetFrameNumbers:
         morphTargetName = "Frame%04d" % ((frameNumber))
         weightChannelSpecs = {}
         channelsSpecs.append(weightChannelSpecs)
         weightChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_KEY] = ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_SCENE_NODE_NAME
         weightChannelSpecs[ANIMATION_CHANNEL_TARGET_OBJECT_KEY] = self.node.name
         weightChannelSpecs[ANIMATION_CHANNEL_TARGET_SUBOBJECT_TYPE_KEY] = ANIMATION_CHANNEL_TARGET_OBJECT_TYPE_MORPH_TARGET_NAME
         weightChannelSpecs[ANIMATION_CHANNEL_TARGET_SUBOBJECT_KEY] = morphTargetName
         weightChannelSpecs[ANIMATION_CHANNEL_TARGET_PROPERTY_KEY] = ANIMATION_CHANNEL_TARGET_PROPERTY_WEIGHT_NAME
         weightChannelSpecs[ANIMATION_CHANNEL_TYPE_KEY] = ANIMATION_CHANNEL_TYPE_FLOAT_SPLINE_LINEAR_NAME
         weightPointsSpecs = weightChannelSpecs[ANIMATION_CHANNEL_SPLINE_LINEAR_POINTS_KEY] = []

         # Initial influence is 0, except for the first frame.
         if frameNumber > frameBegin:
            pointSpecs = {}
            weightPointsSpecs.append(pointSpecs)
            pointSpecs[SPLINE_LINEAR_POINT_TIME_KEY] = max(0, frameNumber - 1)
            pointSpecs[SPLINE_LINEAR_POINT_VALUE_KEY] = 0.0

         # Middle influence is 1.
         pointSpecs = {}
         weightPointsSpecs.append(pointSpecs)
         pointSpecs[SPLINE_LINEAR_POINT_TIME_KEY] = frameNumber
         pointSpecs[SPLINE_LINEAR_POINT_VALUE_KEY] = 1.0

         # Final influence is 0, except for the final frame.
         if frameNumber < frameEnd:
            pointSpecs = {}
            weightPointsSpecs.append(pointSpecs)
            pointSpecs[SPLINE_LINEAR_POINT_TIME_KEY] = frameNumber + 1
            pointSpecs[SPLINE_LINEAR_POINT_VALUE_KEY] = 0.0

      # Since this may affect the bounding volume in a relatively predictable
      # way, update the bounding volume in the mesh specs now.
      self.boundingVolumeHelper.OutputBoundingVolumeSpecs(self.meshSpecs)

#===============================================================================

# Manage Duck-specific scene properties.
class DuckScenePanel(bpy.types.Panel):
   bl_idname = "SCENE_PT_duck"
   bl_label = "Duck"
   bl_space_type = "PROPERTIES"
   bl_region_type = "WINDOW"
   bl_context = "scene"

   def draw(self, context):
      scene = context.scene
      layoutGridFlow = self.layout.grid_flow(row_major=True, columns=0, even_columns=False, even_rows=False, align=True)
      layoutColumn = layoutGridFlow.column()
      layoutColumn.prop(scene, "duck_export_animation", text = "Export Animation")
      layoutColumn.prop(scene, "duck_animation_frame_begin", text = "Animation Frame Begin")
      layoutColumn.prop(scene, "duck_animation_frame_end", text = "Animation Frame End")

#===============================================================================

# Manage Duck-specific object properties.
class DuckObjectPanel(bpy.types.Panel):
   bl_idname = "OBJECT_PT_duck"
   bl_label = "Duck"
   bl_space_type = "PROPERTIES"
   bl_region_type = "WINDOW"
   bl_context = "object"

   def draw(self, context):
      object = context.active_object
      layoutGridFlow = self.layout.grid_flow(row_major=True, columns=0, even_columns=False, even_rows=False, align=True)
      layoutColumn = layoutGridFlow.column()
      layoutColumn.prop(object, "duck_export", text = "Export")
      layoutColumn.prop(object, "duck_visible_specific", text = "Visible (specific)")
      layoutColumn.prop(object, "duck_collidable_specific", text = "Collidable (specific)")
      layoutColumn.prop(object, "duck_billboard_mode", text = "Billboard Mode")
      layoutColumn.prop(object, "duck_depth_sort_priority", text = "Depth Sort Priority")
      layoutColumn.prop(object, "duck_bounding_volume_scale", text = "Bounding Volume Scale")
      layoutColumn.prop(object, "duck_lod_group", text = "LOD Group")
      layoutColumn.prop(object, "duck_lod_group_draw_distance_max", text = "LOD Group Draw Distance Max")
      layoutColumn.prop(object, "duck_lod_level_draw_distance_min", text = "LOD Level Draw Distance Min")
      layoutColumn.prop(object, "duck_object_bake_type", text = "Object Bake Type")

#===============================================================================

# Manage Duck-specific material properties.
class DuckMaterialPanel(bpy.types.Panel):
   bl_idname = "MATERIAL_PT_duck"
   bl_label = "Duck"
   bl_space_type = "PROPERTIES"
   bl_region_type = "WINDOW"
   bl_context = "material"

   def draw(self, context):
      material = context.active_object.active_material
      layoutGridFlow = self.layout.grid_flow(row_major=True, columns=0, even_columns=False, even_rows=False, align=True)
      layoutColumn = layoutGridFlow.column()
      layoutColumn.prop(material, "duck_two_sided", text = "Two-Sided")
      layoutColumn.prop(material, "duck_back_face_normals_flip", text = "Flip Back-Face Normals")
      layoutColumn.prop(material, "duck_alpha_mode", text = "Alpha Mode")
      layoutColumn.prop(material, "duck_shadow_cast_mode", text = "Shadow Cast Mode")
      layoutColumn.prop(material, "duck_additive_blending", text = "Additive Blending")
      layoutColumn.prop(material, "duck_unlit", text = "Unlit")
      layoutColumn.prop(material, "duck_distance_scale_down", text = "Distance Scale Down")
      layoutColumn.prop(material, "duck_distance_scale_down_near", text = "Distance Scale Down Near")
      layoutColumn.prop(material, "duck_distance_scale_down_far", text = "Distance Scale Down Far")
      layoutColumn.prop(material, "duck_normals_up_local", text = "Normals Up")
      layoutColumn.prop(material, "duck_outline_draw", text = "Outline")
      layoutColumn.prop(material, "duck_wind_apply", text = "Apply Wind")
      layoutColumn.prop(material, "duck_wind_lean_coefficient", text = "Wind Lean Coefficient")
      layoutColumn.prop(material, "duck_wind_shake_coefficient", text = "Wind Shake Coefficient")
      layoutColumn.prop(material, "duck_wind_shake_detail_coefficient", text = "Wind Shake Detail Coefficient")

#===============================================================================

# Manage Duck-specific light properties.
class DuckLightPanel(bpy.types.Panel):
   bl_idname = "LIGHT_PT_duck"
   bl_label = "Duck"
   bl_space_type = "PROPERTIES"
   bl_region_type = "WINDOW"
   bl_context = "data"

   @classmethod
   def poll(cls, context):
      try:
         return context.active_object.type == "LIGHT"
      except:
         return False

   def draw(self, context):
      light = context.active_object.data
      layoutGridFlow = self.layout.grid_flow(row_major=True, columns=0, even_columns=False, even_rows=False, align=True)
      layoutColumn = layoutGridFlow.column()
      layoutColumn.prop(light, "duck_bake_type", text = "Bake Type")

#===============================================================================

# Helper function for returning a Frog string representation of a Point3F.
def Point3FStringBuild(point3F):
   return "%.10g|%.10g|%.10g" % (point3F[0], point3F[1], point3F[2])

#-------------------------------------------------------------------------------

# Helper function for returning a Frog string representation of a Quaternion.
def QuaternionStringBuild(quaternion):
   return "%.10g|%.10g|%.10g|%.10g" % (quaternion[0], quaternion[1], quaternion[2], quaternion[3])

#-------------------------------------------------------------------------------

# Helper function for returning a Frog string representation of a ColorRGBA8.
def ColorRGBA8StringBuild(color):
   alpha = 255
   if len(color) > 3:
      alpha = color[3]
   red = max(0.0, min(255.0, color[0] * 255.0))
   green = max(0.0, min(255.0, color[1] * 255.0))
   blue = max(0.0, min(255.0, color[2] * 255.0))
   alpha = max(0.0, min(255.0, alpha * 255.0))
   return "%i|%i|%i|%i" % (red, green, blue, alpha)

#-------------------------------------------------------------------------------

# Helper function for returning a Frog string representation of a ColorRGBA32F.
def ColorRGBA32FStringBuild(color):
   alpha = 1.0
   if len(color) > 3:
      alpha = color[3]
   return "%.10g|%.10g|%.10g|%.10g" % (color[0], color[1], color[2], alpha)

#-------------------------------------------------------------------------------

# Helper function for handling types normally unsupported for serialization to
# JSON.
def DebugJSONSerializer(o):
   if isinstance(o, bytes):
      # Convert binary data to a hex string.
      return "<binary data (%d bytes)>" % len(o)
   else:
      # Fall back on the default.
      return json.JSONEncoder.default(o)

#-------------------------------------------------------------------------------

# Object for determining a bounding volume.
class BoundingVoumeHelper():
   boundingBoxMinX = 0
   boundingBoxMaxX = 0
   boundingBoxMinY = 0
   boundingBoxMaxY = 0
   boundingBoxMinZ = 0
   boundingBoxMaxZ = 0

   # Set the bounding volume based on Blender's bounds for the given node.
   def Set(self, node):
      # Get the bounding box.
      blenderBounds = node.bound_box
      self.boundingBoxMinX = min(blenderBounds[0][0], blenderBounds[1][0],
         blenderBounds[2][0], blenderBounds[3][0], blenderBounds[4][0],
         blenderBounds[5][0], blenderBounds[6][0], blenderBounds[7][0])
      self.boundingBoxMaxX = max(blenderBounds[0][0], blenderBounds[1][0],
         blenderBounds[2][0], blenderBounds[3][0], blenderBounds[4][0],
         blenderBounds[5][0], blenderBounds[6][0], blenderBounds[7][0])
      self.boundingBoxMinY = min(blenderBounds[0][1], blenderBounds[1][1],
         blenderBounds[2][1], blenderBounds[3][1], blenderBounds[4][1],
         blenderBounds[5][1], blenderBounds[6][1], blenderBounds[7][1])
      self.boundingBoxMaxY = max(blenderBounds[0][1], blenderBounds[1][1],
         blenderBounds[2][1], blenderBounds[3][1], blenderBounds[4][1],
         blenderBounds[5][1], blenderBounds[6][1], blenderBounds[7][1])
      self.boundingBoxMinZ = min(blenderBounds[0][2], blenderBounds[1][2],
         blenderBounds[2][2], blenderBounds[3][2], blenderBounds[4][2],
         blenderBounds[5][2], blenderBounds[6][2], blenderBounds[7][2])
      self.boundingBoxMaxZ = max(blenderBounds[0][2], blenderBounds[1][2],
         blenderBounds[2][2], blenderBounds[3][2], blenderBounds[4][2],
         blenderBounds[5][2], blenderBounds[6][2], blenderBounds[7][2])

   # Update the bounding volume to include Blender's bounds for the given node.
   def Union(self, node):
      blenderBounds = node.bound_box
      self.boundingBoxMinX = min(self.boundingBoxMinX, blenderBounds[0][0], blenderBounds[1][0],
         blenderBounds[2][0], blenderBounds[3][0], blenderBounds[4][0],
         blenderBounds[5][0], blenderBounds[6][0], blenderBounds[7][0])
      self.boundingBoxMaxX = max(self.boundingBoxMaxX, blenderBounds[0][0], blenderBounds[1][0],
         blenderBounds[2][0], blenderBounds[3][0], blenderBounds[4][0],
         blenderBounds[5][0], blenderBounds[6][0], blenderBounds[7][0])
      self.boundingBoxMinY = min(self.boundingBoxMinY, blenderBounds[0][1], blenderBounds[1][1],
         blenderBounds[2][1], blenderBounds[3][1], blenderBounds[4][1],
         blenderBounds[5][1], blenderBounds[6][1], blenderBounds[7][1])
      self.boundingBoxMaxY = max(self.boundingBoxMaxY, blenderBounds[0][1], blenderBounds[1][1],
         blenderBounds[2][1], blenderBounds[3][1], blenderBounds[4][1],
         blenderBounds[5][1], blenderBounds[6][1], blenderBounds[7][1])
      self.boundingBoxMinZ = min(self.boundingBoxMinZ, blenderBounds[0][2], blenderBounds[1][2],
         blenderBounds[2][2], blenderBounds[3][2], blenderBounds[4][2],
         blenderBounds[5][2], blenderBounds[6][2], blenderBounds[7][2])
      self.boundingBoxMaxZ = max(self.boundingBoxMaxZ, blenderBounds[0][2], blenderBounds[1][2],
         blenderBounds[2][2], blenderBounds[3][2], blenderBounds[4][2],
         blenderBounds[5][2], blenderBounds[6][2], blenderBounds[7][2])

   # Write the bounding volume specifications to the given 'meshSpecs'.
   def OutputBoundingVolumeSpecs(self, meshSpecs):
      # Get the bounding sphere.  For now, base it on the bounding box.
      boundingSphereCenter = (((self.boundingBoxMinX + self.boundingBoxMaxX) / 2),
         ((self.boundingBoxMinY + self.boundingBoxMaxY) / 2),
         ((self.boundingBoxMinZ + self.boundingBoxMaxZ) / 2))
      boundingSphereRadius = 0.5 * math.sqrt(math.pow(self.boundingBoxMaxX - self.boundingBoxMinX, 2) +
         math.pow(self.boundingBoxMaxY - self.boundingBoxMinY, 2) +
         math.pow(self.boundingBoxMaxZ - self.boundingBoxMinZ, 2))
      meshSpecs["BoundingSphereCenter"] = Point3FStringBuild(boundingSphereCenter)
      meshSpecs["BoundingSphereRadius"] = boundingSphereRadius

#-------------------------------------------------------------------------------

def MenuExport(self, context):
    self.layout.operator(DuckExporter.bl_idname, text = "Duck (.dsc)")

#-------------------------------------------------------------------------------

def register():
   bpy.utils.register_class(DuckExporter)
   bpy.types.TOPBAR_MT_file_export.append(MenuExport)

   # Duck scene properties.
   bpy.types.Scene.duck_export_animation = bpy.props.BoolProperty(name = "Export Animation", description =
      "True if animation should be included when exporting.", default = False)
   bpy.types.Scene.duck_animation_frame_begin = bpy.props.IntProperty(name = "Animation Frame Begin", description =
      "First frame to include when exporting animations.",
      default = 1, min = 0)
   bpy.types.Scene.duck_animation_frame_end = bpy.props.IntProperty(name = "Animation Frame End", description =
      "Final frame to include when exporting animations.",
      default = 250, min = 0)
   bpy.utils.register_class(DuckScenePanel)

   # Duck object properties.
   bpy.types.Object.duck_export = bpy.props.BoolProperty(name = "Export", description =
      "False if this node and its descendants should be excluded when exporting.", default = True)
   bpy.types.Object.duck_visible_specific = bpy.props.BoolProperty(name = "Visible (specific)", description =
      "True if this node should be visible by default in-game.  This does not affect its descendants.", default = True)
   bpy.types.Object.duck_collidable_specific = bpy.props.BoolProperty(name = "Collidable (specific)", description =
      "True if this node's geometry should be included by default for collision detection.  This does not affect its descendants.", default = True)
   bpy.types.Object.duck_billboard_mode = bpy.props.EnumProperty(name = "Billboard Mode", default = "Disabled", description =
      "How opacity should be handled.", items = [
      ("Disabled", "Disabled", "The object behaves as usual", 0),
      ("Billboard", "Billboard", "Align the object so that its local +y direction pointing toward the viewer and perpendicular to the projection plane.", 1),
      ("Upright", "Upright", "Rotate the object about its local z axis trying to make its local +y direction point toward the user and perpendicular to the projection plane.", 2)
   ])
   bpy.types.Object.duck_depth_sort_priority = bpy.props.FloatProperty(name = "Depth Sort Priority", description =
      "Depth-sorted nodes of numerically higher priority are drawn after depth-sorted nodes of lower priority.  Depth-sorted nodes of the same priority are sorted by depth.  The default priority is 0.",
      default = 0.0)
   bpy.types.Object.duck_bounding_volume_scale = bpy.props.FloatProperty(name = "Bounding Volume Scale", description =
      "Scale which should be applied to bounding volumes, like bounding spheres, for culling purposes.",
      default = 1.0, min = 0.0)
   bpy.types.Object.duck_lod_group = bpy.props.BoolProperty(name = "LOD Group", description =
      "True if this node's children form an LOD group.", default = False)
   bpy.types.Object.duck_lod_group_draw_distance_max = bpy.props.FloatProperty(name = "LOD Group Draw Distance Max", description =
      "If this value is greater than 0 and this node is an LOD group, don't draw any of this node's children if the distance from the camera is greater than this amount.",
      default = 0.0, min = 0.0)
   bpy.types.Object.duck_lod_level_draw_distance_min = bpy.props.FloatProperty(name = "LOD Level Draw Distance Min", description =
      "Don't draw this version if the distance to the camera is less than this amount.  Try to draw a higher-quality version instead.",
      default = 0.0, min = 0.0)
   bpy.types.Object.duck_object_bake_type = bpy.props.EnumProperty(name = "Object Bake Type", default = "None", description =
      "How this object should be exported.", items = [
      ("None", "None", "Don't go out of your way to simplify or convert the object when exporting.", 0),
      (DUCK_OBJECT_BAKE_TYPE_MORPH_TARGETS_NAME, "Morph Targets", "Try to export the object as a single mesh with all modifiers applied, and try to represent the animation of the shape itself as a series of morph targets.", 1),
   ])
   bpy.utils.register_class(DuckObjectPanel)

   # Duck material properties.
   bpy.types.Material.duck_two_sided = bpy.props.BoolProperty(name = "Two-Sided", description =
      "True if the back surface should be drawn in addition to the front.", default = False)
   bpy.types.Material.duck_back_face_normals_flip = bpy.props.BoolProperty(name = "Flip Back-Face Normals", description =
      "For lighting purposes, true if the the normal of the back surface should point in the opposite direction from that of the front surface.", default = True)
   bpy.types.Material.duck_alpha_mode = bpy.props.EnumProperty(name = "Alpha Mode", default = "Opaque", description =
      "How opacity should be handled.", items = [
      ("Opaque", "Opaque", "Full opacity", 0),
      ("Blend", "Blend", "Use base color alpha for opacity.", 1),
      ("Clip", "Clip", "Discard fragments with a base color alpha below the clip threshold.", 2),
      ("AlphaToCoverage", "Alpha to Coverage", "Used with MSAA on certain platforms to provide limited order-independent transparency.  This is not compatible with additive blending.", 3)
   ])
   bpy.types.Material.duck_shadow_cast_mode = bpy.props.EnumProperty(name = "Shadow Cast Mode", default = "Default", description =
      "How shadows should be cast by this material.", items = [
      ("Default", "Default", "Use the default shadow casting mode for the material's alpha mode.", 0),
      ("None", "None", "Do not cast shadows.", 1),
      ("Opaque", "Opaque", "When rendering the shadow buffer, ignore alpha and treat all geometry as opaque.", 2),
      ("AlphaClip", "AlphaClip", "When rendering the shadow buffer, discard fragments with a base color alpha below the clip threshold.", 3)
   ])
   bpy.types.Material.duck_additive_blending = bpy.props.FloatProperty(name = "Additive", description =
      "Extent to which blending should be additive rather than normal.  This does not work with AlphaToCoverage.", \
       default = 0.0, min = 0.0, max = 1.0)
   bpy.types.Material.duck_unlit = bpy.props.BoolProperty(name = "Unlit", description =
      "True if realtime lighting should not be performed on this material.", default = False)
   bpy.types.Material.duck_distance_scale_down = bpy.props.BoolProperty(name = "Distance Scale Down", description =
      "True if the object should appear to scale down to zero, depending on its distance from the camera.", default = False)
   bpy.types.Material.duck_distance_scale_down_near = bpy.props.FloatProperty(name = "Distance Scale Down Near", description =
      "When \"Distance Scale Down\" is true, this is the distance at which the object begins to scale down.", \
       default = 0.0, min = 0.0)
   bpy.types.Material.duck_distance_scale_down_far = bpy.props.FloatProperty(name = "Distance Scale Down Far", description =
      "When \"Distance Scale Down\" is true, this is the distance at which the object finishes scaling down to zero.", \
       default = 0.0, min = 0.0)
   bpy.types.Material.duck_normals_up_local = bpy.props.BoolProperty(name = "Normals Up", description =
      "True if the normals should all be treated as pointing upward in local space for lighting purposes.  This is not compatible with normal mapping.", default = False)
   bpy.types.Material.duck_outline_draw = bpy.props.BoolProperty(name = "Outline", description =
      "True if the material should be drawn with an outline.", default = False)
   bpy.types.Material.duck_wind_apply = bpy.props.BoolProperty(name = "Apply Wind", description =
      "True if the material should be move in the wind.", default = False)
   bpy.types.Material.duck_wind_lean_coefficient = bpy.props.FloatProperty(name = "Wind Lean Coefficient", description =
      "Multiplied by the DuckWindLean vertex group weights to affect how much vertices should lean due to the wind.", \
       default = 0.1, min = 0.0)
   bpy.types.Material.duck_wind_shake_coefficient = bpy.props.FloatProperty(name = "Wind Shake Coefficient", description =
      "Multiplied by the DuckWindShake vertex group weights to affect how much vertices should shake due to the wind.", \
       default = 0.1, min = 0.0)
   bpy.types.Material.duck_wind_shake_detail_coefficient = bpy.props.FloatProperty(name = "Wind Shake Detail Coefficient", description =
      "Multiplied by the DuckWindShakeDetail vertex group weights to affect additional, probably high-frequency, shaking due to the wind.", \
       default = 0.05, min = 0.0)
   bpy.utils.register_class(DuckMaterialPanel)

   # Duck light properties.
   bpy.types.Light.duck_bake_type = bpy.props.EnumProperty(name = "Bake Type", default = "Dynamic", description =
      "How this light should influence lightmaps.", items = [
      ("Dynamic", "Dynamic", "This light should not affect lightmaps.", 0),
      ("Indirect", "Indirect", "Lightmaps should include only the indirect (bounced) influence of this light.  Direct light should still be dynamic.", 1),
      ("DirectAndIndirect", "Direct and Indirect", "Lightmaps should include both direct and indirect influence of this light.", 2)
   ])
   bpy.utils.register_class(DuckLightPanel)

#-------------------------------------------------------------------------------

def unregister():
   bpy.types.TOPBAR_MT_file_export.remove(MenuExport)
   bpy.utils.unregister_class(DuckExporter)

   # Duck light properties.
   bpy.utils.unregister_class(DuckLightPanel)
   del bpy.types.Light.duck_bake_type

   # Duck material properties
   bpy.utils.unregister_class(DuckMaterialPanel)
   del bpy.types.Material.duck_wind_shake_detail_coefficient
   del bpy.types.Material.duck_wind_shake_coefficient
   del bpy.types.Material.duck_wind_lean_coefficient
   del bpy.types.Material.duck_wind_apply
   del bpy.types.Material.duck_outline_draw
   del bpy.types.Material.duck_normals_up_local
   del bpy.types.Material.duck_distance_scale_down_far
   del bpy.types.Material.duck_distance_scale_down_near
   del bpy.types.Material.duck_distance_scale_down
   del bpy.types.Material.duck_unlit
   del bpy.types.Material.duck_additive_blending
   del bpy.types.Material.duck_shadow_cast_mode
   del bpy.types.Material.duck_alpha_mode
   del bpy.types.Material.duck_back_face_normals_flip
   del bpy.types.Material.duck_two_sided

   # Duck object properties
   bpy.utils.unregister_class(DuckObjectPanel)
   del bpy.types.Object.duck_object_bake_type
   del bpy.types.Object.duck_lod_level_draw_distance_min
   del bpy.types.Object.duck_lod_group_draw_distance_max
   del bpy.types.Object.duck_lod_group
   del bpy.types.Object.duck_bounding_volume_scale
   del bpy.types.Object.duck_depth_sort_priority
   del bpy.types.Object.duck_billboard_mode
   del bpy.types.Object.duck_collidable_specific
   del bpy.types.Object.duck_visible_specific
   del bpy.types.Object.duck_export

   # Duck scene properties.
   bpy.utils.unregister_class(DuckScenePanel)
   del bpy.types.Scene.duck_animation_frame_end
   del bpy.types.Scene.duck_animation_frame_begin
   del bpy.types.Scene.duck_export_animation

#-------------------------------------------------------------------------------

if __name__ == "__main__":
   register()

#-------------------------------------------------------------------------------
