import bpy
from math import *
import bmesh
import numpy as np
from numpy import newaxis as nax
from bpy_extras import view3d_utils
import time
import mathutils
from ctypes import *
import os
import gc
import platform
import functools

lib=None
if (platform.system() == "Linux"):
	libfile=os.path.dirname(__file__) + "/build/libfclothloader.so"
elif (platform.system() == "Windows"): # 100% untested and without the safetynet of the loader lib
	libfile=os.path.dirname(__file__) + "/build/libfcloth.dll"

def get_settings(context):
	return context.scene.fcloth_settings

class FClothSettings(bpy.types.PropertyGroup):
	collision_mesh: bpy.props.PointerProperty(type=bpy.types.Object, name="Collision Mesh")
	use_c: bpy.props.BoolProperty(name="Use C")


class PANEL_PT_fCloth(bpy.types.Panel):
	"""FCloth Panel"""
	bl_label = "FCloth Panel"
	bl_idname = "PANEL_PT_fCloth"
	bl_space_type = 'VIEW_3D'
	bl_region_type = 'UI'
	bl_category = "Fredriks Tools"
	#gt_show = True

	def draw(self, context):
		status = False
		layout = self.layout
		settings = get_settings(context)
		col = layout.column(align=True)
		col.operator("object.clothtest",text="add keys",icon="HAND")
		if bpy.app.timers.is_registered(loop.update):
			col.operator("object.contrun",text="STOP",icon="PAUSE")
		else:
			col.operator("object.contrun",text="run continuous",icon="PLAY")
		col.operator("object.reset",text="reset",icon="FILE_REFRESH")
		col.prop(settings,"collision_mesh")
		col.prop(settings,"use_c")

def addkeysifneeded(ob):
	keys = ob.data.shape_keys.key_blocks.keys()
	if "Basis" not in keys:
		ob.shape_key_add(name="Basis")
	if "cloth rest" not in keys:
		ob.shape_key_add(name="cloth rest")
	if "cloth shape" not in keys:
		sk=ob.shape_key_add(name="cloth shape")
		sk.value=1

class test(bpy.types.Operator):
	bl_idname = "object.clothtest"
	bl_label = "test button"

	def execute(self,context):
		print("test pressed")
		for ob in context.selected_objects:
			if ob.data.shape_keys is None:
				ob.shape_key_add(name="Basis")
				ob.shape_key_add(name="cloth rest")
				sk=ob.shape_key_add(name="cloth shape")
			addkeysifneeded(ob)
			print(ob)
		return {'FINISHED'}

class reset(bpy.types.Operator):
	bl_idname = "object.reset"
	bl_label = "reset shapekey"

	def execute(self,context):
		print("reset pressed")
		for ob in context.selected_objects:
			if ob.data.shape_keys is None:
				ob.shape_key_add(name="Basis")
				ob.shape_key_add(name="cloth rest")
				sk=ob.shape_key_add(name="cloth shape")
				sk.value=1
			addkeysifneeded(ob)
			shape=ob.data.shape_keys.key_blocks['cloth shape']
			origshape=ob.data.shape_keys.key_blocks['Basis']
			shapedata=[0.0]*len(origshape.data)*3 # numverts*3
			origshape.data.foreach_get("co",shapedata)
			shape.data.foreach_set("co",shapedata)
			shape.value=shape.value
		return {'FINISHED'}

class contrun(bpy.types.Operator):
	bl_idname = "object.contrun"
	bl_label = "test button"

	def execute(self,context):
		print("contrun pressed")
		if bpy.app.timers.is_registered(loop.update):
			print("unregister loop.update")
			bpy.app.timers.unregister(loop.update)
		else:
			print("register loop.update")
			loop.selected_objects=context.selected_objects
			bpy.app.timers.register(loop.update)

		return {'FINISHED'}

class loop():
	tartarus=0
	selected_objects=None
	def update():
		global lib
		settings = get_settings(bpy.context)
		# print(settings.collision_mesh.name)
		for ob in loop.selected_objects:
			if(ob.type != "MESH"):
				continue
			if(ob.mode != "OBJECT"):
				return False
				# bpy.app.timers.unregister(loop.update)
			addkeysifneeded(ob)
			shape=ob.data.shape_keys.key_blocks['cloth shape']
			origshape=ob.data.shape_keys.key_blocks['Basis']
			if(settings.use_c):
				shape_ptr=cast(shape.as_pointer(),POINTER(c_int))
				origshape_ptr=cast(origshape.as_pointer(),POINTER(c_int))
				lib.dothewave(origshape_ptr,shape_ptr,loop.tartarus)
			else:
				# allocate shit
				shapedata=[0]*len(origshape.data)*3 # numverts*3
				origshape.data.foreach_get("co",shapedata)
				for i in range(0, int(len(shapedata)),3):
					shapedata[i]=shapedata[i]+sin(loop.tartarus*0.5+shapedata[i+2]*2.5)
					shapedata[i+1]=shapedata[i+1]+cos(loop.tartarus*0.2+shapedata[i+2]*1)
	
				shape.data.foreach_set("co",shapedata)
			shape.value=shape.value

		loop.tartarus+=1
		return 0.0166

def register():
	global lib
	bpy.utils.register_class(PANEL_PT_fCloth)
	bpy.utils.register_class(test)
	bpy.utils.register_class(reset)
	bpy.utils.register_class(contrun)
	bpy.utils.register_class(FClothSettings)
	lib = CDLL(libfile)
	print("scriptdir" +os.path.dirname(__file__))
	path=os.path.dirname(__file__) + "/build"
	if (platform.system() == "Linux"):
		lib.init(path.encode('ascii'))
		lib.loadLibrary()
	lib.printjunk(551)
	bpy.types.Scene.fcloth_settings = bpy.props.PointerProperty(type=FClothSettings)


def unregister():
	global lib
	bpy.utils.unregister_class(PANEL_PT_fCloth)
	bpy.utils.unregister_class(test)
	bpy.utils.unregister_class(reset)
	bpy.utils.unregister_class(contrun)
	bpy.utils.unregister_class(FClothSettings)
	if bpy.app.timers.is_registered(loop.update):
		print("unregister loop.update")
		bpy.app.timers.unregister(loop.update)
	del bpy.types.Scene.fcloth_settings
	reload_lib()

def is_loaded(lib):
	libp = os.path.abspath(lib)
	ret = os.system("lsof -p %d | grep %s > /dev/null" % (os.getpid(), libp))
	return (ret == 0)

def reload_lib():
	global lib
	lib.loadLibrary()

if __name__ == "__main__":
	register()
