#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glad/gles2.h>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

static SDL_Window* mainWindow = NULL;
static SDL_GLContext context;
int window_width = 800;
int window_height = 600;

float mouseRelative[2];
float mouseAbsolute[2];
bool isGrabbed = false;

void GrabCursor(SDL_Event* event)
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoKeyboard | ImGuiConfigFlags_NoMouse;
    SDL_SetWindowRelativeMouseMode(SDL_GetWindowFromEvent(event), true);
    isGrabbed = true;
}

void ReleaseCursor(SDL_Event* event)
{
	ImGui::GetIO().ConfigFlags &= ~(ImGuiConfigFlags_NoKeyboard | ImGuiConfigFlags_NoMouse);
    SDL_SetWindowRelativeMouseMode(SDL_GetWindowFromEvent(event), false);
    isGrabbed = false;
}
     
void InitImGui()
{
	SDL_Log("Initializing ImGui...");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |=
		ImGuiConfigFlags_NavEnableKeyboard |
		ImGuiConfigFlags_NavEnableGamepad;

	ImGui_ImplSDL3_InitForOpenGL(mainWindow, context);
	ImGui_ImplOpenGL3_Init("#version 300 es");
	SDL_Log("ImGui initialized.");
}

SDL_AppResult SDL_AppInit(void **, int, char**)
{
    // Create SDL window
    if (!(mainWindow = SDL_CreateWindow("SDL Mouse Test", window_width, window_height, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    ))) {
        SDL_Log("Couldn't create window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
	// Create OpenGL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	if (!(context = SDL_GL_CreateContext(mainWindow)))
	{
		const char* contextError = SDL_GetError();
		SDL_Log("Failed to create OpenGL context: \n%s", contextError);
		return SDL_APP_FAILURE;
	}
	SDL_GL_MakeCurrent(mainWindow, context);

	// Initialize GLAD
	if (!gladLoadGLES2((GLADloadfunc)SDL_GL_GetProcAddress))
	{
		SDL_Log("Failed to initialize GLAD");
		return SDL_APP_FAILURE;
	}
	// Initialize OpenGL Context
	glViewport(0, 0, window_width, window_height);

	InitImGui();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event *event)
{
	ImGui_ImplSDL3_ProcessEvent(event);

	// Quit requests
	if (event->type == SDL_EVENT_QUIT)
		return SDL_APP_SUCCESS; //Exit
	
	// Window resize
	if (event->type == SDL_EVENT_WINDOW_RESIZED)
	{
		SDL_GetWindowSizeInPixels(mainWindow, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
	}

    // Track absolute mouse movements
    if (event->type == SDL_EVENT_MOUSE_MOTION)
    {
        SDL_MouseMotionEvent* mouseEvent = (SDL_MouseMotionEvent*)event;
        mouseAbsolute[0] = mouseEvent->x;
        mouseAbsolute[1] = mouseEvent->y;
    }
    
    // Release grab
    if (event->key.key == SDLK_ESCAPE)
	{
		ReleaseCursor(event);
	}

    // Grab cursor and track relative movements
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
			GrabCursor(event);
		
		if (isGrabbed)
		{
			if (event->type == SDL_EVENT_MOUSE_MOTION)
            {
                SDL_MouseMotionEvent* mouseEvent = (SDL_MouseMotionEvent*)event;
                mouseRelative[0] = mouseEvent->xrel;
                mouseRelative[1] = mouseEvent->yrel;
            }
		}
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void*)
{
    // Initialize ImGui for this frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

    // Gui Window
    if (ImGui::Begin("Cursor Info"))
    {
        ImGui::Text("Absolute: (%0.1f, %0.1f)", mouseAbsolute[0], mouseAbsolute[1]);
        ImGui::Text("Relative: (%0.1f, %0.1f)", mouseRelative[0], mouseRelative[1]);
        ImGui::Text("Cursor Grabbed: %s", isGrabbed ? "Yes" : "No");
    }
    ImGui::End();

    // Render Gui
	ImGui::GetIO().FontGlobalScale = SDL_GetWindowDisplayScale(mainWindow);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Present framebuffer
	SDL_GL_SwapWindow(mainWindow);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void*, SDL_AppResult)
{

}
