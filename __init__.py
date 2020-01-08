#----------------------------------------------------------
# File __init__.py
#----------------------------------------------------------

#    Addon info
bl_info = {
    "name": "FCloth",
    "author": "Fredrik Hansson, email: ",
    "version": (1, 0),
    "blender": (2, 80, 0),
    "location": "View3D > Fredriks Tools > FCloth",
    "description": "",
    "warning": "made by a madman",
    "wiki_url": "",
    "category": '3D View'}

if "FCloth" in locals():
    import imp
    imp.reload(FCloth)
    print("Reloaded FCloth")
else:
    from . import FCloth
    print("Imported FCloth")


def register():
    FCloth.register()


def unregister():
    FCloth.unregister()


if __name__ == "__main__":
    register()
