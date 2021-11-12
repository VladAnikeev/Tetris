const int XM = 10; //������ �������� ����
const int YN = 20; //������ �������� ����

int field[XM][YN] = { 0 };//������� ����
struct Point
{
	int x, y;
};

void check_travel_x_asis(int& move_state, Point* s)
{
	for (int i = 0; i < 4; ++i)
	{
		if ((s[i].x + move_state == XM) || ((s[i].x + move_state) == -1) || ((s[i].y) == YN) || (field[s[i].x + move_state][s[i].y]))
		{
			move_state = 0;
			return;
		}
	}
}

void check_turn(Point* s)
{
	Point p = s[1];
	Point b[4];
	int x, y;
	for (int i = 0; i < 4; ++i)
	{
		x = p.x - (s[i].y - p.y);
		y = p.y + (s[i].x - p.x);

		if ((x == XM) || (x == -1) || (y == YN) || (field[x][y]))
			return;
		else
		{
			b[i].x = x;
			b[i].y = y;
		}
	}
	for (int i = 0; i < 4; ++i)
		s[i] = b[i];
}
int	line = 0; int k;
void check_travel_y_asis(int& dy, int& dx, Point* s, bool& new_ttrs)
{
	for (int i = 0; i < 4; ++i)
	{
		if (((s[i].y + 1) == 20) || (field[s[i].x][s[i].y + 1] != 0))
		{
			for (i = 0; i < 4; ++i)
			{
				s[i].x += dx;
				field[s[i].x][s[i].y] = 1;
			}
			k = YN - 1;
			for (int j = YN - 1; j > -1; --j)
			{
				line = 0;
				for (int i = 0; i < XM; ++i)
				{
					if (field[i][j])
						++line;
					field[i][k] = field[i][j];
				}
				if (line < XM)
					--k;
			}
			new_ttrs = 1;
			return;
		}
	}
	dy = 1;
}



int main()
{
	//�������� ���� � �������� ��������
	sf::Vertex rectangle[] =
	{
		sf::Vertex(sf::Vector2f(0,0) , sf::Color(67, 206, 162)),
		sf::Vertex(sf::Vector2f(0,360) , sf::Color(24, 90, 157)),
		sf::Vertex(sf::Vector2f(360,360) , sf::Color(24, 90, 157)),
		sf::Vertex(sf::Vector2f(180,0) , sf::Color(67, 206, 162))
	};

	sf::RenderWindow window(sf::VideoMode(180, 360), "Tetris");
	sf::Texture texture;
	texture.loadFromFile("C:\\Users\\Admin\\Desktop\\Tetris\\x64\\Debug\\texture.png");
	sf::Sprite sprite(texture);

	Point a[4];//�������� ����������
	a[0].x = 0;//���������� ����������
	int number_ttrs = 0;

	int dx = 0, dy = 0;//��������� �����������
	bool rotate = false;//��������� ��������
	bool new_ttrs = 1;
	std::mt19937_64 gen(std::time(0));
	std::uniform_int_distribution<> uid(0, 6);
	std::uniform_int_distribution<> uid_color(1, 7);


	sf::Clock clock;
	float timer = 0, delay = 0.4;
	int colorNum = uid_color(gen);

	//
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// ������������ ����� �� �������� � ����� ������� ����?
			if (event.type == sf::Event::Closed)
				// ����� ��������� ���
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Up) 
					rotate = true;// ��� ����� ������� �����
				else if (event.key.code == sf::Keyboard::Left) 
					dx = -1;	// ��� ������� ������
				else if (event.key.code == sf::Keyboard::Right) 
					dx = 1;
				else if(event.key.code == sf::Keyboard::Down)


			}
		}

		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		if (new_ttrs)
		{
			number_ttrs = uid(gen);
			colorNum = uid_color(gen);
			for (int i = 0; i < 4; ++i)//�������� ���������� 
			{
				a[i].x = figures[number_ttrs][i] % 2;
				a[i].y = figures[number_ttrs][i] / 2;
				field[a[i].x][a[i].y] = colorNum;
			}
			new_ttrs = 0;
		}
		for (int i = 0; i < 4; ++i)
		{
			field[a[i].x][a[i].y] = 0;//���������
		}
		if (dx != 0)
			check_travel_x_asis(dx, a);//�������� �� ��� x
		if (rotate)
			check_turn(a);
		if (timer > delay)
		{
			check_travel_y_asis(dy, dx, a, new_ttrs);
			timer = 0;
		}
		if (new_ttrs == 0)
			for (int i = 0; i < 4; ++i)
			{
				a[i].x += dx;
				a[i].y += dy;
				field[a[i].x][a[i].y] = 1;
			}

		//��������� ����������
		dy = 0;
		dx = 0;
		rotate = 0;

		window.draw(rectangle, 4, sf::Quads);
		//window.clear(sf::Color::White);
		for (int j = YN - 1; j > -1; --j)
		{
			for (int i = 0; i < XM; ++i)
			{
				if (field[i][j] == 0)
					continue;
				sprite.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));
				sprite.setPosition(i * 18, j * 18);
				window.draw(sprite);
			}
		}
		line = 0;
		window.display();
	}
}