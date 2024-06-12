'''OpenGL extension ANGLE.depth_texture

This module customises the behaviour of the 
OpenGL.raw.GLES2.ANGLE.depth_texture to provide a more 
Python-friendly API

Overview (from the spec)
	
	This extension defines support for 2D depth and depth-stencil
	textures in an OpenGL ES implementation.
	
	This extension incorporates the depth texturing functionality of
	OES_depth_texture and OES_packed_depth_stencil, but does not
	provide the ability to load existing data via TexImage2D or
	TexSubImage2D. This extension also allows implementation
	variability in which components from a sampled depth texture
	contain the depth data. Depth textures created with this
	extension only support 1 level.

The official definition of this extension is available here:
http://www.opengl.org/registry/specs/ANGLE/depth_texture.txt
'''
from OpenGL import platform, constant, arrays
from OpenGL import extensions, wrapper
import ctypes
from OpenGL.raw.GLES2 import _types, _glgets
from OpenGL.raw.GLES2.ANGLE.depth_texture import *
from OpenGL.raw.GLES2.ANGLE.depth_texture import _EXTENSION_NAME

def glInitDepthTextureANGLE():
    '''Return boolean indicating whether this extension is available'''
    from OpenGL import extensions
    return extensions.hasGLExtension( _EXTENSION_NAME )


### END AUTOGENERATED SECTION