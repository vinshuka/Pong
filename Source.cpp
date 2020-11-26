#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Pong : public olc::PixelGameEngine
{
public:
	Pong()
	{
		sAppName = "PONG";
	}

private:
	//Paddles
	float fPaddleOnePos = 100.0f;
	float fPaddleTwoPos = 100.0f;
	float fPaddleWidth = 10.0f;
	float fPaddleSpeed = 250.0f;

	//Ball
	olc::vf2d vBall = { 240.0f, 120.0f };
	olc::vf2d vBallVel = { -120.0f, 120.0f };

	//Player scores
	int P1 = 0;
	int P2 = 0;	

	//WinLose text
	std::string P1WinLose = "";
	std::string P2WinLose = "";

	bool start = false;

public:
	bool OnUserCreate() override
	{
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Draw Display--------------------------------------------------------

		
		Clear(olc::BLACK);

		DrawString(184, 100, "PONG", olc::WHITE, 3);
		DrawString(154, 140, "Press Enter To Start", olc::WHITE, 1);
		if (GetKey(olc::ENTER).bPressed)
			start = true;

		if (start)
		{
			//Draw Screen
			Clear(olc::BLACK);

			//Draw Divider
			DrawLine(240, 0, 240, 240, olc::WHITE);

			//Draw Score
			DrawString(192, 5, std::to_string(P1), olc::WHITE, 3);
			DrawString(268, 5, std::to_string(P2), olc::WHITE, 3);

			//Draw WinLose
			DrawString(64, 120, P1WinLose, olc::WHITE, 3);
			DrawString(304, 120, P2WinLose, olc::WHITE, 3);

			//Draw padle one
			FillRect(10, int(fPaddleOnePos), int(fPaddleWidth), 40, olc::WHITE);
			//Draw padle two
			FillRect(460, int(fPaddleTwoPos), int(fPaddleWidth), 40, olc::WHITE);

			//Draw Ball
			FillCircle(vBall, 5, olc::WHITE);

			// User Input----------------------------------------------------------

			//User Input Padle One
			if (GetKey(olc::Key::S).bHeld)
				fPaddleOnePos += fPaddleSpeed * fElapsedTime;
			if (GetKey(olc::Key::W).bHeld)
				fPaddleOnePos -= fPaddleSpeed * fElapsedTime;

			// Top Paddle One Boundary
			if (fPaddleOnePos < 10.0f)
				fPaddleOnePos = 10.f;

			// Bottom Paddle One Boundary
			if (fPaddleOnePos + 40 > float(ScreenHeight()) - 10.f)
				fPaddleOnePos = float(ScreenHeight()) - 10.0f - 40;

			//User Input Padle Two
			if (GetKey(olc::Key::DOWN).bHeld)
				fPaddleTwoPos += fPaddleSpeed * fElapsedTime;
			if (GetKey(olc::Key::UP).bHeld)
				fPaddleTwoPos -= fPaddleSpeed * fElapsedTime;

			// Collision-----------------------------------------------------------

			// Top Paddle Two Boundary
			if (fPaddleTwoPos < 10.0f)
				fPaddleTwoPos = 10.f;
			// Bottom Paddle Two Boundary
			if (fPaddleTwoPos + 40 > float(ScreenHeight()) - 10.f)
				fPaddleTwoPos = float(ScreenHeight()) - 10.0f - 40;

			//Update ball
			vBall += vBallVel * fElapsedTime;

			// Top Boundary for ball
			if (vBall.y <= 10.0f)
				vBallVel.y *= -1.0f;

			// Bottom Boundary for Ball
			if (vBall.y >= float(ScreenHeight()) - 10.0f)
				vBallVel.y *= -1.0f;

			// Check if Ball hits paddle one
			if ((vBall.x <= 30.0f) && (vBall.y > fPaddleOnePos) && (vBall.y < fPaddleOnePos + 40.0f))
				vBallVel.x *= -1.0f;

			// Check if Ball hits paddle two
			if ((vBall.x >= float(ScreenWidth()) - 30.0f) && (vBall.y > fPaddleTwoPos) && (vBall.y < fPaddleTwoPos + 40.0f))
				vBallVel.x *= -1.0f;

			// Win/Lose Condition--------------------------------------------------

			// Check if ball goes out of bounds
			if (vBall.x < 0 || vBall.x > ScreenWidth())
			{
				// Out on paddle one side
				if (vBall.x < 0)
				{
					// Increase player 2 score and draw it
					P2++;
					DrawString(252, 5, std::to_string(P2), olc::WHITE, 3);
				}

				// Out on paddle two side
				if (vBall.x > ScreenWidth())
				{
					// Increase player 1 score and draw it
					P1++;
					DrawString(208, 5, std::to_string(P1), olc::WHITE, 3);

				}

				// Check player 2 win, stop ball, set winLose text
				if (P2 >= 10) {
					vBallVel = { 0.0f, 0.0f };
					P2WinLose = "Win!";
					P1WinLose = "Lose.";
				}

				// Check player 1 win, stop ball, set winLose text
				if (P1 >= 10) {
					vBallVel = { 0.0f, 0.0f };
					P1WinLose = "Win!";
					P2WinLose = "Lose.";
				}

				// Restart ball at center
				vBall = { 240.0f, 120.0f };
			}

			// resets the game variables, this could be done better by discarding the current instance and starting a new one
			if (GetKey(olc::ESCAPE).bPressed)
			{
				vBall = { 240.0f, 120.0f };
				vBallVel = { -120.0f, 120.0f };
				P1 = 0;
				P2 = 0;
				P1WinLose = "";
				P2WinLose = "";

				start = false;
			}
				
		}
		
		return true;
	}
};

int main()
{
	Pong pong;
	if (pong.Construct(480, 240, 2, 2))
		pong.Start();
	return 0;
}