#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#pragma once

typedef struct gpu_buffer gpu_buffer;
typedef struct gpu_texture gpu_texture;
typedef struct gpu_canvas gpu_canvas;
typedef struct gpu_shader gpu_shader;
typedef struct gpu_pipeline gpu_pipeline;

// Buffer

typedef enum {
  GPU_BUFFER_USAGE_VERTEX  = (1 << 0),
  GPU_BUFFER_USAGE_INDEX   = (1 << 1),
  GPU_BUFFER_USAGE_UNIFORM = (1 << 2),
  GPU_BUFFER_USAGE_COMPUTE = (1 << 3),
  GPU_BUFFER_USAGE_COPY    = (1 << 4),
  GPU_BUFFER_USAGE_PASTE   = (1 << 5)
} gpu_buffer_usage;

typedef struct {
  uint64_t size;
  uint32_t usage;
  void* data;
  const char* label;
} gpu_buffer_info;

size_t gpu_sizeof_buffer(void);
bool gpu_buffer_init(gpu_buffer* buffer, gpu_buffer_info* info);
void gpu_buffer_destroy(gpu_buffer* buffer);
uint8_t* gpu_buffer_map(gpu_buffer* buffer, uint64_t offset, uint64_t size);
void gpu_buffer_discard(gpu_buffer* buffer);

// Texture

typedef enum {
  GPU_TEXTURE_USAGE_SAMPLE  = (1 << 0),
  GPU_TEXTURE_USAGE_CANVAS  = (1 << 1),
  GPU_TEXTURE_USAGE_COMPUTE = (1 << 2),
  GPU_TEXTURE_USAGE_COPY    = (1 << 3),
  GPU_TEXTURE_USAGE_PASTE   = (1 << 4)
} gpu_texture_usage;

typedef enum {
  GPU_TEXTURE_TYPE_2D,
  GPU_TEXTURE_TYPE_3D,
  GPU_TEXTURE_TYPE_CUBE,
  GPU_TEXTURE_TYPE_ARRAY
} gpu_texture_type;

typedef enum {
  GPU_TEXTURE_FORMAT_RGBA8,
  GPU_TEXTURE_FORMAT_RGBA4,
  GPU_TEXTURE_FORMAT_R16F,
  GPU_TEXTURE_FORMAT_RG16F,
  GPU_TEXTURE_FORMAT_RGBA16F,
  GPU_TEXTURE_FORMAT_R32F,
  GPU_TEXTURE_FORMAT_RG32F,
  GPU_TEXTURE_FORMAT_RGBA32F,
  GPU_TEXTURE_FORMAT_RGB10A2,
  GPU_TEXTURE_FORMAT_RG11B10F,
  GPU_TEXTURE_FORMAT_D16,
  GPU_TEXTURE_FORMAT_D32F,
  GPU_TEXTURE_FORMAT_D24S8
} gpu_texture_format;

typedef struct {
  gpu_texture* source;
  gpu_texture_type type;
  gpu_texture_format format;
  uint32_t baseMipmap;
  uint32_t mipmapCount;
  uint32_t baseLayer;
  uint32_t layerCount;
} gpu_texture_view_info;

typedef struct {
  gpu_texture_type type;
  gpu_texture_format format;
  uint32_t size[3];
  uint32_t layers;
  uint32_t mipmaps;
  uint32_t usage;
  const char* label;
} gpu_texture_info;

size_t gpu_sizeof_texture(void);
bool gpu_texture_init(gpu_texture* texture, gpu_texture_info* info);
bool gpu_texture_init_view(gpu_texture* texture, gpu_texture_view_info* info);
void gpu_texture_destroy(gpu_texture* texture);
void gpu_texture_write(gpu_texture* texture, uint8_t* data, uint16_t offset[4], uint16_t extent[4], uint16_t mip);

// Canvas

typedef enum {
  GPU_LOAD_OP_LOAD,
  GPU_LOAD_OP_CLEAR,
  GPU_LOAD_OP_DISCARD
} gpu_load_op;

typedef struct {
  gpu_texture* texture;
  uint32_t layer;
  uint32_t level;
  gpu_load_op load;
  float clear[4];
} gpu_color_attachment;

typedef struct {
  gpu_texture* texture;
  gpu_load_op load;
  float clear;
  struct {
    gpu_load_op load;
    uint8_t clear;
  } stencil;
} gpu_depth_attachment;

typedef struct {
  gpu_color_attachment color[4];
  gpu_depth_attachment depth;
  uint32_t size[2];
  uint32_t views;
  uint32_t msaa;
  const char* label;
} gpu_canvas_info;

size_t gpu_sizeof_canvas(void);
bool gpu_canvas_init(gpu_canvas* canvas, gpu_canvas_info* info);
void gpu_canvas_destroy(gpu_canvas* canvas);

// Shader

typedef struct {
  const void* code;
  size_t size;
  const char* entry;
} gpu_shader_source;

typedef struct {
  gpu_shader_source vertex;
  gpu_shader_source fragment;
  gpu_shader_source compute;
  const char* label;
} gpu_shader_info;

size_t gpu_sizeof_shader(void);
bool gpu_shader_init(gpu_shader* shader, gpu_shader_info* info);
void gpu_shader_destroy(gpu_shader* shader);

// Pipeline

typedef enum {
  GPU_FLOAT_F32,
  GPU_VEC2_F32,
  GPU_VEC2_F16,
  GPU_VEC2_U16N,
  GPU_VEC2_I16N,
  GPU_VEC3_F32,
  GPU_VEC4_F32,
  GPU_VEC4_F16,
  GPU_VEC4_U16N,
  GPU_VEC4_I16N,
  GPU_VEC4_U8N,
  GPU_VEC4_I8N,
  GPU_UINT_U32,
  GPU_UVEC2_U32,
  GPU_UVEC3_U32,
  GPU_UVEC4_U32,
  GPU_INT_I32,
  GPU_IVEC2_I32,
  GPU_IVEC3_I32,
  GPU_IVEC4_I32
} gpu_attribute_format;

typedef struct {
  uint8_t location;
  uint8_t buffer;
  uint8_t format;
  uint8_t offset;
} gpu_attribute;

typedef struct {
  uint16_t stride;
  uint16_t divisor;
} gpu_buffer_layout;

typedef enum {
  GPU_DRAW_POINTS,
  GPU_DRAW_LINES,
  GPU_DRAW_LINE_STRIP,
  GPU_DRAW_TRIANGLES,
  GPU_DRAW_TRIANGLE_STRIP
} gpu_draw_mode;

typedef enum {
  GPU_INDEX_U16,
  GPU_INDEX_U32
} gpu_index_stride;

typedef enum {
  GPU_CULL_NONE,
  GPU_CULL_FRONT,
  GPU_CULL_BACK
} gpu_cull_mode;

typedef enum {
  GPU_WINDING_CCW,
  GPU_WINDING_CW
} gpu_winding;

typedef enum {
  GPU_COMPARE_NONE,
  GPU_COMPARE_EQUAL,
  GPU_COMPARE_NEQUAL,
  GPU_COMPARE_LESS,
  GPU_COMPARE_LEQUAL,
  GPU_COMPARE_GREATER,
  GPU_COMPARE_GEQUAL
} gpu_compare_mode;

typedef enum {
  GPU_STENCIL_KEEP,
  GPU_STENCIL_ZERO,
  GPU_STENCIL_REPLACE,
  GPU_STENCIL_INCREMENT,
  GPU_STENCIL_DECREMENT,
  GPU_STENCIL_INCREMENT_WRAP,
  GPU_STENCIL_DECREMENT_WRAP,
  GPU_STENCIL_INVERT
} gpu_stencil_action;

typedef struct {
  gpu_stencil_action fail;
  gpu_stencil_action depthFail;
  gpu_stencil_action pass;
  gpu_compare_mode test;
  uint8_t readMask;
  uint8_t writeMask;
  uint8_t reference;
} gpu_stencil_state;

typedef enum {
  GPU_BLEND_ZERO,
  GPU_BLEND_ONE,
  GPU_BLEND_SRC_COLOR,
  GPU_BLEND_ONE_MINUS_SRC_COLOR,
  GPU_BLEND_SRC_ALPHA,
  GPU_BLEND_ONE_MINUS_SRC_ALPHA,
  GPU_BLEND_DST_COLOR,
  GPU_BLEND_ONE_MINUS_DST_COLOR,
  GPU_BLEND_DST_ALPHA,
  GPU_BLEND_ONE_MINUS_DST_ALPHA
} gpu_blend_factor;

typedef enum {
  GPU_BLEND_ADD,
  GPU_BLEND_SUB,
  GPU_BLEND_RSUB,
  GPU_BLEND_MIN,
  GPU_BLEND_MAX
} gpu_blend_op;

typedef struct {
  struct {
    gpu_blend_factor src;
    gpu_blend_factor dst;
    gpu_blend_op op;
  } color, alpha;
  bool enabled;
} gpu_blend_state;

typedef struct {
  gpu_shader* shader;
  gpu_canvas* canvas;
  gpu_buffer_layout buffers[16];
  gpu_attribute attributes[16];
  gpu_index_stride indexStride;
  gpu_draw_mode drawMode;
  gpu_cull_mode cullMode;
  gpu_winding winding;
  float depthOffset;
  float depthOffsetSloped;
  bool depthWrite;
  gpu_compare_mode depthTest;
  gpu_stencil_state stencilFront;
  gpu_stencil_state stencilBack;
  bool alphaToCoverage;
  uint8_t colorMask;
  gpu_blend_state blend;
  const char* label;
} gpu_pipeline_info;

size_t gpu_sizeof_pipeline(void);
bool gpu_pipeline_init(gpu_pipeline* pipeline, gpu_pipeline_info* info);
void gpu_pipeline_destroy(gpu_pipeline* pipeline);

// Entry

typedef struct {
  bool debug;
  void* (*getProcAddress)(const char*);
  void (*callback)(void* context, const char* message, int level);
  void* context;
} gpu_config;

typedef struct {
  bool anisotropy;
  bool astc;
  bool dxt;
} gpu_features;

typedef struct {
  uint32_t textureSize;
  uint32_t framebufferSize[2];
  uint32_t framebufferSamples;
} gpu_limits;

bool gpu_init(gpu_config* config);
void gpu_destroy(void);
void gpu_frame_wait(void);
void gpu_frame_finish(void);
void gpu_render_begin(gpu_canvas* canvas);
void gpu_render_finish(void);
void gpu_set_pipeline(gpu_pipeline* pipeline);
void gpu_set_vertex_buffers(gpu_buffer** buffers, uint64_t* offsets, uint32_t count);
void gpu_set_index_buffer(gpu_buffer* buffer, uint64_t offset);
void gpu_draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex);
void gpu_draw_indexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, uint32_t baseVertex);
void gpu_draw_indirect(gpu_buffer* buffer, uint64_t offset, uint32_t drawCount);
void gpu_draw_indirect_indexed(gpu_buffer* buffer, uint64_t offset, uint32_t drawCount);
void gpu_compute(gpu_shader* shader, uint32_t x, uint32_t y, uint32_t z);
void gpu_get_features(gpu_features* features);
void gpu_get_limits(gpu_limits* limits);
