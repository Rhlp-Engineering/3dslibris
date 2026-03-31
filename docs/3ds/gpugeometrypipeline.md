# Gpugeometrypipeline

> Source: https://problemkaputt.de/gbatek.htm
> Section: Gpugeometrypipeline

3DS GPU Geometry Pipeline 
| |
| --- |

 **Array Attributes (via ARRAY pointers)**
 The most common way seems to be drawing via GPUREG_ATTR_BUF_xxx and GPUREG_ATTR_BUF_DRAWARRAYS, the GPU is then automatically reading vertex data from the array pointers (instead of needing to send all vertices to the command buffer).
 
 **Fixed Vertex Attributes (fixed attr for ALL vertices)**
 To use a fixed attribute, set the bit corresponding to the attribute in GPUREG_ATTR_BUF_FORMAT_HIGH and ensure that no vertex arrays are configured for the attribute (any configured arrays will override the fixed value, regardless of the bit setting). Even if a vertex array isn't being used for the attribute it still needs to be counted in the number of active attributes specified in the same register.
 
 **Immediate-Mode Vertex Submission (via GPUREG_FIXEDATTRIB_DATA)**
 Instead of using vertex arrays to supply vertex data, drawing can be done by directly writing vertex data to a register. This allows vertex data to be inlined directly in the command buffer. Since this is restricted to 4-component float data, it is more useful for small draws like UI elements or debug displays, to avoid using an unreasonable amount of memory and processing time appending the vertices to the command buffer.
 
 To use this feature, configure the number of attributes per vertex in GPUREG_VSH_NUM_ATTR. (All settings in the registers related to the vertex loader are ignored.) Then setup the GPU and shaders the same as if doing a regular draw call with GPUREG_ATTR_BUF_DRAWARRAYS or GPUREG_ATTR_BUF_DRAWELEMENTS, but instead of writing to either register, write the value 0xF to GPUREG_FIXEDATTRIB_INDEX and then follow by repeatedly writing vertex data to GPUREG_FIXEDATTRIB_DATA.
 
 Each set of writes to the 3 data registers specifies one attribute and all attributes (as configured in GPUREG_VSH_NUM_ATTR) need to be written, in order, to specify a vertex. Drawing happens automatically as vertices are specified. After finishing specifying vertices, follow with the same writes used after a draw arrays/elements.