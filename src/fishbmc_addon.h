/*
 *  fishBMC visualization plugin
 *
 *  Copyright (C) 2005-2019 Team Kodi
 *  Copyright (C) 2012 Marcel Ebmer
 *  This file is part of Kodi - https://kodi.tv
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Kodi; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "fische.h"

#include <kodi/addon-instance/Visualization.h>
#include <kodi/gui/gl/Shader.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>

struct sPosition
{
  sPosition() : x(0.0f), y(0.0f), z(1.0f), u(1.0f) {}
  sPosition(float* d) : x(d[0]), y(d[1]), z(d[2]), u(1.0f) {}
  sPosition(float x, float y, float z = 0.0f) : x(x), y(y), z(z), u(1.0f) {}
  float x,y,z,u;
};

struct sCoord
{
  sCoord() : s(0.0f), t(0.0f) {}
  sCoord(float s, float t) : s(s), t(t) {}
  float s,t;
};

class ATTRIBUTE_HIDDEN CVisualizationFishBMC
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceVisualization,
    public kodi::gui::gl::CShaderProgram
{
public:
  CVisualizationFishBMC();
  virtual ~CVisualizationFishBMC();

  virtual ADDON_STATUS GetStatus() override;
  virtual bool Start(int channels, int samplesPerSec, int bitsPerSample, std::string songName) override;
  virtual void Stop() override;
  virtual void Render() override;
  virtual void AudioData(const float* audioData, int audioDataLength, float *freqData, int freqDataLength) override;
  virtual ADDON_STATUS SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue) override;

  void OnCompiledAndLinked() override;
  bool OnEnabled() override;

private:
  void start_render();
  void finish_render();
  void textured_quad (float center_x,
                      float center_y,
                      float angle,
                      float axis,
                      float width,
                      float height,
                      float tex_left,
                      float tex_right,
                      float tex_top,
                      float tex_bottom);
  static void on_beat(void* handler, double frames_per_beat);
  static void write_vectors(void* handler, const void* data, size_t bytes);
  static size_t read_vectors(void* handler, void** data);
  void delete_vectors();

  bool m_startOK = false;
  bool m_shaderLoaded = false;

  glm::mat4 m_projMatrix;
  glm::mat4 m_modelMatrix;

  sPosition m_vertex[4];
  sCoord m_coord[4];
  GLuint m_indexer[4] = {0, 1, 3, 2};

  GLint m_uProjMatrixLoc = -1;
  GLint m_uModelViewMatrixLoc = -1;
  GLint m_aVertexLoc = -1;
  GLint m_aCoordLoc = -1;

  GLuint m_vertexVBO[2] = {0};
  GLuint m_indexVBO = 0;
  GLuint m_texture = 0;

  FISCHE* m_fische = nullptr;
  float m_aspect;
  bool m_isrotating;
  float m_angle;
  float m_lastangle;
  bool m_errorstate;
  int m_framedivisor;
  float m_angleincrement;
  float m_texright;
  float m_texleft;
  bool m_filemode;
  int m_size;
  uint8_t* m_axis = nullptr;
};

