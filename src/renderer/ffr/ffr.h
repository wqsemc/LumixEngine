#pragma once

#include "engine/lumix.h"


namespace Lumix {

struct IAllocator;

namespace ffr {


struct BufferHandle { u32 value; bool isValid() const { return value != 0xFFffFFff; } };
struct ProgramHandle { u32 value; bool isValid() const { return value != 0xFFffFFff; } };
struct TextureHandle { u32 value; bool isValid() const { return value != 0xFFffFFff; } };
struct QueryHandle { u32 value; bool isValid() const { return value != 0xFFffFFff; } };

const BufferHandle INVALID_BUFFER = { 0xffFFffFF };
const ProgramHandle INVALID_PROGRAM = { 0xffFFffFF };
const TextureHandle INVALID_TEXTURE = { 0xffFFffFF };
const QueryHandle INVALID_QUERY = { 0xffFFffFF };


enum class LogLevel : u32 {
	INFO,
	WARNING,
	ERROR,
	FATAL
};

enum class Backend {
	OPENGL,
	DX11
};

enum class FramebufferFlags : u32 {
	SRGB = 1 << 0,
	READONLY_DEPTH_STENCIL = 1 << 1
};

enum class StateFlags : u64 {
	WIREFRAME = 1 << 0,
	DEPTH_TEST = 1 << 1,
	CULL_FRONT = 1 << 2,
	CULL_BACK = 1 << 3,
	SCISSOR_TEST = 1 << 4,
	DEPTH_WRITE = 1 << 5,
	/* 16 bits reserved for blending*/
	/* 40 bits reserver for stencil*/ 
};


enum class PrimitiveType : u32 {
	TRIANGLES,
	TRIANGLE_STRIP,
	LINES,
	POINTS
};


enum class ShaderType : u32 {
	VERTEX,
	FRAGMENT,
	GEOMETRY
};


enum class ClearFlags : u32 {
	COLOR = 1 << 0,
	DEPTH = 1 << 1,
	STENCIL = 1 << 2
};


enum class StencilFuncs : u8 {
	DISABLE,
	ALWAYS,
	EQUAL,
	NOT_EQUAL,
};

enum class StencilOps : u8 {
	KEEP,
	ZERO,
	REPLACE,
	INCR,
	INCR_WRAP,
	DECR,
	DECR_WRAP,
	INVERT
};


enum class BlendFactors : u8 {
	ZERO,
	ONE,
	SRC_COLOR,
	ONE_MINUS_SRC_COLOR,
	SRC_ALPHA,
	ONE_MINUS_SRC_ALPHA,
	DST_COLOR,
	ONE_MINUS_DST_COLOR,
	DST_ALPHA,
	ONE_MINUS_DST_ALPHA,
};


enum class AttributeType : u8 {
	U8,
	FLOAT,
	I16
};


enum class TextureFormat : u32 {
	R8,
	D32,
	D24,
	D24S8,
	RGBA8,
	RGBA16,
	RGBA16F,
	R16F,
	R16,
	R32F,
	SRGB,
	SRGBA
};


enum class TextureFlags : u32 {
	SRGB = 1 << 0,
	CLAMP_U = 1 << 1,
	CLAMP_V = 1 << 2,
	CLAMP_W = 1 << 3,
	NO_MIPS = 1 << 4,
	POINT_FILTER = 1 << 5,
};

enum class BufferFlags : u32 {
	IMMUTABLE = 1 << 0,
	UNIFORM_BUFFER = 1 << 1
};

enum class DataType {
	U16,
	U32
};

#pragma pack(1)
struct Attribute {
	enum Flags {
		NORMALIZED = 1 << 0,
		AS_INT = 1 << 1,
		INSTANCED = 1 << 2
	};
	u8 idx;
	u8 components_count;
	u8 byte_offset;
	AttributeType type;
	u8 flags;
};
#pragma pack()

struct VertexDecl {
	enum { MAX_ATTRIBUTES = 16 };

	void addAttribute(u8 idx, u8 byte_offset, u8 components_num, AttributeType type, u8 flags);

	u8 attributes_count = 0;
	u32 hash = 0;
	Attribute attributes[MAX_ATTRIBUTES];
};


struct TextureInfo {
	int width;
	int height;
	int depth;
	int layers;
	int mips;
	bool is_cubemap;
};


void preinit(IAllocator& allocator);
bool init(void* window_handle, bool debug);
Backend getBackend();
void swapBuffers(u32 w, u32 h);
bool isHomogenousDepth();
bool isOriginBottomLeft();
void checkThread();
void shutdown();
void startCapture();
void stopCapture();
int getSize(AttributeType type);


void clear(u32 flags, const float* color, float depth);

void scissor(u32 x, u32 y, u32 w, u32 h);
void viewport(u32 x, u32 y, u32 w, u32 h);

inline u64 getBlendStateBits(BlendFactors src_rgb, BlendFactors dst_rgb, BlendFactors src_a, BlendFactors dst_a)
{
	return (((u64)src_rgb & 15) << 6) | (((u64)dst_rgb & 15) << 10) | (((u64)src_a & 15) << 14) | (((u64)dst_a & 15) << 18);
}

inline u64 getStencilStateBits(u8 write_mask, StencilFuncs func, u8 ref, u8 mask, StencilOps sfail, StencilOps dpfail, StencilOps dppass)
{
	return ((u64)write_mask << 22) | ((u64)func << 30) | ((u64)ref << 34) | ((u64)mask << 42) | ((u64)sfail << 50) | ((u64)dpfail << 54) | ((u64)dppass << 58);
}

TextureHandle allocTextureHandle();
BufferHandle allocBufferHandle();
ProgramHandle allocProgramHandle();

void setState(u64 state);
bool createProgram(ProgramHandle program, const VertexDecl& decl, const char** srcs, const ShaderType* types, int num, const char** prefixes, int prefixes_count, const char* name);
void useProgram(ProgramHandle prg);

void createBuffer(BufferHandle handle, u32 flags, size_t size, const void* data);
bool createTexture(TextureHandle handle, u32 w, u32 h, u32 depth, TextureFormat format, u32 flags, const void* data, const char* debug_name);
void createTextureView(TextureHandle view, TextureHandle texture);
bool loadTexture(TextureHandle handle, const void* data, int size, u32 flags, const char* debug_name);
void update(TextureHandle texture, u32 level, u32 x, u32 y, u32 w, u32 h, TextureFormat format, void* buf);
QueryHandle createQuery();

void bindVertexBuffer(u32 binding_idx, BufferHandle buffer, u32 buffer_offset, u32 stride_offset);
void bindTextures(const TextureHandle* handles, u32 offset, u32 count);
void update(BufferHandle buffer, const void* data, size_t size);
void* map(BufferHandle buffer, size_t size);
void unmap(BufferHandle buffer);
void bindUniformBuffer(u32 index, BufferHandle buffer, size_t offset, size_t size);
void getTextureImage(ffr::TextureHandle texture, u32 size, void* buf);
TextureInfo getTextureInfo(const void* data);
void queryTimestamp(QueryHandle query);
u64 getQueryResult(QueryHandle query);
u64 getQueryFrequency();
bool isQueryReady(QueryHandle query);
void generateMipmaps(TextureHandle texture);

void destroy(ProgramHandle program);
void destroy(BufferHandle buffer);
void destroy(TextureHandle texture);
void destroy(QueryHandle query);

void bindIndexBuffer(BufferHandle handle);
void drawTriangles(u32 indices_count, DataType index_type);
void drawTrianglesInstanced(u32 indices_count, u32 instances_count, DataType index_type);
void drawElements(u32 byte_offset, u32 count, PrimitiveType primitive_type, DataType index_type);
void drawArrays(u32 offset, u32 count, PrimitiveType type);
void drawTriangleStripArraysInstanced(u32 indices_count, u32 instances_count);

void pushDebugGroup(const char* msg);
void popDebugGroup();

void setFramebuffer(TextureHandle* attachments, u32 num, u32 flags);


} // namespace ffr

} // namespace Lumix