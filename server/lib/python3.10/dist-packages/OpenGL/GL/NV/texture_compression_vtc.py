'''OpenGL extension NV.texture_compression_vtc

This module customises the behaviour of the 
OpenGL.raw.GL.NV.texture_compression_vtc to provide a more 
Python-friendly API

Overview (from the spec)
	
	This extension adds support for the VTC 3D texture compression
	formats, which are analogous to the S3TC texture compression formats,
	with the addition of some retiling in the Z direction.  VTC has the
	same compression ratio as S3TC and uses 4x4x1, 4x4x2, (4x4x3 when
	non-power-of-two textures are supported), or 4x4x4 blocks.

The official definition of this extension is available here:
http://www.opengl.org/registry/specs/NV/texture_compression_vtc.txt
'''
from OpenGL import platform, constant, arrays
from OpenGL import extensions, wrapper
import ctypes
from OpenGL.raw.GL import _types, _glgets
from OpenGL.raw.GL.NV.texture_compression_vtc import *
from OpenGL.raw.GL.NV.texture_compression_vtc import _EXTENSION_NAME

def glInitTextureCompressionVtcNV():
    '''Return boolean indicating whether this extension is available'''
    from OpenGL import extensions
    return extensions.hasGLExtension( _EXTENSION_NAME )


### END AUTOGENERATED SECTION