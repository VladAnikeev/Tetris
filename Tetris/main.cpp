#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

int figures[7][4] =
{
	1,3,5,7, // I
	2,5,4,7, // Z
	3,4,5,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

class tetris
{
private:
	struct coordinates
	{
		int x;
		int y;
	};
	
	coordinates tetramino_spare[4];
	float timer;
	float speed;
	bool need_tetramino;
	bool check(); 
public:
	int field[10][20] = { 0 };
	const int max_x = 10;
	const int max_y = 20;
	int color_tetramino;
	coordinates tetramino_main[4];

	tetris() :timer(0), need_tetramino(1),speed(0.4f) {};
	~tetris() {};
	
	bool new_tetramino(int number, int number_color);
	void speed_increase();
	void move_along_y(float &time);
	void move_along_x(int &motion);
	void rotate();
	void delete_line();
	
};


bool tetris::check()
{
	for (int i = 0; i < 4; ++i)
	{
		if ((tetramino_spare[i].x == -1) or (tetramino_spare[i].x == max_x) or
			(tetramino_spare[i].y == max_y) or (field[tetramino_spare[i].x][tetramino_spare[i].y]))
			return 0;
	}
	for (int i = 0; i < 4; ++i)
	{
		tetramino_main[i] = tetramino_spare[i];
	}
	return 1;
}

bool tetris::new_tetramino(int number, int number_color)
{
	if (need_tetramino)
	{
		for (int i = 0; i < 4; ++i)
		{
			tetramino_spare[i].x = figures[number][i] % 2;
			tetramino_spare[i].y = figures[number][i] / 2;
		}
		if (check())
		{
			color_tetramino = number_color;
			
			need_tetramino = 0;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

void tetris::speed_increase()
{
	speed = 0.01f;
}

void tetris::move_along_y(float& time)
{
	timer += time;
	if (timer > speed)
	{
		for (int i = 0; i < 4; i++)
		{
			tetramino_spare[i].x = tetramino_main[i].x;
			tetramino_spare[i].y = (tetramino_main[i].y + 1);
		}
		if (!check())
		{
			for (int i = 0; i < 4; i++)
			{
				field[tetramino_main[i].x][tetramino_main[i].y] = color_tetramino;
			}
			need_tetramino = 1;
			
		}	
		timer = 0;
	}
}

void tetris::move_along_x(int& motion)
{
	if (motion != 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			tetramino_spare[i].x = tetramino_main[i].x + motion;
			tetramino_spare[i].y = tetramino_main[i].y;
		}
		check();
		motion = 0;
	}
}

void tetris::rotate()
{
	for (int i = 0; i < 4; ++i)
	{
		tetramino_spare[i].x = tetramino_main[1].x - (tetramino_main[i].y - tetramino_main[1].y);
		tetramino_spare[i].y = tetramino_main[1].y + (tetramino_main[i].x - tetramino_main[1].x);
	}
	check();
}

void tetris::delete_line()
{
	speed = 0.4f;
	if (need_tetramino)
	{
		int counter = max_y - 1;
		int line;
		for (int j = counter; j > -1; --j)
		{
			line = 0;
			for (int i = 0; i < max_x; ++i)
			{
				if (field[i][j])
					++line;
				field[i][counter] = field[i][j];
			}
			if (line < max_x)
				--counter;
		}
	}
}


int main()
{
	//создание окна и создание текстуры

	//фон, квадрат с гридиентом
	sf::Vertex rectangle[] =
	{
		sf::Vertex(sf::Vector2f(0,0) , sf::Color(67, 206, 162)),
		sf::Vertex(sf::Vector2f(0,360) , sf::Color(24, 90, 157)),
		sf::Vertex(sf::Vector2f(360,360) , sf::Color(24, 90, 157)),
		sf::Vertex(sf::Vector2f(180,0) , sf::Color(67, 206, 162))
	};

	//само окно
	sf::RenderWindow window(sf::VideoMode(180,360),"Tetris");

	//создание и загрузка текстур
	sf::Texture texture;
	texture.loadFromFile("texture.png");
	sf::Sprite sprite(texture);

	//рандом:времb, номер тетресинb, номер цвета
	std::mt19937_64 gen(std::time(0));
	std::uniform_int_distribution<> number_tetrimino(0, 6);
	std::uniform_int_distribution<> number_color(1, 7);
	
	//вреемя
	sf::Clock clock;
	//класс логики тетриса
	tetris game;

	//смещение в сторону по оси X
	int motive = 0;

	//события
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно
			if (event.type == sf::Event::Closed)
				window.close();// тогда закрываем его
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up) // переворот тетресины
				{
					game.rotate();
				}
				else if (event.key.code == sf::Keyboard::Left) // смещение влево
				{
					motive = -1;
					game.move_along_x(motive);
				}	
				else if (event.key.code == sf::Keyboard::Right)// смещение вправо
				{
					motive = 1;
					game.move_along_x(motive);
				}
				else if (event.key.code == sf::Keyboard::Down)//ускорение
					game.speed_increase();
			}
		}
		
		float time = clock.getElapsedTime().asSeconds();//запись времени
		clock.restart();

		game.new_tetramino(number_tetrimino(gen),number_color(gen));

		game.move_along_y(time);

		game.delete_line();

		//отрисовка
		window.draw(rectangle, 4, sf::Quads);//фон
		
		//отрисовка поля
		for (int j = game.max_y - 1; j > -1; --j)
		{		
			for (int i = 0; i < game.max_x; ++i)
			{
				if (game.field[i][j] == 0)
					continue;
				sprite.setTextureRect(sf::IntRect((game.field[i][j]-1)*18, 0, 18, 18));
				sprite.setPosition(float(i * 18),float(j * 18));
				window.draw(sprite);
			}	
		}
		//отрисовка тетресины
		for (int i = 0; i < 4; i++)
		{
			sprite.setTextureRect(sf::IntRect((game.color_tetramino - 1) * 18, 0, 18, 18));
			sprite.setPosition(float(game.tetramino_main[i].x * 18),
				float( game.tetramino_main[i].y * 18));

			window.draw(sprite);
		}
		
		window.display();
	}
}
