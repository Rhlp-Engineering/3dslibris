# Gpuprimitiveengineandshaders

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpuprimitiveengineandshaders

3DS GPU Primitive Engine and Shaders 
| |
| --- |

 The 3DS seems to have four "shaders", each with there own I/O ports. However, the vertex shaders seem to be used in parallel (to process 3 or 4 vertices at once... so it's unknown if one needs to initialize all sets of I/O ports separately; 3dbrew seems to have only two shaders documented (GSH and VSH), without even being aware of the VSH2 and VSH3 registers.
 
 **Vertex Shader (VSH)**
 The Vertex Shader is used to forward incoming vertex data (coordinates, colors, texcoords) to the drawing engine. Most commonly, the shader would be used to multiply the incoming coordinates with a Position Matrix and/or Projection Matrix.
 
 **Geometry Shader (GSH)**
 The Geometry Shader can be used to do something... but it appears to be unknown what, and how.
 It can probably somehow receive incoming values from somewhere. And probably output values to somewhere (maybe somehow forwarding them to other GPU registers, and maybe somehow sending them as response to the CPU).
 The "EMIT" opcode might output data from Geometry Shader to Vertex shader?
 The "Primitive Engine" section below might refer to the geometry shader?
 
 **Primitive Engine**
 Primitive Engine (PE) is one of the PICA200's four vertex processor units and provides some unique features which are used to implement a geometry shader stage and variable-size primitive rendering.
 The full functionality of PE is not yet understood and remains to be reverse-engineered.
 Variable-size primitives are implemented by prefixing each per-primitive sequence of indices in an index array with a primitive size. This is used for various effects, for example Catmull-Clark subdivision and Loop subdivision. It is unknown how this feature is enabled specifically.