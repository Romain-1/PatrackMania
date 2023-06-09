#include <GL/gl3w.h>

#include "Engine.h"

#include <chrono>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Ressources/Data/Shader.h"
#include <glm/gtc/type_ptr.hpp>

void MouseDragging(Viewer& viewer, glm::vec2& lastMouse);

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;
Shader* TextShader;
unsigned int VAO, VBO;

void RenderText(
    Shader* s,
    std::string text,
    float x,
    float y,
    float scale,
    glm::vec3 color
)
{
	glm::vec2 dimensions = Engine::Graphics->GetWindowSize();
	glm::mat4 projection = glm::ortho(0.0f, dimensions.x, dimensions.y, 0.0f);//Engine::Graphics->GetCameraProjection();
    // activate corresponding render state	
    s->use();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUniform3f(s->uniform("textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv( s->uniform("projection"), 1, GL_FALSE, glm::value_ptr( projection ) );
	glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
	Character tallest = Characters[text[0]];
	for (auto c : text) {
		if (Characters[c].Size.y > tallest.Size.y) tallest = Characters[c];
	}

    for (auto c : text)
    {
        Character ch = Characters[c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y + (tallest.Size.y - ch.Size.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,		ypos,		0.0f, 0.0f },
            { xpos,		ypos + h,	0.0f, 1.0f },
            { xpos + w,	ypos + h,	1.0f, 1.0f },

            { xpos,		ypos,		0.0f, 0.0f },
            { xpos + w,	ypos + h,	1.0f, 1.0f },
            { xpos + w,	ypos,		1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void InitText()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		throw new std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");
	}

	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/Ubuntu-Regular.ttf", 0, &face))
	{
		throw new std::runtime_error("ERROR::FREETYPE: Failed to load font");
	}
	FT_Set_Pixel_Sizes(face, 0, 20);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Console.Log("ERROR::FREETYTPE: Failed to load Glyph");
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RED,
			face->glyph->bitmap.width, face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			.TextureID = texture,
			.Size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			.Bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			.Advance = (unsigned int)face->glyph->advance.x
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	TextShader = new Shader();
	TextShader->initFromFiles("assets/shaders/TextShader.vert", "assets/shaders/TextShader.frag");

	TextShader->addUniform("textColor");
	TextShader->addUniform("projection");
}

void Engine::Run()
{
	InitText();
	m_running = true;

	double deltaTime = 0, time = 0;
	glm::vec2 lastMouse(0, 0);
	while (m_running && !Graphics->WindowShouldClose())
	{
		//Console.Log(__func__, "loop");

		RenderText(TextShader, "Track 1", 100, 100, 1, glm::vec3(255, 255, 255));
		auto start = std::chrono::system_clock::now();

		for (auto module : m_addedModules) { module->Init(); m_modules.push_back(module); }
		m_addedModules.clear();

		for (auto module : m_modules) { module->Update(deltaTime, m_gameObjects); }
		for (auto obj : m_gameObjects) {
			obj->ApplyOnHierarchy([deltaTime](GameObject* go) {
				if (go->activeInHierarchy) go->Update(deltaTime);
			});
		}

		if (Keyboard::GetKeyState(GLFW_KEY_F2) == JUST_PRESSED) {
			debug = !debug;
			Console.Log("Debug mode", debug?"on":"off");
		}
		//MouseDragging(*Graphics->m_viewer, lastMouse);

		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		deltaTime = elapsed_seconds.count();
		time += deltaTime;
	}
}

void Engine::Stop() { m_running = false; }
