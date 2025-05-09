/**
******************************************************************************
  * @file    FontRenderer.cppm
  * @author  Timicasto
  * @brief   Font renderer submodule in Renderer
  ******************************************************************************
  */

// SPDX-License-Identifier: AGPL-3.0-or-later

module;
#include <ft2build.h>
#include <glm/glm.hpp>

#include "glad/glad.h"

#include FT_FREETYPE_H

export module Renderer:FontRenderer;
import :Shader;
import std;
import Logger;

class Character {
public:
Character(const GLuint tex_id, const glm::ivec2 &size, const glm::ivec2 &bearing, GLuint advance)
		: texID(tex_id),
		size(size),
		bearing(bearing),
		advance(advance) {
	}
	GLuint texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	GLuint advance;
};

export class FontRenderer {
public:
	FontRenderer() : shader(vshader, fshader) {
		if (FT_Init_FreeType(&freetype)) {
			Logger::log(LogLevel::FATAL, "FontRenderer submodule initializing failed : Freetype Initialization failed");
		} else {
			Logger::log(LogLevel::INFO, "Freetype initialized.");
		}
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	size_t parseOpenType(const std::string& path, const size_t size) {
		FT_Face face;
		if (FT_New_Face(freetype, path.c_str(), 0, &face)) {
			Logger::log(LogLevel::FATAL, "Failed to read font file " + path);
		} else {
			Logger::log(LogLevel::INFO, "Font file " + path + " loaded, got face " + face->family_name + "(" + face->style_name + ")" + " and " + std::to_string(face->num_glyphs) + " glyphs.");
		}

		if (FT_HAS_FIXED_SIZES(face)) {
			Logger::log(LogLevel::WARNING, "Loaded font file contains fixed size glyphs, parsing and rendering may misbahve.");
		}
		if (FT_HAS_COLOR(face)) {
			Logger::log(LogLevel::WARNING, "Loaded font file contains color, parsing and rendering may misbehave.");
		}
		FT_Set_Pixel_Sizes(face, 0, size);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		std::vector<std::pair<wchar_t, wchar_t>> toLoad;

		std::unordered_map<wchar_t, Character> glyphs;

		std::uint32_t index;
		std::uint64_t charcode = FT_Get_First_Char(face, &index);

		while (index) {
			if (FT_Load_Char(face, index, FT_LOAD_RENDER)) {
				Logger::log(LogLevel::ERROR, "Failed to load glyph " + index);
			} else {
				Logger::log(LogLevel::DEBUG, "Loaded glyph: " + index);
			}

			GLuint tex;
			glGenTextures(1, &tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			auto ch = Character(tex, {face->glyph->bitmap.width, face->glyph->bitmap.rows}, {face->glyph->bitmap_left, face->glyph->bitmap_top}, face->glyph->advance.x);
			glyphs.insert({index, ch});

			charcode = FT_Get_Next_Char(face, charcode, &index);
		}

		fonts.emplace_back(glyphs);
		FT_Done_Face(face);
		return fonts.size() - 1;
	}

	void render(const size_t font, const std::wstring& text, float x, float y, const float scale, const glm::vec3 color, const glm::mat4 &projection) const {
		shader.use();
		shader.setVec3("color", color);
		shader.setMat4("projection", projection);

		for (wchar_t item : text) {
			const Character c = fonts[font].find(item)->second;
			const GLfloat xPos = x + c.bearing.x * scale;
			const GLfloat yPos = y - (c.size.y - c.bearing.y) * scale;
			const GLfloat w = c.size.x * scale;
			const GLfloat h = c.size.y * scale;

			GLfloat vertices[6][4] = {
				{xPos,     yPos + h, 0, 1},
				{xPos,     yPos,     0, 1},
				{xPos + w, yPos,     1, 1},
				{xPos,     yPos + h, 0, 0},
				{xPos + w, yPos,     1, 1},
				{xPos + w, yPos + h, 1, 0}
			};

			glBindTexture(GL_TEXTURE_2D, c.texID);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			x += (c.advance >> 6) * scale;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	~FontRenderer() {
		FT_Done_FreeType(freetype);
	}
private:
	std::string vshader = R"(
#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 texCoord;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
    texCoord = vertex.zw;
}
)";

	std::string fshader = R"(
#version 330 core

in vec2 texCoord;

out vec4 color;

uniform sampler2D texture;

uniform vec3 textColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture, texCoord).r);
    color = vec4(textColor, 1.0) * sampled;
}
)";

	GLuint vao{}, vbo{};
	Shader shader;
	FT_Library freetype{};
	std::vector<std::unordered_map<wchar_t, Character>> fonts;
};



