#include "shaders.h"
#include <chrono>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

float points[12]{
	0.,1.,
	1.,1.,
	0.,0.,
	0.,0.,
	1.,0.,
	1.,1.
};

int main(int argc, char** argv) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		std::cerr << "yall we done fucked up:\n\t" << SDL_GetError() << std::endl;
	}

	SDL_Window *window = SDL_CreateWindow("ShaderToy", 1000, 1000, SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GLContext context = SDL_GL_CreateContext(window);

	if (!context) {
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	// Tell ImGui you're using SDL3 + OpenGL
	ImGui_ImplSDL3_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 450 core");


	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		SDL_GL_DestroyContext(context);
		SDL_DestroyWindow(window);
		return -1;
	}

	int width, height;
	SDL_GetWindowSize(window, &width, &height);

	glViewport(0, 0, width, height);

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	std::string fSS;
	loadFragmentShader(fSS);
	
	GLuint VS = compileShader(GL_VERTEX_SHADER, vSS);
	GLuint FS = compileShader(GL_FRAGMENT_SHADER, fSS.c_str());
	GLuint program = linkShaderProgram(VS, FS);
	


	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	glDeleteShader(VS);
	glDeleteShader(FS);


	const bool *keys = SDL_GetKeyboardState(nullptr);

	float mX, mY;

	std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();

	double time = 0;
	size_t frame = 0;
	while (true) {
		std::chrono::time_point<std::chrono::high_resolution_clock> end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> dT = end - start;
		start = std::chrono::high_resolution_clock::now();

		time += dT.count();

		
		SDL_GetMouseState(&mX, &mY);

		bool quit = false;
		{
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				ImGui_ImplSDL3_ProcessEvent(&e);
				if (e.type == SDL_EVENT_QUIT) quit = true;
			}
		}
		if (keys[SDL_SCANCODE_ESCAPE]) quit = true;
		if (quit) break;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();


		ImGui::Begin("Info",  nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		if (isError) {
			ImGui::Text("Error:\n%s", error.c_str());
			ImGui::Text("Check shader.glsl for errors!");
		}
		ImGui::Text("FPS: %.1f", 1.0/dT.count());
		ImGui::End();


		ImGui::Render();



		glClearColor(0., 0., 0., 1.);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);

		//std::cout << "mouse x: " << mX << "\tmouse y: " << height - mY << '\n';

		glUniform2f(glGetUniformLocation(program, "uMouse"), mX, height - mY);
		glUniform2f(glGetUniformLocation(program, "uResolution"), (float) width, (float) height);
		

		glUniform1d(glGetUniformLocation(program, "uTime"), time);
		glUniform1d(glGetUniformLocation(program, "uTimeDelta"), dT.count());

		glUniform1ui(glGetUniformLocation(program, "uFrame"), frame++);




		glDrawArrays(GL_TRIANGLES, 0, sizeof(points) / (2 * sizeof(points[0])));

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		SDL_GL_SwapWindow(window);

	}


	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}