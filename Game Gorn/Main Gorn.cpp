#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <WinUser.h>
#include <time.h>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace sf;
using namespace std;
enum {Menu,Game,Stageclear,Missionfail,Tutorial,Name,Rank,Win,pause};

Vector2f movement, movementmon1, movementmon2,movementmon3,itemmovement;  // สร้างตัวแปรเก็บค่าความเร็ว
Vector2f position, positionmon1, chickposition, positionmon2,itemposition;            // สร้างตัวเเปรเก็บตำเเหน่งของตัวละคร
bool jumpcheck = false;      //  กำหนดตัวเเปรที่ทำให้ตัวละครไม่สามารถกระโดดซ้ำๆได้
Clock narika, narika1, narika2;                      //  ประกาศตัวเเปรเวลา
float timer,itemspawntime,itemtime;		//  ประกาศตัวเเปรไว้เก็บเวลา
int life=4,state=0,score=0;
int statemon1 = 0, statemon2 = 1, statemon3 = 0, stategorn = 0, itemstate = 0, speedstate=0,slowdownstate=0,immortalstate=0,gamestate=1,chickstate=0,statetext=1;
bool pressed;
// function to random chicken /////////////////////////////////
int i, j, k;
int scorerank;
string name;

int random(int min, int max)
{
	int a,count;
	for (count = 0; count < 100; count++)
	{
		a = rand() % max;
		if (a >= min)
		{
			return a;
			break;
		}
	}
}

int main()
{
	RenderWindow window(sf::VideoMode(1280, 720), "My Daddy become to human pls. (DEMO)", sf::Style::Close | sf::Style::Resize);
	FloatRect windowBounds({ 0.f, 0.f }, window.getDefaultView().getSize());
	srand(time(NULL));
	//icon/////////////////////////////////////////////////////////
	Image icon;
	icon.loadFromFile("animation/gornhead.png");
	window.setIcon(512, 512, icon.getPixelsPtr());
	//sound//////////////////////////////////////////////////////////
	SoundBuffer menubuffer;
	menubuffer.loadFromFile("music/menu.ogg");
	Sound menu;
	menu.setBuffer(menubuffer);
	menu.setVolume(75.0f);
	menu.setLoop(true);

	SoundBuffer gamebuffer;
	gamebuffer.loadFromFile("music/game.ogg");
	Sound game;
	game.setBuffer(gamebuffer);
	game.setVolume(100.0f);
	game.setLoop(true);

	SoundBuffer collectbuffer;
	collectbuffer.loadFromFile("music/collect.ogg");
	Sound collect;
	collect.setBuffer(collectbuffer);
	collect.setVolume(25.0f);

	SoundBuffer jumpbuffer;
	jumpbuffer.loadFromFile("music/jump.ogg");
	Sound jump;
	jump.setBuffer(jumpbuffer);
	jump.setVolume(50.0f);

	SoundBuffer gameoverbuffer;
	gameoverbuffer.loadFromFile("music/gameover.ogg");
	Sound gameover;
	gameover.setBuffer(gameoverbuffer);
	gameover.setVolume(50.0f);

	SoundBuffer clearbuffer;
	clearbuffer.loadFromFile("music/stageclear.ogg");
	Sound clear;
	clear.setBuffer(clearbuffer);
	clear.setVolume(50.0f);

	SoundBuffer winbuffer;
	winbuffer.loadFromFile("music/win.ogg");
	Sound win;
	win.setBuffer(winbuffer);
	win.setVolume(50.0f);

	SoundBuffer diebuffer;
	diebuffer.loadFromFile("music/die.ogg");
	Sound die;
	die.setBuffer(diebuffer);
	die.setVolume(50.0f);

	SoundBuffer buttonbuffer;
	buttonbuffer.loadFromFile("music/button.ogg");
	Sound button;
	button.setBuffer(buttonbuffer);
	button.setVolume(35.0f);

	SoundBuffer itembuffer;
	itembuffer.loadFromFile("music/item.ogg");
	Sound itemsound;
	itemsound.setBuffer(itembuffer);
	itemsound.setVolume(50.0f);

	//score board////////////////////////////////////////////////////
	Font font;
	font.loadFromFile("font/Players.ttf");

	Font fontname;
	fontname.loadFromFile("font/Hanged Letters.ttf");

	Text text;
	text.setFont(font);
	text.setCharacterSize(23);
	text.setFillColor(Color::White);

	Text updatescore;
	updatescore.setFont(font);
	updatescore.setCharacterSize(90);
	updatescore.setFillColor(Color::Black);

	Texture scoreboard;
	scoreboard.loadFromFile("animation/scoreboard.png");
	Sprite Sboard;
	Sboard.setTexture(scoreboard);
	Sboard.setPosition(1100.0f,5.0f);
	Sboard.setScale(0.04f,0.04f);

	String playername;
	Text player;

	Font creditfont;
	creditfont.loadFromFile("font/Handsdown.ttf");
	Text credit;
	credit.setString("Siriwan Kullavanijaya 61011023");
	credit.setFont(creditfont);
	credit.setCharacterSize(25);
	
	credit.setPosition(20, 680);

	//rank///////////////////////////////////////////////
	vector<pair<int, string>> highscore;

	// Main Menu /////////////////////////////////////////////////////
	Texture BGmain;
	BGmain.loadFromFile("animation/bgmain.png");
	Sprite BGmainsprite;
	BGmainsprite.setTexture(BGmain);

	Texture click, click1, click2, click3;

	click.loadFromFile("animation/click.png");
	click1.loadFromFile("animation/click1.png");
	click2.loadFromFile("animation/click2.png");
	click3.loadFromFile("animation/click3.png");

	Sprite sclick, sclick1, sclick2, sclick3;

	sclick.setTexture(click);
	sclick1.setTexture(click1);
	sclick2.setTexture(click2);
	sclick3.setTexture(click3);
	sclick.setPosition(Vector2f(0, 0));
	sclick1.setPosition(Vector2f(0, 0));
	sclick2.setPosition(Vector2f(0, 0));
	sclick3.setPosition(Vector2f(0, 0));

	Vector2i mouse;
	HWND windowHandle = window.getSystemHandle();
	long propsLong = GetWindowLong(windowHandle, GWL_STYLE);
	SetWindowLong(windowHandle, GWL_STYLE, propsLong & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);
	
	//Gorn///////////////////////////////////////////////////////
	RectangleShape gornbox(Vector2f(60, 90)); //สร้างกรอบสี่เหลี่ยมสำหรับแสดงภาพ
	gornbox.setPosition(Vector2f(80.0f, 22.0f)); //กำหนดจุดที่แสดงในจอ
	Texture Gorn; //ตัวแปรสำหรับโหลดภาพ
	Gorn.setRepeated(true); //ตั้งค่าเพื่อใช้ภาพแบบซ้ำๆ
	Gorn.setSmooth(true); //ตั้งค่าเพื่อให้ภาพเปลี่ยนอย่างลื่นไหลขึ้น
	gornbox.setTexture(&Gorn); //กำหนดกรอบที่ใช้แสดงให้แสดงภาพของเรา
	Gorn.loadFromFile("animation/gornmove.png");

	//mom////////////////////////////////////////////////////////
	RectangleShape mombox(Vector2f(72, 90)); //สร้างกรอบสี่เหลี่ยมสำหรับแสดงภาพ
	mombox.setPosition(Vector2f(1160.0f, 622.0f)); //กำหนดจุดที่แสดงในจอ
	Texture Mom; //ตัวแปรสำหรับโหลดภาพ
	Mom.setRepeated(true); //ตั้งค่าเพื่อใช้ภาพแบบซ้ำๆ
	Mom.setSmooth(true); //ตั้งค่าเพื่อให้ภาพเปลี่ยนอย่างลื่นไหลขึ้น
	mombox.setTexture(&Mom); //กำหนดกรอบที่ใช้แสดงให้แสดงภาพของเรา
	Mom.loadFromFile("animation/mom.png");

	//blackground////////////////////////////////////////////////
	RectangleShape BG(Vector2f(1280.0f, 720.0f));
	BG.setPosition(0.0f, 0.0f);
	Texture blackground;
	blackground.loadFromFile("animation/bg.png");
	BG.setTexture(&blackground);

	//monster1//////////////////////////////////////////////////
	RectangleShape mon1(Vector2f(57.0f, 87.0f));
	mon1.setPosition(2.0f, 180.0f);
	Texture monster1;
	monster1.setRepeated(true); //ตั้งค่าเพื่อใช้ภาพแบบซ้ำๆ
	monster1.setSmooth(true); //ตั้งค่าเพื่อให้ภาพเปลี่ยนอย่างลื่นไหลขึ้น
	mon1.setTexture(&monster1);
	monster1.loadFromFile("animation/mon1.png");

	//monster2/////////////////////////////////////////////////
	RectangleShape mon2(Vector2f(80.0f, 80.0f));
	mon2.setPosition(1150.0f, 485.0f);
	
	Texture monster2;
	monster2.setRepeated(true); //ตั้งค่าเพื่อใช้ภาพแบบซ้ำๆ
	monster2.setSmooth(true); //ตั้งค่าเพื่อให้ภาพเปลี่ยนอย่างลื่นไหลขึ้น
	mon2.setTexture(&monster2);
	monster2.loadFromFile("animation/mon21.png");

	//monster3/////////////////////////////////////////////////
	RectangleShape mon3(Vector2f(80.0f, 80.0f));
	mon3.setPosition(200.0f, 335.0f);
	Texture monster3;
	monster3.setRepeated(true); //ตั้งค่าเพื่อใช้ภาพแบบซ้ำๆ
	monster3.setSmooth(true); //ตั้งค่าเพื่อให้ภาพเปลี่ยนอย่างลื่นไหลขึ้น
	mon3.setTexture(&monster3);
	monster3.loadFromFile("animation/mon31.png");

	//heart////////////////////////////////////////////////////
	Texture hearttexture;
	RectangleShape Sheart;
	hearttexture.loadFromFile("animation/heart.png");
	vector<RectangleShape> heartvec;
	Sheart.setTexture(&hearttexture);
	Sheart.setSize(Vector2f(55.0f, 30.0f));
	for (i = 0; i < 4; i++)
	{
		Sheart.setPosition((i * 55.0f) + 870.0f, 10.0f);
		heartvec.push_back(Sheart);
	}
	//fried chicken item////////////////////////////////////////
	Texture chickentexture;
	RectangleShape chicken;	
	vector<RectangleShape> chick,chick1;
	chickentexture.loadFromFile("animation/chicken.png");
	chicken.setTexture(&chickentexture);
	chicken.setSize(Vector2f(27, 37));
	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 5; j++)
		{
		chicken.setPosition((i * random(110,125)) + 20, 65 + (150 * j));	
		chick.push_back(chicken);
		}
	}
	//item/////////////////////////////////////////////////////////////
	RectangleShape itemReg;
	Texture item;
	vector<RectangleShape*> itemvec;
	//howtoplay//////////////////////////////////////////////////////////
	Texture howtoplay;
	howtoplay.loadFromFile("animation/howtoplay.png");
	Sprite showtoplay;
	showtoplay.setTexture(howtoplay);
	//game over scene////////////////////////////////////////////////////
	RectangleShape OV(Vector2f(1280.0f, 720.0f));
	OV.setPosition(0.0f, 0.0f);
	Texture Gameover;
	Gameover.setRepeated(true); 
	Gameover.setSmooth(true); //ตั้งค่าเพื่อให้ภาพเปลี่ยนอย่างลื่นไหลขึ้น
	OV.setTexture(&Gameover);
	Gameover.loadFromFile("animation/gameover.png");
	//Rank board////////////////////////////////////////////////////////
	Texture rankboard;
	rankboard.loadFromFile("animation/rank.png");
	Sprite srankboard;
	srankboard.setTexture(rankboard);
	//stage clear//////////////////////////////////////////////////////
	Texture stageclear;
	stageclear.loadFromFile("animation/stateclear.png");
	Sprite sstageclear;
	sstageclear.setTexture(stageclear);
	//mission complete/////////////////////////////////////////////////
	Texture missioncomplete;
	missioncomplete.loadFromFile("animation/missioncomplete.png");
	Sprite smissioncomplete;
	smissioncomplete.setTexture(missioncomplete);
	//name//////////////////////////////////////////////////////
	Texture nameplayer;
	nameplayer.loadFromFile("animation/name.png");
	Sprite snameplayer;
	snameplayer.setTexture(nameplayer);

	// Rec floor 1 left  ///////////////////////////////
	RectangleShape floor1left(Vector2f(10.0f, 720.0f));
	floor1left.setPosition(1270.0f, 152.0f);
	// Rec floor 1 right //////////////////////////////////
	RectangleShape floor1right(Vector2f(10.0f, 720.0f));
	floor1right.setPosition(0.0f, 152.0f);
	// Rec floor 1 st down //////////////////////////////////
	RectangleShape floor1stdown(Vector2f(1280.0f, 10.0f));
	floor1stdown.setPosition(0.0f, 110.0f);
	// Rec floor 1 down //////////////////////////////////
	RectangleShape floor1down(Vector2f(1280.0f, 10.0f));
	floor1down.setPosition(0.0f, 264.0f);
	// Rec floor 2 down //////////////////////////////////
	RectangleShape floor2down(Vector2f(1280.0f, 10.0f));
	floor2down.setPosition(0.0f, 415.0f);
	// Rec floor 2 down for mon2//////////////////////////////////
	RectangleShape floor2down2(Vector2f(1280.0f, 10.0f));
	floor2down2.setPosition(0.0f, 350.0f);
	// Rec floor 3 down //////////////////////////////////
	RectangleShape floor3down(Vector2f(1280.0f, 10.0f));
	floor3down.setPosition(0.0f, 564.0f);
	// Rec floor 4 down //////////////////////////////////
	RectangleShape floor4down(Vector2f(1280.0f, 10.0f));
	floor4down.setPosition(0.0f, 713.0f);
	// ladder floor 1 ///////////////////////////////////
	//1
	RectangleShape ladder1(Vector2f(20.0f, 80.0f));
	ladder1.setPosition(145.0f, 115.0f);
	//2
	RectangleShape ladder2(Vector2f(20.0f, 80.0f));
	ladder2.setPosition(501.0f, 115.0f);
	//3
	RectangleShape ladder3(Vector2f(20.0f, 80.0f));
	ladder3.setPosition(875.0f, 115.0f);
	// ladder floor2 ////////////////////////////////////
	//4
	RectangleShape ladder4(Vector2f(20.0f, 80.0f));
	ladder4.setPosition(356.0f, 267.0f);
	//5
	RectangleShape ladder5(Vector2f(20.0f, 80.0f));
	ladder5.setPosition(725.0f, 267.0f);
	//6
	RectangleShape ladder6(Vector2f(20.0f, 80.0f));
	ladder6.setPosition(1202.0f, 267.0f);
	// ladder floor3 ////////////////////////////////////
	//7
	RectangleShape ladder7(Vector2f(20.0f, 80.0f));
	ladder7.setPosition(188.0f, 420.0f);
	//8
	RectangleShape ladder8(Vector2f(20.0f, 80.0f));
	ladder8.setPosition(693.0f, 420.0f);
	//9
	RectangleShape ladder9(Vector2f(20.0f, 80.0f));
	ladder9.setPosition(954.0f, 420.0f);
	// ladder floor4 ////////////////////////////////////
	//10
	RectangleShape ladder10(Vector2f(20.0f, 80.0f));
	ladder10.setPosition(50.0f, 567.0f);
	//11
	RectangleShape ladder11(Vector2f(20.0f, 80.0f));
	ladder11.setPosition(405.0f, 567.0f);
	//12
	RectangleShape ladder12(Vector2f(20.0f, 80.0f));
	ladder12.setPosition(854.0f, 567.0f);
	//13
	RectangleShape ladder13(Vector2f(20.0f, 80.0f));
	ladder13.setPosition(1197.0f, 567.0f);
	// ladder floor 1 for jump down ///////////////////////////////////
	//1
	RectangleShape ladder00(Vector2f(20.0f, 80.0f));
	ladder00.setPosition(145.0f, 70.0f);
	//2
	RectangleShape ladder22(Vector2f(20.0f, 80.0f));
	ladder22.setPosition(501.0f, 70.0f);
	//3
	RectangleShape ladder33(Vector2f(20.0f, 80.0f));
	ladder33.setPosition(875.0f, 70.0f);
	// ladder floor2 ////////////////////////////////////
	//4
	RectangleShape ladder44(Vector2f(20.0f, 80.0f));
	ladder44.setPosition(356.0f, 222.0f);
	//5
	RectangleShape ladder55(Vector2f(20.0f, 80.0f));
	ladder55.setPosition(725.0f, 222.0f);
	//6
	RectangleShape ladder66(Vector2f(20.0f, 80.0f));
	ladder66.setPosition(1202.0f, 222.0f);
	// ladder floor3 ////////////////////////////////////
	//7
	RectangleShape ladder77(Vector2f(20.0f, 80.0f));
	ladder77.setPosition(188.0f, 375.0f);
	//8
	RectangleShape ladder88(Vector2f(20.0f, 80.0f));
	ladder88.setPosition(693.0f, 375.0f);
	//9
	RectangleShape ladder99(Vector2f(20.0f, 80.0f));
	ladder99.setPosition(954.0f, 375.0f);
	// ladder floor4 ////////////////////////////////////
	//10
	RectangleShape ladder100(Vector2f(20.0f, 80.0f));
	ladder100.setPosition(50.0f, 522.0f);
	//11
	RectangleShape ladder111(Vector2f(20.0f, 80.0f));
	ladder111.setPosition(405.0f, 522.0f);
	//12
	RectangleShape ladder122(Vector2f(20.0f, 80.0f));
	ladder122.setPosition(854.0f, 522.0f);
	//13
	RectangleShape ladder133(Vector2f(20.0f, 80.0f));
	ladder133.setPosition(1197.0f, 522.0f);

	////animation////////////////////////////////////////////////////////
	float totalTime = 0,totalTime1=0; //ตัวแปรสำหรับเก็บเวลาสะสมของการทำงานในลูป
	float switchTime = 0.1,switchTime1 = 0.3,currenttime=0; //ระยะเวลาระหว่างการเปลี่ยนฉากของ animation
	Vector2u currentBox(0, 0), currentmon1(0, 0), currentmon2(0, 0),currentmon3(0,0),currentscene(0,0),currenthtp(0,0),currentitem(0,0),currentmom(0,0); //เลือกชุด animationแถวที่ 3 และให้เริ่มต้นแสดงจากฉากแรก
	IntRect boxgorn, boxmon1, boxmon2,boxmon3,gameoverbox,htpbox,itembox,boxmom; //ตัวแปรสำหรับกำหนดฉากที่จะเอามาแสดง
	//Gorn animation//////////////////////////////////////////////////////
	boxgorn.width = Gorn.getSize().x / 3;
	boxgorn.height = Gorn.getSize().y / 6;
	boxgorn.left = currentBox.x; //กำหนดให้เริ่มแสดงที่ฉากลำดับแรก
	boxgorn.top = currentBox.y*boxgorn.height; //กำหนดให้แสดงชุด animationแถวที่ 3
	//Mom//////////////////////////////////////////////////////
	boxmom.width = Mom.getSize().x / 3;
	boxmom.height = Mom.getSize().y / 1;
	boxmom.left = currentmom.x; //กำหนดให้เริ่มแสดงที่ฉากลำดับแรก
	boxmom.top = currentmom.y*boxmom.height; //กำหนดให้แสดงชุด animationแถวที่ 3
	//mon 1 animation//////////////////////////////////////////////////////
	boxmon1.width = monster1.getSize().x / 3;
	boxmon1.height = monster1.getSize().y / 1;
	boxmon1.left = currentmon1.x;
	boxmon1.top = currentmon1.y*boxmon1.height;
	//mon 2 animation/////////////////////////////////////////////////////
	boxmon2.width = monster2.getSize().x / 3;
	boxmon2.height = monster2.getSize().y / 2;
	boxmon2.left = currentmon2.x;
	boxmon2.top = currentmon2.y*boxmon2.height;
	//mon 3 animation/////////////////////////////////////////////////////
	boxmon3.width = monster3.getSize().x / 3;
	boxmon3.height = monster3.getSize().y / 2;
	boxmon3.left = currentmon3.x;
	boxmon3.top = currentmon3.y*boxmon3.height;
	// gameover //////////////////////////////////////////////////////////
	gameoverbox.width = Gameover.getSize().x / 3;
	gameoverbox.height = Gameover.getSize().y / 1;
	gameoverbox.top = currentscene.x;
	gameoverbox.left = currentscene.y*gameoverbox.height;

	int Mode = Menu;
	int Random;
	bool pauseGame = false;
	
	cout << Joystick::getButtonCount(0) << "\n";
	if (Joystick::isConnected(0)) cout << "connect";
	while (window.isOpen())
	{
	
		if (Mode == Menu)
		{
			menu.play();
		}
		if (Mode == Game)
		{
			game.play();
		}
		credit.setFillColor(Color::White);
		if (Mode == Game) credit.setFillColor(Color::Black);
		sf::Event event;
		switch (Mode)
		{
		case Menu:
			while (Mode == Menu)
			{
				playername.clear();
				statetext = 1;
				position.x = 80;
				position.y = 22;
					mouse = sf::Mouse::getPosition(window);
					
					while (window.pollEvent(event))
					{
						switch (event.type)
						{
						case sf::Event::Closed:
							window.close();
							menu.stop();
								break;
						case sf::Event::Resized:
								break;
						case sf::Event::MouseButtonPressed: // ถ้ามีการคลิปของเมาส์
							if (event.mouseButton.button == sf::Mouse::Left) // เช็คว่าเมาส์ซ้ายไหม
							{
								if (mouse.x > 917 && mouse.x < 1105 && mouse.y >334 && mouse.y < 380)
								{ // ถ้าเกิดมีการคลิปในกรอบ START
									button.play();
									Mode = Name;
								}
								else if (mouse.x > 929 && mouse.x < 1095 && mouse.y >432 && mouse.y < 487)
								{
									button.play();
									for (i = 0; i < highscore.size(); i++)
									{
										highscore.erase(highscore.begin());
									}
									ifstream loadFile;
									loadFile.open("highscore.txt");
									while (!loadFile.eof()) {
										string tempName;
										int tempScore;
										loadFile >> tempName >> tempScore;
										if (tempName != "")
										{
											highscore.push_back({ tempScore,tempName });
										}
									}
									sort(highscore.begin(), highscore.end(), greater<pair<int, string>>());
									Mode = Rank; // เปลียน mode = 2 คือเข้าดูอันดับแรงค์
								}
								else if (mouse.x > 949 && mouse.x < 1072 && mouse.y >543 && mouse.y < 588)
								{
									button.play();
									menu.stop();
									window.close();
								}
							}
						}
					}
					window.draw(BGmainsprite);
					if (mouse.x > 917 && mouse.x < 1105 && mouse.y >334 && mouse.y < 380)
					{
						
						window.draw(sclick1); //เปลี่ยนรุปที่สตาร์ทขึ้นสีขาว
					}
					else if (mouse.x > 929 && mouse.x < 1095 && mouse.y >432 && mouse.y < 487) // ขอบเขตของปุ่ม RANK
					{
						
						window.draw(sclick2); // เปลี่ยนรูปที่ RANK ขึ้นสีขาว
					}
					else if (mouse.x > 949 && mouse.x < 1072 && mouse.y >543 && mouse.y < 588) // ขอบเขตของ EXIT
					{
						
						window.draw(sclick3); //เปลี่ยนรูป
					}
					else // ถ้าไม่มีอะไรก็จะวาดแบบปกติ
					{
						window.draw(sclick);
					}
					window.draw(credit);
					window.display();
					
					window.clear();
					
				}
			
		case Game:
			
			while (Mode == Game)
			{
				if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
					button.play();
					Mode = pause;
				}
				
				for (i = 0; i < highscore.size(); i++)
				{
					highscore.erase(highscore.begin());
				}
				FloatRect gornbound = gornbox.getGlobalBounds();
				FloatRect mon1bound = mon1.getGlobalBounds();
				
				//score board/////////////////////////////////////////
				text.setPosition(1170, 8);
				text.setString(" " + to_string(score));
				//item///////////////////////////////////////////////////////////////
				currenttime += narika2.restart().asSeconds();
					if (currenttime - itemspawntime > 5)
					{
							if (itemstate == 0)
							{
								itemReg.setSize(Vector2f(70, 70));
								itemReg.setPosition(random(20, 1120), -70);
								item.loadFromFile("animation/item.png");
								itemReg.setTexture(&item);
								itemvec.push_back(&itemReg);

								itembox.width = item.getSize().x / 3;
								itembox.height = item.getSize().y / 1;
								itembox.top = currentitem.y*itembox.height;
								itembox.left = currentitem.x*itembox.width;

								itemstate = 1;
								itemspawntime = currenttime;
								Random = random(0, 3);
							}
					}
				totalTime += narika.restart().asSeconds();
				if (totalTime >= switchTime) //ถ้าเวลาสะสมมากกว่าหรือเท่ากับเวลาในการเปลี่ยนฉาก
				{
					//mom////////////////////////////////////////////////
					currentmom.x++;
					if (currentmom.x >= 3.0f)
					{
						currentmom.x = 0;
						//std::cout << "mon1" << std::endl;
					}
					boxmom.left = currentmom.x*boxmom.width;
					// mon 1 animation///////////////////////////////////
					currentmon1.x++;
					if (currentmon1.x >= 3.0f)
					{
						currentmon1.x = 0;
						//std::cout << "mon1" << std::endl;
					}
					boxmon1.left = currentmon1.x*boxmon1.width;
					// mon 2 animation///////////////////////////////////
					if (statemon2 == 1)
					{
						currentmon2.x++;
						if (currentmon2.x >= 3.0f)
						{
							currentmon2.x = 0;
							currentmon2.y = 0;
						}
						boxmon2.left = currentmon2.x*boxmon2.width;
						boxmon2.top = currentmon2.y*boxmon2.height;
					}
					if (statemon2 == 0)
					{
						currentmon2.x++;
						if (currentmon2.x >= 3.0f)
						{
							currentmon2.x = 0;
							currentmon2.y = 1;
						}
						boxmon2.left = currentmon2.x*boxmon2.width;
						boxmon2.top = currentmon2.y*boxmon2.height;
					}

					// mon 3 animation///////////////////////////////////
					if (statemon3 == 1)
					{
						currentmon3.x++;
						if (currentmon3.x >= 3.0f)
						{
							currentmon3.x = 0;
							currentmon3.y = 0;
						}
						boxmon3.left = currentmon3.x*boxmon3.width;
						boxmon3.top = currentmon3.y*boxmon3.height;
					}
					if (statemon3 == 0)
					{
						currentmon3.x++;
						if (currentmon3.x >= 3.0f)
						{
							currentmon3.x = 0;
							currentmon3.y = 1;
						}
						boxmon3.left = currentmon3.x*boxmon3.width;
						boxmon3.top = currentmon3.y*boxmon3.height;
					}

					// GORN MOVE////////////////////////////////////////
					if (movement.x == 0.0f) {
						currentBox.x++; //เปลี่ยนฉากไปทางขวาตามแนวนอน
						if (currentBox.x >= 3)
						{  //ถ้าฉากเกินภาพแล้วให้วนไปที่ฉากลำดับแรกใหม่
							currentBox.x = 0;
							currentBox.y = 0;
						}
						boxgorn.left = currentBox.x*boxgorn.width; //กำหนดตำแหน่งของฉากแนวนอนด้วยการเอาลำดับฉากปัจุบันคูณกับความกว้างของฉาก
						boxgorn.top = currentBox.y*boxgorn.height; //กำหนดให้แสดงชุด animationแถวที่ 3
					}
					if (movement.x > 0.0f)
					{
						currentBox.x++;
						if (currentBox.x >= 3)
						{
							currentBox.x = 0;
							currentBox.y = 1;
						}
						boxgorn.left = currentBox.x*boxgorn.width; //กำหนดตำแหน่งของฉากแนวนอนด้วยการเอาลำดับฉากปัจุบันคูณกับความกว้างของฉาก
						boxgorn.top = currentBox.y*boxgorn.height; //กำหนดให้แสดงชุด animationแถวที่ 3
					}
					if (movement.x < 0.0f)
					{
						currentBox.x++; //เปลี่ยนฉากไปทางขวาตามแนวนอน
						if (currentBox.x >= 3)
						{  //ถ้าฉากเกินภาพแล้วให้วนไปที่ฉากลำดับแรกใหม่
							currentBox.x = 0;
							currentBox.y = 2;
						}
						boxgorn.left = currentBox.x*boxgorn.width; //กำหนดตำแหน่งของฉากแนวนอนด้วยการเอาลำดับฉากปัจุบันคูณกับความกว้างของฉาก
						boxgorn.top = currentBox.y*boxgorn.height; //กำหนดให้แสดงชุด animationแถวที่ 3
					}
					if (movement.y > 0.0f && movement.x > 0.0f)
					{
						currentBox.x++; //เปลี่ยนฉากไปทางขวาตามแนวนอน
						if (currentBox.x >= 3)
						{  //ถ้าฉากเกินภาพแล้วให้วนไปที่ฉากลำดับแรกใหม่
							currentBox.x = 0;
							currentBox.y = 3;
						}
						boxgorn.left = currentBox.x*boxgorn.width; //กำหนดตำแหน่งของฉากแนวนอนด้วยการเอาลำดับฉากปัจุบันคูณกับความกว้างของฉาก
						boxgorn.top = currentBox.y*boxgorn.height; //กำหนดให้แสดงชุด animationแถวที่ 3
					}
					if (movement.y > 0.0f && movement.x < 0.0f)
					{
						currentBox.x++;
						if (currentBox.x >= 3)
						{
							currentBox.x = 0;
							currentBox.y = 4;
						}
						boxgorn.left = currentBox.x*boxgorn.width; //กำหนดตำแหน่งของฉากแนวนอนด้วยการเอาลำดับฉากปัจุบันคูณกับความกว้างของฉาก
						boxgorn.top = currentBox.y*boxgorn.height; //กำหนดให้แสดงชุด animationแถวที่ 3
					}
					totalTime -= switchTime;//รีเซ็ตเวลาสะสมใหม่
				}

				timer = narika1.restart().asSeconds();//  นับเวลาในเกม
				position = gornbox.getPosition();         //  เก็บตำเเหน่งของผู้เล่นไว้ในตัวเเปร
				movement.x = 0;  //  ตั้งให้ความเร็วเป็น  0 ทุกครั้งที่วน Loop 
				movementmon1.x = 0;
				movementmon2.x = 0;
				movementmon3.x = 0;

				if (position.y < 22.0f&&position.y >= 0) 
				{
					movement.y += 4.0f*timer;     
				}
				else if (position.y <= 175.0f && position.y >= 25)
				{
					movement.y += 4.0f*timer;
				}
				else if (position.y <= 327.0f&&position.y >= 177)
				{
					movement.y += 4.0f*timer;
				}
				else if (position.y <= 475.0f&&position.y >= 329)
				{
					movement.y += 4.0f*timer;
				}
				else if (position.y <= 625.0f&&position.y >= 477.0f)
				{
					movement.y += 4.0f*timer;
				}
				else
				{
					jumpcheck = true;                      //  เมื่อ player เเตะขอบจอล่าง ถึงจะกระโดดได้อีกรอบ
					movement.y = 0;                     //  เมื่อ player เเตะขอบจอล่างจะให้เหยุดเเรงโน้มถ่วง
				}

				//mon1 move//////////////////////////////////////
				if (statemon1 == 0)
				{
					if (slowdownstate == 1)
					{
						movementmon1.x -= 0.5f;
					}
					movementmon1.x += 0.8f;
				}
				else
				{
					if (slowdownstate == 1)
					{
						movementmon1.x += 0.5f;
					}
					movementmon1.x -= 0.8f;
				}
				//mon2 move//////////////////////////////////////
				if (statemon2 == 1)
				{
					if (slowdownstate == 1)
					{
						movementmon2.x += 0.5f;
					}
					movementmon2.x -= 0.8f;
				}
				else
				{
					if (slowdownstate == 1)
					{
						movementmon2.x -= 0.5f;
					}
					movementmon2.x += 0.8f;
				}

				if (gornbound.intersects(floor3down.getGlobalBounds()))
				{
					if (statemon2 == 1)
					{
						if (slowdownstate == 1)
						{
							movementmon2.x += 0.7f;
						}
						movementmon2.x -= 1.1f;
					}
					else
					{
						if (slowdownstate == 1)
						{
							movementmon2.x -= 0.7f;
						}
						movementmon2.x += 1.1f;
					}
				}
				//mon3 move//////////////////////////////////////
				if (statemon3 == 0)
				{
					if (slowdownstate == 1)
					{
						movementmon3.x -= 0.3f;
					}
					movementmon3.x += 0.5f;
				}
				else
				{
					if (slowdownstate == 1)
					{
						movementmon3.x += 0.3f;
					}
					movementmon3.x -= 0.5f;
				}
				// keyboard input //////////////////////////////
				if (Keyboard::isKeyPressed(Keyboard::X) and !pressed)
				{
					pressed = true;
					game.stop();
					button.play();
					gamestate = 1;
					score = 0;
					life = 4;
					Mode = Menu;
				}
				if (Keyboard::isKeyPressed(Keyboard::A) || Joystick::getAxisPosition(0, Joystick::PovX) < -1)
				{
					if (speedstate == 1)
					{
						movement.x -= 1.0f;
					}
					else
					movement.x -= 0.5f;
				}
				if (Keyboard::isKeyPressed(Keyboard::D) || Joystick::getAxisPosition(0, Joystick::PovX) > 5)
				{
					if (speedstate == 1)
					{
						movement.x += 1.0f;
					}
					else
					movement.x += 0.5f;
				}
				if (Keyboard::isKeyPressed(Keyboard::W) and jumpcheck and !pressed)
				{
					jump.play();
					jumpcheck = false;
					pressed = true;
					if (gornbound.intersects(ladder1.getGlobalBounds()) || gornbound.intersects(ladder2.getGlobalBounds()) || gornbound.intersects(ladder3.getGlobalBounds())
						|| gornbound.intersects(ladder4.getGlobalBounds()) || gornbound.intersects(ladder5.getGlobalBounds()) || gornbound.intersects(ladder6.getGlobalBounds())
						|| gornbound.intersects(ladder7.getGlobalBounds()) || gornbound.intersects(ladder8.getGlobalBounds()) || gornbound.intersects(ladder9.getGlobalBounds())
						|| gornbound.intersects(ladder10.getGlobalBounds()) || gornbound.intersects(ladder12.getGlobalBounds()) || gornbound.intersects(ladder13.getGlobalBounds())
						|| gornbound.intersects(ladder11.getGlobalBounds()))
					{
						movement.y -= 1.4f;
					}
					if (gornbound.intersects(floor1down.getGlobalBounds()) || gornbound.intersects(floor2down.getGlobalBounds()) || gornbound.intersects(floor3down.getGlobalBounds())
						|| gornbound.intersects(floor4down.getGlobalBounds()) || gornbound.intersects(floor1stdown.getGlobalBounds()))
					{
						movement.y -= 0.6f;
					}
				}
				if (Joystick::isButtonPressed(0, 3) and jumpcheck )
				{
					jump.play();
					jumpcheck = false;
					if (gornbound.intersects(ladder1.getGlobalBounds()) || gornbound.intersects(ladder2.getGlobalBounds()) || gornbound.intersects(ladder3.getGlobalBounds())
						|| gornbound.intersects(ladder4.getGlobalBounds()) || gornbound.intersects(ladder5.getGlobalBounds()) || gornbound.intersects(ladder6.getGlobalBounds())
						|| gornbound.intersects(ladder7.getGlobalBounds()) || gornbound.intersects(ladder8.getGlobalBounds()) || gornbound.intersects(ladder9.getGlobalBounds())
						|| gornbound.intersects(ladder10.getGlobalBounds()) || gornbound.intersects(ladder12.getGlobalBounds()) || gornbound.intersects(ladder13.getGlobalBounds())
						|| gornbound.intersects(ladder11.getGlobalBounds()))
					{
						movement.y -= 1.4f;
					}
					if (gornbound.intersects(floor1down.getGlobalBounds()) || gornbound.intersects(floor2down.getGlobalBounds()) || gornbound.intersects(floor3down.getGlobalBounds())
						|| gornbound.intersects(floor4down.getGlobalBounds()) || gornbound.intersects(floor1stdown.getGlobalBounds()))
					{
						movement.y -= 0.6f;
					}
				}
				if (Keyboard::isKeyPressed(Keyboard::S) || Joystick::isButtonPressed(0, 0))
				{
					if (gornbound.intersects(ladder00.getGlobalBounds()) || gornbound.intersects(ladder22.getGlobalBounds()) || gornbound.intersects(ladder33.getGlobalBounds())
						|| gornbound.intersects(ladder44.getGlobalBounds()) || gornbound.intersects(ladder55.getGlobalBounds()) || gornbound.intersects(ladder66.getGlobalBounds())
						|| gornbound.intersects(ladder77.getGlobalBounds()) || gornbound.intersects(ladder88.getGlobalBounds()) || gornbound.intersects(ladder99.getGlobalBounds())
						|| gornbound.intersects(ladder100.getGlobalBounds()) || gornbound.intersects(ladder122.getGlobalBounds()) || gornbound.intersects(ladder133.getGlobalBounds())
						|| gornbound.intersects(ladder111.getGlobalBounds()))
					{
						movement.y = 1.0f;
					}
				}
				//skill for item/////////////////////////////////////////////////////////
				
				// ---collision detection--- ////////////////////////////////////////////

				// ---detect gorn+mon1,mon2,mon3 --- //////////////////////////////////////////////
				if (gornbound.intersects(mon1bound))
				{
					stategorn = 1; die.play();
				}
				// ---check detect mon 1 x wall--- /////////////////////////////////////////////
				if (mon1.getGlobalBounds().intersects(floor1left.getGlobalBounds()))
					statemon1 = 1;
				if (mon1.getGlobalBounds().intersects(floor1right.getGlobalBounds()))
					statemon1 = 0;
				// ---check detect mon 2 x wall--- /////////////////////////////////////////////
				if (mon2.getGlobalBounds().intersects(floor1right.getGlobalBounds()))
					statemon2 = 0;
				if (mon2.getGlobalBounds().intersects(floor1left.getGlobalBounds()))
					statemon2 = 1;
				// ---check detect mon 3 x wall--- /////////////////////////////////////////////
				if (mon3.getGlobalBounds().intersects(floor1left.getGlobalBounds()))
					statemon3 = 1;
				if (mon3.getGlobalBounds().intersects(floor1right.getGlobalBounds()))
					statemon3 = 0;
				// ---check detect mom--- ///////////////////////////////////////////////////////
				if (gornbound.intersects(mombox.getGlobalBounds()))
				{
					if (score == 50 || score == 100)
					{
						clear.play();
						speedstate = 0;
						slowdownstate = 0;
						immortalstate = 0;
					}
					if (score == 50)
					{
						Mode = Stageclear;
						position.x = 80;
						position.y = 22;
						for (i = 0; i < 10; i++)
						{
							for (j = 0; j < 5; j++)
							{
								chicken.setPosition((i * random(110, 125)) + 20, 65 + (150 * j));
								chick.push_back(chicken);
							}
						}
					}
					else if (score == 100)
					{
						Mode = Stageclear;
						position.x = 80;
						position.y = 22;
						for (i = 0; i < 10; i++)
						{
							for (j = 0; j < 5; j++)
							{
								chicken.setPosition((i * random(110, 125)) + 20, 65 + (150 * j));
								chick.push_back(chicken);
							}
						}
					}
					else if (score == 150)
					{
						position.x = 80;
						position.y = 22;
						scorerank = score;
						name = playername;

						ofstream myFile;
						myFile.open("highscore.txt", ios::out | ios::app);
						myFile << "\n" << name << " " << scorerank;
						myFile.close();

						ifstream loadFile;
						loadFile.open("highscore.txt");
						while (!loadFile.eof()) {
							string tempName;
							int tempScore;
							loadFile >> tempName >> tempScore;
							if (tempName != "")
							{
								highscore.push_back({ tempScore,tempName });
							}
						}
						sort(highscore.begin(), highscore.end(), greater<pair<int, string>>());
						win.play();
						game.stop();
						Mode = Win;
					}
				}
				if (stategorn == 1)
				{
					if (immortalstate == 1)
					{
						stategorn = 0;
					}
					else
					{
						speedstate = 0;
						slowdownstate = 0;
						immortalstate = 0;
						stategorn = 0;
						position.x = 80;
						position.y = 22;
						life--;
					}
				}
				if (life == 0)
				{
					scorerank = score;
					name = playername;

					ofstream myFile;
					myFile.open("highscore.txt", ios::out | ios::app);
					myFile << "\n" << name << " " << scorerank;
					myFile.close();

					ifstream loadFile;
					loadFile.open("highscore.txt");
					while (!loadFile.eof()) {
						string tempName;
						int tempScore;
						loadFile >> tempName >> tempScore;
						if (tempName != "")
						{
							highscore.push_back({ tempScore,tempName });
						}
					}
					sort(highscore.begin(), highscore.end(), greater<pair<int, string>>());
					gameover.play();
					game.stop();
					gamestate = 1;
					Mode = Missionfail;
				}

				// boundary gorn ///////////////////////////////////////
				position.x = std::max(position.x, windowBounds.left);
				position.x = std::min(position.x, windowBounds.left + windowBounds.width - gornbox.getSize().x);
				position.y = std::max(position.y, windowBounds.top);
				position.y = std::min(position.y, windowBounds.top + windowBounds.height - gornbox.getSize().y);
				// set position follow position ///////////////////////
				gornbox.setPosition(position);
				// movement ///////////////////////////////////////////
				gornbox.move(movement);
				mon1.move(movementmon1);
				mon2.move(movementmon2);
				mon3.move(movementmon3);

				while (window.pollEvent(event))
				{
					switch (event.type)
					{
					case Event::Closed:
						window.close();
						game.stop();
						break;
					case Event::Resized:
						break;
					case Event::KeyReleased:
						pressed = false;
						break;
					}
				}

				// แสดง animation /////////////////////////////////////
				window.clear();
				window.draw(ladder1);
				window.draw(ladder2);
				window.draw(ladder3);
				window.draw(ladder4);
				window.draw(ladder5);
				window.draw(ladder6);
				window.draw(ladder7);
				window.draw(ladder8);
				window.draw(ladder9);
				window.draw(ladder10);
				window.draw(ladder11);
				window.draw(ladder12);
				window.draw(ladder13);
				window.draw(ladder00);
				window.draw(ladder22);
				window.draw(ladder33);
				window.draw(ladder44);
				window.draw(ladder55);
				window.draw(ladder66);
				window.draw(ladder77);
				window.draw(ladder88);
				window.draw(ladder99);
				window.draw(ladder100);
				window.draw(ladder111);
				window.draw(ladder122);
				window.draw(ladder133);
				window.draw(floor1left);
				window.draw(floor1right);
				window.draw(floor1stdown);
				window.draw(floor1down);
				window.draw(floor2down);
				window.draw(floor2down2);
				window.draw(floor3down);
				window.draw(floor4down);
				window.draw(BG);
				
				for (i = 0; i < chick.size(); i++)
				{
					if (gornbound.intersects(chick[i].getGlobalBounds()))
					{
						collect.play();
						chick.erase(chick.begin() + i);
						score++;
					}
					else window.draw(chick[i]);
				}

				for (int Life = 0; Life < heartvec.size(); Life++)
				{
					if (Life==life) heartvec.erase(heartvec.begin()+Life);
					else window.draw(heartvec[Life]);
				}
				for (i = 0; i < itemvec.size(); i++)
				{
					itemReg.move(0.0f,0.5f);
					if (itemstate == 1)
					{
						currentitem.x = Random;
						itembox.left = currentitem.x*itembox.width;
						itemReg.setTextureRect(itembox);
						itemposition = itemReg.getPosition();
						if (itemReg.getGlobalBounds().intersects(gornbound))
							{
							itemsound.play();
							if (currentitem.x == 0) { speedstate = 1; itemtime = currenttime;  }
							if (currentitem.x == 1){slowdownstate = 1; itemtime = currenttime; }
							if (currentitem.x == 2){immortalstate = 1; itemtime = currenttime; }
							itemvec.erase(itemvec.begin()+i);
							itemstate = 0;
						}
						else if (itemposition.y == 650)
						{
							itemvec.erase(itemvec.begin() + i);
							itemstate = 0;
						}
						else
						{
							window.draw(itemReg);
						}
					}
					if (currenttime - itemtime >= 3.0f){speedstate = 0; slowdownstate = 0; immortalstate = 0;}
				}
				if (speedstate == 1)
				{
					itemReg.setSize(Vector2f(60, 60));
					itemReg.setPosition(800, 5);
					itembox.left = 0*itembox.width;
					itemReg.setTextureRect(itembox);
					window.draw(itemReg);
				}
				if (slowdownstate == 1)
				{
					itemReg.setSize(Vector2f(60, 60));
					itemReg.setPosition(800, 5);
					itembox.left = 1 * itembox.width;
					itemReg.setTextureRect(itembox);
					window.draw(itemReg);
				}
				if (immortalstate == 1)
				{
					itemReg.setSize(Vector2f(60, 60));
					itemReg.setPosition(800, 5);
					itembox.left = 2 * itembox.width;
					itemReg.setTextureRect(itembox);
					window.draw(itemReg);
				}
				gornbox.setTextureRect(boxgorn); //นำกรอบที่สร้างมาใส่ใน box
				window.draw(gornbox); //แสดงอนิเมชั่น
				mon1.setTextureRect(boxmon1);
				mon2.setTextureRect(boxmon2);
				mon3.setTextureRect(boxmon3);
				window.draw(mon1);
				if (gamestate == 2)
				{
					if (gornbound.intersects(mon3.getGlobalBounds())) {
						die.play();
						stategorn = 1;
					}
					window.draw(mon3);
				}
				else if (gamestate == 3)
				{
					if (gornbound.intersects(mon2.getGlobalBounds()) || gornbound.intersects(mon3.getGlobalBounds()))
					{
						stategorn = 1; die.play();
					}
					window.draw(mon2);
					window.draw(mon3);
				}
				
				window.draw(credit);
				mombox.setTextureRect(boxmom);
				window.draw(mombox);
				window.draw(Sboard);
				window.draw(text);
				window.display();
			}
		case Missionfail:
			while (Mode == Missionfail)
			{
				updatescore.setPosition(577, 570);
				updatescore.setString(to_string(score));
				totalTime1 += narika2.restart().asSeconds(); //บวกสะสมเวลาที่ใช้ใน loop แต่ละรอบ
				if (totalTime1 >= switchTime1) //ถ้าเวลาสะสมมากกว่าหรือเท่ากับเวลาในการเปลี่ยนฉาก
				{
					currentscene.x++;
					if (currentscene.x >= 3.0f)
					{
						currentscene.x = 0;
					}
					gameoverbox.left = currentscene.x*gameoverbox.width;
					totalTime1 -= switchTime1;
				}
				if (Keyboard::isKeyPressed(Keyboard::Space) and!pressed)
				{
					pressed = true;
					gameover.stop();
					button.play();
					Mode = Rank;
				}
				while (window.pollEvent(event))
				{
					switch (event.type)
					{
					case sf::Event::Closed:
						window.close();
						gameover.stop();
						break;
					case sf::Event::Resized:
						break;
					case Event::KeyReleased:
						pressed = false;
						break;
					}
				}
				window.clear();
				OV.setTextureRect(gameoverbox);
				window.draw(OV);
				window.draw(updatescore);
				window.draw(credit);
				window.display();
			}
		case Rank:
			while (Mode == Rank)
			{
				if (Keyboard::isKeyPressed(Keyboard::X))
				{
					button.play();
					Mode = Menu;
				}
				if (Keyboard::isKeyPressed(Keyboard::Space)&&(score==150||life==0) and !pressed)
				{
					pressed = true;
					button.play();
					menu.play();
					Mode = Menu;
				}
				window.clear();
				window.draw(srankboard);
				int cnt = 0;  //ประกาศตัวนับ
				for (vector<pair<int, string>>::iterator i = highscore.begin(); i != highscore.end(); ++i) {
					++cnt;
					if (cnt > 5) break;                       //เมื่อตัวนับเกิน 5 ให้จบการทำงาน
					Font myfont;                              //การตั้งค่า Font
					myfont.loadFromFile("font/Handsdown.ttf");    //โหลดไฟล์ Font
					Text a, b;                                //ประกาศ Text
					a.setString(to_string(i->first));         //เนื่องจากค่าคะแนนเป็น integer จึงต้องทำการแปลงเป็น string ก่อนนำมาแสดงผล (first คือpair ตัวที่หนึ่ง =>int)
					a.setFont(myfont);                        //การตั้งค่า Font คะแนน
					a.setCharacterSize(50);                   //ตั้งค่าขนาด Font คะแนน
					a.setPosition(900, 120 + (80 * cnt));      //ตั้งค่าตำแหน่งแสดงผลของคะแนน และเพิ่มค่าตำแหน่งให้ลงมาตัวละ 80 หน่วย
					window.draw(a);                           //แสดงผลคะแนน
					b.setString(i->second);            // (second คือpair ตัวที่สอง =>string) 
					b.setFont(myfont);                        //การตั้งค่า Font ชื่อผู้เล่น
					b.setCharacterSize(50);                   //ตั้งค่าขนาด Font ชื่อผู้เล่น
					b.setPosition(200, 120 + (80 * cnt));      //ตั้งค่าตำแหน่งแสดงผลของชื่อผู้เล่น และเพิ่มค่าตำแหน่งให้ลงมาตัวละ 80 หน่วย
					window.draw(b);                           //แสดงผลชื่อผู้เล่น
				}
				window.draw(credit);
				window.display();

				while (window.pollEvent(event))
				{
					switch (event.type)
					{
					case Event::Closed:
						window.close();
						menu.stop();
						break;
					case Event::Resized:
						break;
					case Event::KeyReleased:
						pressed = false;
						break;
					}
				}
			}
			
		case Tutorial :
			while (Mode == Tutorial)
			{
				
				if (Keyboard::isKeyPressed(Keyboard::X) and !pressed)
				{
					menu.stop();
					button.play();
					position.x = 80;
					position.y = 22;
					life = 4;
					score = 0;
					gamestate = 1;
					Mode = Menu;
					menu.play();
				}
				if (Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
				{
					pressed = true;
					button.play();
					menu.stop();
					life = 4;
					score = 0;
					gamestate = 1;
					chick.clear();
					for (i = 0; i < 10; i++)
					{
						for (j = 0; j < 5; j++)
						{
							chicken.setPosition((i * random(110, 125)) + 20, 65 + (150 * j));
							chick.push_back(chicken);
						}
					}
					heartvec.clear();
					for (i = 0; i < 4; i++)
					{
						Sheart.setPosition((i * 55) + 870, 10);
						heartvec.push_back(Sheart);
					}
					Mode = Game;
				}
				while (window.pollEvent(event))
				{
					switch (event.type)
					{
					case Event::Closed:
						window.close();
						menu.stop();
						break;
					case Event::Resized:
						break;
					case Event::KeyReleased:
						pressed = false;
						break;
					}
				}
				window.clear();
				window.draw(showtoplay);
				window.draw(credit);
				window.display();
			}
			case Stageclear:
			{
				while (Mode == Stageclear)
				{
					game.stop();
					
					if (Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
					{
						clear.stop();
						button.play();
						pressed = true;
						Mode = Game; gamestate ++;
					}
					while (window.pollEvent(event))
					{
						switch (event.type)
						{
						case Event::Closed:
							window.close();
							clear.stop();
							break;
						case Event::Resized:
							break;
						case Event::KeyReleased:
							pressed = false;
							break;
						}
					}
					window.clear();
					window.draw(sstageclear);
					window.draw(credit);
					window.display();

				}
			}
			case Win :
				while (Mode == Win)
				{
					updatescore.setPosition(760, 582);
					updatescore.setString(to_string(score));
					if (Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
					{
						pressed = true;
						button.play();
						for (i = 0; i < heartvec.size(); i++)
						{
							heartvec.erase(heartvec.begin());
						}
						Mode = Rank;
					}
					while (window.pollEvent(event))
					{
						switch (event.type)
						{
						case Event::Closed:
							window.close();
							win.stop();
							break;
						case Event::Resized:
							break;
						case Event::KeyReleased:
							pressed = false;
							break;
						}
					}
					window.clear();
					window.draw(smissioncomplete);
					window.draw(updatescore);
					window.draw(credit);
					window.display();
				}
			case Name:
			{
				while (Mode==Name)
				{
					while (window.pollEvent(event)) {
						if (event.type == sf::Event::TextEntered) {
							if (event.text.unicode == '\b') {//ถ้ากด Backspace เป็นการลบตัวอักษร
								if(playername.getSize()>=1) playername.erase(playername.getSize() - 1, 1);
								player.setFont(fontname);
								player.setString(playername);
							}
							else {
								string name;
								if (playername.getSize() <= 8)
								{
									playername += static_cast<char>(event.text.unicode);
									name += static_cast<char>(event.text.unicode);
								}
								
								if ((event.text.unicode < 128)) {
									statetext = 0;
									player.setFont(fontname);
									player.setString(playername);
								}
							}
							player.setCharacterSize(120);   //เซ็ตขนาดของข้อความ
							player.setPosition(500.0f, 318.0f);  //เซ็ตขนาดของข้อความ
							player.setFillColor(Color::Black);
						}
						else if (event.type == sf::Event::KeyPressed) {
							if (event.key.code == sf::Keyboard::Space) {
								player.setString(playername);
							}
						}
						if (Keyboard::isKeyPressed(Keyboard::Space) and !pressed)
						{
							pressed = true;
							button.play();
							Mode = Tutorial;
						}
						switch (event.type)
							{
							case Event::Closed:
								window.close();
								menu.stop();
								break;
							case Event::Resized:
								break;
							case Event::KeyReleased:
								pressed = false;
								break;
							}
					}
					window.clear();
					window.draw(snameplayer);
					if(statetext==0) window.draw(player);
					window.draw(credit);
					window.display();
				}
				
			}
			case pause:
				while (Mode == pause)
				{
					Texture pause;
					pause.loadFromFile("animation/pause.png");
					Sprite spause;
					spause.setTexture(pause);
					window.clear();
					window.draw(spause);
					window.display();
					pauseGame = true;
					while (pauseGame == true) {
						while (window.pollEvent(event)) {
							if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Return) {
								button.play();
								Mode = Game;
								pauseGame = false;
								narika.restart();
							}
							switch (event.type)
							{
							case Event::Closed:
								window.close();
								menu.stop();
								break;
							case Event::Resized:
								break;
							}
						}
					}
					
				}

		}
			
	}
	cin.ignore();
	return 0;
}