#include <SDL.h>
#include <SDL_ttf.h>
#include <spdlog/spdlog.h>

#include <argparse/argparse.hpp>
#include <cstring>
#include <string>

#include "ClearanceField.h"

using ClearanceField::BrushfireClearanceField;
using ClearanceField::IClearanceField;
using ClearanceField::inf;
using ClearanceField::ObstacleChecker;
using ClearanceField::TrueClearanceField;

const int GRID_SIZE = 32;
const int N = 100;

int GRID[N][N] = { 0 };

struct Options
{
	int w, h, u;
	int costUnit, diagonalCostUnit;
	int fontSize;
	// implementer:
	// 0 for TrueClearanceField
	// 1 for BrushfireClearanceField
	int implementer = 0;
};

// for i=0~9, get TextChars[i]
// for inf, get TextChars[10]
const char TextChars[12] = "0123456789-";

class Visualizer
{
public:
	Visualizer(const Options& options, IClearanceField* field);
	int	 Init();
	void Start();
	void Destroy();

private:
	const Options&	 options;
	IClearanceField* field = nullptr;

	SDL_Window*	  window;
	SDL_Renderer* renderer;
	TTF_Font*	  font;

	SDL_Texture* texture; // "0123456789-"

	// width,offset and height in x axis in texture for char i.
	int font_char_width[11];
	int font_char_offset[11];
	int font_char_height;

	int	 handleInputs();
	void draw();
	void reset();
	void handleInvertObstacle(int x, int y);
	void renderDigit(int x, int y, int d);
	void renderNumber(SDL_Rect& rect, int number);
};

// Parse options from command line.
// Returns 0 on success.
int ParseOptionsFromCommandline(int argc, char* argv[], Options& options);

int main(int argc, char* argv[])
{
	memset(GRID, 0, sizeof GRID);

	// Parse arguments.
	Options options;
	if (ParseOptionsFromCommandline(argc, argv, options) != 0)
		return -1;

	// Field
	ObstacleChecker isObstacle = [](int x, int y) { return GRID[y][x]; };

	IClearanceField* field = nullptr;
	if (options.implementer == 0)
	{
		field = new TrueClearanceField(options.w, options.h, options.u, options.costUnit,
			options.diagonalCostUnit, isObstacle);
		spdlog::info("using TrueClearanceField");
	}
	else
	{
		field = new BrushfireClearanceField(options.w, options.h, options.u, options.costUnit,
			options.diagonalCostUnit, isObstacle);
		spdlog::info("using BrushfireClearanceField");
	}

	// Visualizer
	Visualizer visualizer(options, field);
	if (visualizer.Init() != 0)
	{
		delete field;
		return -1;
	}
	visualizer.Start();
	visualizer.Destroy();
	delete field;
	return 0;
}

int ParseOptionsFromCommandline(int argc, char* argv[], Options& options)
{
	argparse::ArgumentParser program("visualizer");
	program.add_argument("-w", "--width")
		.help("width of grid map")
		.default_value(16)
		.store_into(options.w);
	program.add_argument("-h", "--height")
		.help("height of grid map")
		.default_value(16)
		.store_into(options.h);
	program.add_argument("-u")
		.help("upper bound of distance to maintain")
		.default_value(1024)
		.store_into(options.u);
	program.add_argument("-c", "--cost")
		.help("cost unit")
		.default_value(1)
		.store_into(options.costUnit);
	program.add_argument("-cd", "--cost-diagonal")
		.help("cost unit on diagonal directions")
		.default_value(1)
		.store_into(options.diagonalCostUnit);
	program.add_argument("-fs", "--font-size")
		.help("font size")
		.default_value(20)
		.store_into(options.fontSize);
	program.add_argument("-impl", "--implementer")
		.help("implementer, 0 for TrueClearanceField, 1 for BrushfireClearanceField")
		.default_value(0)
		.store_into(options.implementer);
	try
	{
		program.parse_args(argc, argv);
	}
	catch (const std::exception& e)
	{
		spdlog::error(e.what());
		return 1;
	}
	if (options.w > N || options.h > N)
	{
		spdlog::error("w or h is too large");
		return 2;
	}
	return 0;
}

Visualizer::Visualizer(const Options& options, IClearanceField* field)
	: options(options), field(field) {}

int Visualizer::Init()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		spdlog::error("SDL init error: {}", SDL_GetError());
		return -1;
	}

	// Load font.
	if (TTF_Init() == -1)
	{
		spdlog::error("SDL_ttf init error: {}", SDL_GetError());
		SDL_Quit();
		return -1;
	}

	font = TTF_OpenFont("fonts/Inconsolata-Medium.ttf", options.fontSize);
	if (font == nullptr)
	{
		spdlog::error("cant open fonts/Inconsolata-Medium.ttf: {}", SDL_GetError());
		TTF_Quit();
		SDL_Quit();
		return -1;
	}

	// Texture surface 0123456789-. black
	SDL_Surface* ts = TTF_RenderText_Solid(font, TextChars, { 0, 0, 0, 255 });
	if (!ts)
	{
		spdlog::error("can create SDL_Surface: {}", TTF_GetError());
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_Quit();
		return -1;
	}

	// Creates window.
	int w = options.w * GRID_SIZE, h = options.h * GRID_SIZE;
	window = SDL_CreateWindow("visualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
		SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		spdlog::error("Create window error: {}", SDL_GetError());
		SDL_FreeSurface(ts);
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_Quit();
		return -3;
	}

	// Creates renderer.
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == nullptr)
	{
		spdlog::error("Create renderer error: {}", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_FreeSurface(ts);
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_Quit();
		return -1;
	}

	// Texture.
	texture = SDL_CreateTextureFromSurface(renderer, ts);
	if (texture == nullptr)
	{
		spdlog::error("can create font texture: {}", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_FreeSurface(ts);
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_Quit();
		return -1;
	}
	// the surface could be freed now.
	SDL_FreeSurface(ts);

	// Char offsets and height in texture.
	int offset = 0;
	for (int i = 0; i < 11; i++)
	{
		int minx, maxx, miny, maxy, advance;
		TTF_GlyphMetrics(font, TextChars[i], &minx, &maxx, &miny, &maxy, &advance);
		font_char_width[i] = advance;
		font_char_offset[i] = offset;
		offset += advance;
	}
	font_char_height = TTF_FontHeight(font);

	// TrueClearanceField.
	field->Build();

	spdlog::info("Visualizer init done");
	return 0;
}

void Visualizer::Destroy()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
}

void Visualizer::Start()
{
	while (true)
	{
		// quit on -1
		if (handleInputs() == -1)
			break;

		// Background: white
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		draw();
		SDL_RenderPresent(renderer);

		SDL_Delay(30); // 30ms
	}
}

int Visualizer::handleInputs()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
			case SDL_QUIT: // quit
				return -1;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
				{
					spdlog::info("Ctrl-C : quit...");
					return -1;
				}
				if (e.key.keysym.sym == SDLK_ESCAPE)
				{
					reset();
					spdlog::info("ESC : clear all obstacles...");
					return 0;
				}
				break;
			case SDL_MOUSEBUTTONDOWN: // invert obstacles
				handleInvertObstacle(e.button.x / GRID_SIZE, e.button.y / GRID_SIZE);
				spdlog::info("invert an obstacle");
				break;
		}
	}
	return 0;
}

void Visualizer::reset()
{
	for (int y = 0; y < options.h; ++y)
	{
		for (int x = 0; x < options.w; ++x)
		{
			if (GRID[y][x])
			{
				GRID[y][x] = 0;
				field->Update(x, y);
			}
		}
	}
	field->Compute();
}

void Visualizer::handleInvertObstacle(int x, int y)
{
	GRID[y][x] ^= 1;
	field->Update(x, y);
	int n = field->Compute();
	spdlog::info("updated {} cells", n);
}

void Visualizer::draw()
{
	for (int y = 0; y < options.h; ++y)
	{
		for (int x = 0; x < options.w; ++x)
		{
			SDL_Rect rect{ x * GRID_SIZE, y * GRID_SIZE, GRID_SIZE, GRID_SIZE };
			SDL_Rect inner = { rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2 };
			// black border
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &rect);
			// for obstacles: red background.
			if (GRID[y][x])
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(renderer, &inner);
			}
			// Fill the number
			int v = field->Get(x, y);
			if (v == inf)
			{
				// render a single '-' in the middler of rect
				int x1 = rect.x + (rect.w - font_char_width[10]) / 2;
				int y1 = rect.y + (rect.h - font_char_height) / 2;
				renderDigit(x1, y1, 10);
			}
			else
			{
				// render the number in the middle of rect.
				renderNumber(rect, v);
			}
		}
	}
}

// Render char TextChars[d] to rect dst.
// d is the index of texture TextChars.
void Visualizer::renderDigit(int x, int y, int d)
{
	int		 w = font_char_width[d];
	int		 h = font_char_height;
	int		 offset = font_char_offset[d];
	SDL_Rect src = { offset, 0, w, h };
	SDL_Rect dst = { x, y, w, h };
	SDL_RenderCopy(renderer, texture, &src, &dst);
}

// Render a number (a sequence of digits) in the middle rect.
void Visualizer::renderNumber(SDL_Rect& rect, int number)
{
	// collects the digits
	int digits[10] = { 0 };
	int cnt = 0;
	do
	{
		digits[cnt++] = number % 10;
		number /= 10;
	}
	while (number);
	// the total width
	int w = 0;
	for (int i = cnt - 1; i >= 0; --i)
		w += font_char_width[digits[i]];
	// find the position of start.
	int x = rect.x + (rect.w - w) / 2;
	int y = rect.y + (rect.h - font_char_height) / 2;
	// starting render.
	for (int i = cnt - 1; i >= 0; --i)
	{
		renderDigit(x, y, digits[i]);
		x += font_char_width[digits[i]];
	}
}
