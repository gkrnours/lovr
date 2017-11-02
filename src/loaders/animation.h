#include "lib/vec/vec.h"
#include "lib/map/map.h"

#pragma once

typedef struct {
  double time;
  float data[4];
} Keyframe;

typedef vec_t(Keyframe) vec_keyframe_t;

typedef struct {
  const char* node;
  vec_keyframe_t positionKeyframes;
  vec_keyframe_t rotationKeyframes;
  vec_keyframe_t scaleKeyframes;
} AnimationChannel;

typedef map_t(AnimationChannel) map_channel_t;

typedef struct {
  const char* name;
  map_channel_t channels;
  int channelCount;
} AnimationData;

AnimationData* lovrAnimationDataCreate(const char* name);
void lovrAnimationDataDestroy(AnimationData* animationData);